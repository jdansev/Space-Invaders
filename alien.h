#ifndef ALIEN_H
#define ALIEN_H
#pragma once
#include "alienbase.h"
#include "base.h"
#include "bullet.h"
#include "bulletbuilder.h"
namespace game {
class Alien : public AlienBase {
public:
    Alien(QPixmap image, int x, int y, int velocity, int score, QString baseType);
    virtual ~Alien() {}

    // inherited moves
    virtual void move(QString direction);
    QList<Bullet*> shoot(QString type);  // returns bullet to swarm
    virtual QList<Bullet*> react();

    void toggleSound(bool status);

    // other (GETTERS)
    int get_score() const;
    QList<AlienBase*> getAliens() const;
    bool add(AlienBase* toAdd);
    void remove(AlienBase* toDelete);
    virtual void paint(QPainter& painter);

protected:
    int velocity;
    int score;
    BulletBuilder builder;
};
}

#endif  // ALIEN_H