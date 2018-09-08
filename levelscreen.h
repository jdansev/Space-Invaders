#ifndef LEVELSCREEN_H
#define LEVELSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QFont>
#include <QRect>
#include <QSound>

#include "config.h"

class LevelScreen
{
public:
    LevelScreen(QWidget* parent);
    void advanceLevel();
    void hide();
private:
    QLabel *levelScreenLabel;
    int level;
    int labelWidth;
    int labelHeight;
    int WIDTH;
    int HEIGHT;
};

#endif // LEVELSCREEN_H
