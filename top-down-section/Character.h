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
    Rectangle getWeaponCollisionRect() { return weaponCollisionRect; }

private:
    int windowWidth{};
    int windowHeight{};
    Texture2D weapon{LoadTexture("characters/weapon_sword.png")};
    Rectangle weaponCollisionRect;
};

#endif