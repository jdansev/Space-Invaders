#include "score.h"

Score::Score(QWidget* parent) : score(0)
{

    game::Config* c = game::Config::getInstance();
    WIDTH = c->get_SCALEDWIDTH();
    HEIGHT = c->get_SCALEDHEIGHT();

    labelWidth = 140;
    labelHeight = 20;

    scoreLabel = new QLabel(parent);
    scoreLabel->setVisible(true);

    scoreLabel->setStyleSheet("color:#F7F9F9; background-color:rgba(36, 113, 163, 100)");
    QFont font("IBM 3270", 16);
    scoreLabel->setFont(font);
    scoreLabel->setAlignment(Qt::AlignCenter);

    name = new QLabel(parent);
    name->setVisible(true);
    name->setStyleSheet("color:#F7F9F9; background-color:rgba(36, 113, 163, 100)");
    name->setFont(font);
    name->setAlignment(Qt::AlignCenter);
    name->setText("PLAYER:" + c->get_name());
    name->setGeometry(QRect(0, 14, labelWidth, labelHeight));

    redraw();
}

void Score::redraw() {

    scoreLabel->setText("SCORE:" + QString::number(score));
    scoreLabel->setGeometry(QRect(WIDTH-labelWidth, 14, labelWidth, labelHeight));

}

// Increment the score by 1 point, then redraws it to the screen.
void Score::updateScore(int currentScore) {
    score = currentScore;
    redraw();
}
