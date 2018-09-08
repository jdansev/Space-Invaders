#include "pausemenu.h"

PauseMenu::PauseMenu(QWidget* parent)
{

    game::Config* c = game::Config::getInstance();
    int WIDTH = c->get_SCALEDWIDTH();
    int HEIGHT = c->get_SCALEDHEIGHT();
    int labelWidth = 170;
    int labelHeight = 30;

    QFont optionsFont("Courier", 13);
    QFont titleFont("IBM 3270 Semi-Narrow", 25, QFont::Bold);

    bg = new QLabel(parent);
    bg->setGeometry(QRect(0, 0, WIDTH, HEIGHT));
    bg->setVisible(false);
    bg->setStyleSheet("background-color: rgba(52, 73, 94, 150)");

    int titleWidth = 200;
    int titleHeight = 100;
    title = new QLabel(parent);
    title->setVisible(false);
    title->setText("PAUSED");
    title->setGeometry(QRect(WIDTH/2-titleWidth/2, HEIGHT/2-titleHeight/2-100, titleWidth, titleHeight));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color:white; background-color: rgba(0,0,0,0)");
    title->setFont(titleFont);

    soundfx = new QPushButton("SHIP SFX: ON", parent);
    soundfx->setGeometry(QRect(WIDTH/2-labelWidth/2, HEIGHT/2-labelHeight/2+(labelHeight*-1)-15, labelWidth, labelHeight));
    soundfx->setVisible(false);
    soundfx->setStyleSheet("background-color:#008080; border:none");
    soundfx->setFont(optionsFont);
    QObject::connect(soundfx, SIGNAL(released()), parent, SLOT(toggleSound()));

    resume = new QPushButton("RESUME", parent);
    resume->setGeometry(QRect(WIDTH/2-labelWidth/2, HEIGHT/2-labelHeight/2+(labelHeight*0)+18, labelWidth, labelHeight));
    resume->setVisible(false);
    resume->setStyleSheet("background-color:#148F77; color:#FAFAFA; border:none");
    resume->setFont(optionsFont);
    QObject::connect(resume, SIGNAL(released()), parent, SLOT(resume()));

    restart = new QPushButton("RESTART", parent);
    restart->setGeometry(QRect(WIDTH/2-labelWidth/2, HEIGHT/2-labelHeight/2+(labelHeight*1)+22, labelWidth, labelHeight));
    restart->setVisible(false);
    restart->setStyleSheet("background-color:#148F77; color:#FAFAFA; border:none");
    restart->setFont(optionsFont);
    QObject::connect(restart, SIGNAL(released()), parent, SLOT(restartLevel()));

    quit = new QPushButton("QUIT", parent);
    quit->setGeometry(QRect(WIDTH/2-labelWidth/2, HEIGHT/2-labelHeight/2+(labelHeight*2)+26, labelWidth, labelHeight));
    quit->setVisible(false);
    quit->setStyleSheet("background-color:#148F77; color:#FAFAFA; border:none");
    quit->setFont(optionsFont);
    QObject::connect(quit, SIGNAL(released()), this, SLOT(close()));

}

PauseMenu::~PauseMenu()
{
    delete bg;
    delete title;
    delete soundfx;
    delete resume;
    delete restart;
    delete quit;
}

void PauseMenu::updateSoundStatus(bool sfx) {
    QString status;
    if (sfx) {
        status = "SOUND FX: ON";
        soundfx->setStyleSheet("font-family:courier; color:#58D68D; border:none");
    }
    else {
        status = "SOUND FX: OFF";
        soundfx->setStyleSheet("font-family:courier; color:#E74C3C; border:none");
    }
    soundfx->setText(status);
}

void PauseMenu::displayMenu(bool sfx)
{
    updateSoundStatus(sfx);
    bg->setVisible(true);
    title->setVisible(true);
    resume->setVisible(true);
    restart->setVisible(true);
    quit->setVisible(true);
    soundfx->setVisible(true);
}

void PauseMenu::hideMenu()
{
    bg->setVisible(false);
    title->setVisible(false);
    resume->setVisible(false);
    restart->setVisible(false);
    quit->setVisible(false);
    soundfx->setVisible(false);
}

void PauseMenu::close()
{
    exit(0);
}
