#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "config.h"
#include <QLabel>
#include <QFile>
#include <QDebug>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class GameOver
{
public:
    GameOver(QWidget *parent);
    void display(int playerScore);

private:
    game::Config* c;
    QLabel *bg;
    QLabel *title;
    QLabel *scoreboard;
    int WIDTH;
    int HEIGHT;

    vector<pair<int, string> > getScores();
    QString scoresAsText(vector<pair<int, string> > scoreList);
    void appendScore(string playerName, int playerScore);

};

#endif // GAMEOVER_H
