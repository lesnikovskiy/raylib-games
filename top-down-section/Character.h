#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "BaseCharacter.h"

class Character : public BaseCharacter
{
public:
    Character(int winWidth, int winHeight);
    virtual Vector2 getScreenPos() override;
    virtual void tick(float deltaTime) override;

private:
    int windowWidth{};
    int windowHeight{};
};

#endif