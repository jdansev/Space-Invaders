#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "config.h"
#include <QLabel>
#include <QPushButton>


class PauseMenu : public QObject
{
    Q_OBJECT
public:
    PauseMenu(QWidget* parent);
    ~PauseMenu();
    void displayMenu(bool);
    void hideMenu();

public slots:
    void close();

private:
    QLabel *bg;
    QLabel *title;
    QPushButton* resume;
    QPushButton* restart;
    QPushButton* quit;
    QPushButton* soundfx;
    void makeButtons(QWidget* parent);
    void updateSoundStatus(bool);
};

#endif // PAUSEMENU_H
