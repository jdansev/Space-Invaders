#pragma once

#include "config.h"
#include "menu.h"
#include "ship.h"
#include "swarm.h"
#include "swarminfo.h"
#include "levelscreen.h"
#include "pausemenu.h"
#include "score.h"
#include "gameover.h"
#include <QDialog>
#include <QSoundEffect>
#include <QWidget>
#include <vector>

namespace game {
class GameDialog : public QDialog {
    Q_OBJECT

public:
    GameDialog(QWidget* parent = nullptr);
    void generateAliens(const QList<SwarmInfo>& swarms);
    virtual ~GameDialog();

protected:
    QTimer* timer;
    void paintEvent(QPaintEvent* event);
    void paintBullets(QPainter& painter);
    void updateBullets();
    void paintSwarm(QPainter& painter, AlienBase*& root);
    void checkSwarmCollisions(AlienBase*& root);
    // ship and swarms
    Ship* ship;
    std::vector<Bullet*> bullets;
    AlienBase* swarms;  // swarms is the ROOT node of the composite
    QSoundEffect shipFiringSound;
    int next_instruct;

    // helper
    bool checkAliensDefeated();
    void resetGame();
    void stopGame();
    void endGame();

    // keys
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent *event);

    // mouse
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *eventPress);

    // level screen
    LevelScreen* levelScreen;

    // pause menu
    PauseMenu* pauseMenu;

    // score
    Score *score;

    // gameover screen
    GameOver* gameOver;

    // about the canvas
    int frames;
    const int WIDTH = 800;
    const int HEIGHT = 600;
    int SCALEDWIDTH;
    int SCALEDHEIGHT;
    bool movingRight = false;
    bool movingLeft = false;

    // collision...
    int get_collided_swarm(Bullet*& b, AlienBase*& root);
    int get_collided(Bullet*& b, AlienBase*& root);
    void addBullets(const QList<Bullet*>& list);

    // pausing & menu
    bool sound;
    bool paused;
    void pauseStart();
    Menu* menu;

    // score
    int gameScore;  // this run's score.
public slots:
    void nextFrame();
    void showScore();

    // menu functions
    void restartLevel();
    void toggleSound();
    void toggleSwarmSound(AlienBase *&root, bool status);
    void pause();
    void resume();

};
}
