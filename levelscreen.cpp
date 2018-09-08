#include "levelscreen.h"

LevelScreen::LevelScreen(QWidget *parent) : levelScreenLabel(new QLabel(parent)), level(1)
{

    labelWidth = 300;
    labelHeight = 400;

    game::Config* c = game::Config::getInstance();
    WIDTH = c->get_SCALEDWIDTH();
    HEIGHT = c->get_SCALEDHEIGHT();

    QFont font("IBM 3270 Semi-Narrow");
    font.setPointSize(60);
    levelScreenLabel->setFont(font);
    levelScreenLabel->setAlignment(Qt::AlignCenter);

}

void LevelScreen::advanceLevel() {

    levelScreenLabel->setGeometry(QRect(WIDTH/2-labelWidth/2, HEIGHT/2-labelHeight/2, labelWidth, labelHeight));
    levelScreenLabel->setText("<font color='white'>LEVEL<br>" + QString::number(++level) + "</font>");
    levelScreenLabel->show();

}

void LevelScreen::hide()
{
    levelScreenLabel->hide();
}
