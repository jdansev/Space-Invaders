#ifndef SCORE_H
#define SCORE_H

#include <QLabel>
#include <QString>
#include "config.h"

class Score
{
public:
    Score(QWidget* parent);
    void updateScore(int currentScore);

private:
    QLabel *scoreLabel;
    QLabel *name;
    int WIDTH;
    int HEIGHT;
    int labelWidth;
    int labelHeight;
    int score;
    void redraw();
};

#endif // SCORE_H
