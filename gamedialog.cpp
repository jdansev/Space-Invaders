#include "gamedialog.h"
#include "bullet.h"
#include "ship.h"
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QSound>
#include <QTimer>
#include <QWidget>
#include <vector>
#include <iostream>

namespace game {

GameDialog::GameDialog(QWidget* parent)
        : QDialog(parent), bullets(), shipFiringSound(this), gameScore(0), sound(true) {
    // SET UP GAME DIMENSIONS AND CONFIG
    Config* c = Config::getInstance();
    SCALEDWIDTH = c->get_SCALEDWIDTH();
    SCALEDHEIGHT = c->get_SCALEDHEIGHT();
    this->frames = c->get_frames();

    setCursor(Qt::BlankCursor);

    // mouse tracking
    setMouseTracking(true);

    // level screen
    levelScreen = new LevelScreen(this);

    // pause menu
    pauseMenu = new PauseMenu(this);

    // score
    score = new Score(this);

    // gameover screen
    gameOver = new GameOver(this);

    // MENU
    QList<QPair<QString, int>> dummy;
    menu = new Menu(this, c->get_name(), this->gameScore, dummy);

    // EXTENSION STAGE 1 PART 1 - RESCALE GAME SCREEN FOR SHIP SIZE
    this->setFixedWidth(SCALEDWIDTH);
    this->setFixedHeight(SCALEDHEIGHT);
    // SHIP
    QPixmap ship;
    ship.load(":/Images/ship.png");
    this->ship = new Ship(ship, c->get_scale(), c->get_startpos(), SCALEDHEIGHT);
    this->next_instruct = 0;
    // SHIP SOUND
    shipFiringSound.setSource(QUrl::fromLocalFile(":/Sounds/shoot.wav"));
    shipFiringSound.setVolume(0.3f);

    // ALIENS
    generateAliens(c->getSwarmList());

    // SET BACKGROUND
    setStyleSheet("background-color: #000000;");

    paused = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    timer->start(this->frames);

    update();
}

GameDialog::~GameDialog() {
    delete ship;
    delete timer;  // optional, don't have to do this apparently

    // loop though swarms to delete aliens
    delete swarms;  // recursively deletes itself.
    for (Bullet* b : bullets) {
        delete b;
    }
}

// make the swarms for this level.
void GameDialog::generateAliens(const QList<SwarmInfo>& makeSwarms) {
    SwarmInfo def = SwarmInfo();

    swarms = new Swarm(def, *this->ship);
    for (SwarmInfo info : makeSwarms) {
        AlienBase* s = new Swarm(info, *this->ship);
        swarms->add(s);
    }

    // set sound to current sound fx setting
    toggleSwarmSound(swarms, sound);
}

void GameDialog::pauseStart() {

    if (paused) {
        resume();
        //this->menu->displayMenu(paused);

    } else {
        pause();
        pauseMenu->displayMenu(sound);
        //this->menu->displayMenu(paused);

    }

}

void GameDialog::pause() {

    setCursor(Qt::ArrowCursor);
    this->paused = true;
    this->timer->stop();

}

void GameDialog::resume() {

    setCursor(Qt::BlankCursor);
    pauseMenu->hideMenu();
    this->paused = false;
    this->timer->start(frames);

}

void GameDialog::toggleSwarmSound(AlienBase*& root, bool status) {

    if (root->getAliens().size() == 0)
        return;

    for (int i = 0; i < root->getAliens().size(); i++) {
        AlienBase* child = root->getAliens().at(i);
        if (status)
            child->bulletSFX.setVolume(0.3f);
        else
            child->bulletSFX.setVolume(0.0f);
        toggleSwarmSound(child, status);
    }

    return;
}

void GameDialog::toggleSound() {
    if (sound) {
        shipFiringSound.setVolume(0.0f);
        toggleSwarmSound(swarms, false);
        sound = false;
    }
    else {
        shipFiringSound.setVolume(0.3f);
        toggleSwarmSound(swarms, true);
        sound = true;
    }
    pauseMenu->displayMenu(sound);
}

void GameDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (!paused) {
        ship->set_x(event->pos().x() - ship->get_image().width()/2);
    }
}

void GameDialog::mousePressEvent(QMouseEvent *eventPress) {

    if (!paused) {
        if (eventPress->button() == Qt::LeftButton) {
            bullets.push_back(this->ship->shoot());
            this->shipFiringSound.play();
        }
    }

}

Config* c;
bool levelAdvance = false;

void GameDialog::resetGame() {

    generateAliens(c->getSwarmList());
    levelScreen->hide();
    resume();
    levelAdvance = false;

    if (frames >= 20) {
        frames -= 5;
    }


}

void GameDialog::restartLevel() {

    // clear the bullets
    bullets.clear();
    // centre the spaceship
    ship->set_x(c->get_SCALEDWIDTH()/2-ship->get_image().width()/2);
    // restore all variables to initial states
    resetGame();
}

void GameDialog::stopGame() {

    // clear the bullets
    bullets.clear();

    // centre the spaceship
    ship->set_x(c->get_SCALEDWIDTH()/2-ship->get_image().width()/2);

    // stop the game
    pause();

    // display the new level screen
    levelScreen->advanceLevel();

    // enable level advance by pressing spacebar
    levelAdvance = true;

    // redraw elements
    update();
    updateBullets();

}

void GameDialog::endGame() {
    pause();
    setCursor(Qt::ArrowCursor);
    gameOver->display(gameScore);
}

void GameDialog::keyReleaseEvent(QKeyEvent* event) {
    if (!paused) {
        // keyboard control of the ship
        if (event->key() == Qt::Key_Left)
            movingLeft = false;
        else if (event->key() == Qt::Key_Right)
            movingRight = false;
    }
}


void GameDialog::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_P)
        pauseStart();

    if (!paused) {
        // keyboard control of the ship
        if (event->key() == Qt::Key_Left)
            movingLeft = true;
        else if (event->key() == Qt::Key_Right)
            movingRight = true;
        else if (event->key() == Qt::Key_Space) {
            bullets.push_back(this->ship->shoot());
            this->shipFiringSound.play();
        }
    }

    if (levelAdvance && event->key() == Qt::Key_Space)
        resetGame();

}

// shows this game score
void GameDialog::showScore() {
    // in future, implement 'score list' in menu.
    menu->openScore();
}

bool GameDialog::checkAliensDefeated() {
    if (swarms->getAliens().size() == 0) {
        setCursor(Qt::ArrowCursor);
        stopGame();
        return true;
    }
    return false;
}

// FOLLOWING EACH INSTRUCTION TO FRAME - for PLAYER ship.
void GameDialog::nextFrame() {

    if (!paused) {
        c = Config::getInstance();

        QStringList instruct = c->get_instructs();
        if (next_instruct >= instruct.size()) {
            next_instruct = next_instruct % instruct.size();
        }

        // move the ship
        if (movingRight && !movingLeft)
            ship->move_right();
        else if (movingLeft && !movingRight)
            ship->move_left();

        //QString ins = instruct[next_instruct];
        //next_instruct++;

        //if (ins == "Left") {
        //    ship->move_left();

        //} else if (ins == "Right") {
        //    ship->move_right();

        //} else if (ins == "Shoot") {
        //    bullets.push_back(this->ship->shoot());
        //    this->shipFiringSound.play();
        //}

        checkAliensDefeated();

        // loop through each alien swarm, move and calculated collisions
        swarms->move("");  // recursive.
        checkSwarmCollisions(swarms);

        addBullets(swarms->shoot(""));

        updateBullets();
        update();

    }

}

void GameDialog::paintBullets(QPainter& painter) {
    for (int i = 0; i < bullets.size(); i++) {
        Bullet* b = bullets[i];
        painter.drawPixmap(b->get_x(), b->get_y(), b->get_image());
    }
}

void GameDialog::updateBullets()
{
    for (int i = 0; i < bullets.size(); i++) {
        Bullet* b = bullets[i];
        // WHEN BULLET OFF GAME SCREEN, FREE MEMORY AND DELETE
        int score = get_collided(b, swarms);
        if (b->get_y() < 0 || b->get_y() >= SCALEDHEIGHT || b->get_x() < 0 ||
                b->get_x() >= SCALEDWIDTH || score > 0) {
            delete b;
            bullets.erase(bullets.begin() + i);
            i--;
        } else if (score == -1) {
            // DEAD SHIP!
            score = 0;
        } else
        {
            b->move();// we move at the end so that we can see collisions before the game ends
        }

        gameScore += score;

    }

    // update the game score
    score->updateScore(gameScore);

}

// recurse throughout the tree and draw everything.
// will also
void GameDialog::paintSwarm(QPainter& painter, AlienBase*& root) {
    for (AlienBase* child : root->getAliens()) {
        // if the child is a leaf (i.e., an alien that has no children), draw it.
        const QList<AlienBase*>& list = child->getAliens();
        if (list.size() == 0) {  // leaf
            painter.drawPixmap(child->get_x(), child->get_y(), child->get_image());
        } else {
            paintSwarm(painter, child);
        }
    }
}

//check if any aliens based off the alien root are crashing with the player ship.
void GameDialog::checkSwarmCollisions(AlienBase *&root)
{
    for (AlienBase* child : root->getAliens()) {
        // if the child is a leaf (i.e., an alien that has no children), check for collisions.
        const QList<AlienBase*>& list = child->getAliens();
        if (list.size() == 0) {  // leaf
            // check if it is crashing into the player ship
            if (child->collides(*this->ship)) {
                close();  // DEAD SHIP AGAIN
            }
        } else {
          checkSwarmCollisions(child);
        }
    }
}

// PAINTING THE SHIP AND ANY BULLETS
void GameDialog::paintEvent(QPaintEvent*) {

    // SHIP
    QPainter painter(this);

    painter.drawPixmap(ship->get_x(), ship->get_y(), ship->get_image());

    // loop through each alien swarm and draw
    paintSwarm(painter, swarms);

    // BULLETS last so they draw over everything
    paintBullets(painter);
}

// if this bullet is unfriendly, only check if it hits Ship
// if this bullet is friendly, will check the swarm;
// returns the score from the deleted hit object.
// Returns 0 if nothing hit, -ve if ship is hit.
int GameDialog::get_collided(Bullet*& b, AlienBase*& root) {

    int totalScore = 0;

    // if it's an enemy bullet, then don't look at the swarm.
    if (!b->isFriendly()) {
        // check it hits the player ship
        if (b->collides(*this->ship)) {
            totalScore = -1;

            // player has been hit
            endGame();


        }  // future; add barriers here.
    } else {
        totalScore += get_collided_swarm(b, root);
    }
    return totalScore;
}

// helper function, recursively searches swarms.
int GameDialog::get_collided_swarm(Bullet*& b, AlienBase*& root) {
    int totalScore = 0;
    // Case 1: you are a leaf node, or root node
    if (root->getAliens().size() == 0) {
        // check collision.
        if (b->collides(*root)) {
            // if it is an alien, it will return >0.
            // if it's a swarm (i.e., root) it will return 0.

            return root->get_score();
        }

    } else {
        for (int i = 0; i < root->getAliens().size(); i++) {
            AlienBase* child = root->getAliens().at(i);
            // if it's another swarm, recurse down the tree
            totalScore += get_collided_swarm(b, child);
            // if something was hit, score > 0
            if (totalScore > 0 && child->getAliens().size() == 0) {
                // some children shoot more bullets when they die.
                // ask child for reaction when you're going to delete them
                addBullets(child->react());
                root->remove(child);
                i--;

                return totalScore;
            }
        }
    }
    return totalScore;
}

void GameDialog::addBullets(const QList<Bullet*>& list) {
    for (Bullet* b : list) {
        this->bullets.push_back(b);
    }
}
}
