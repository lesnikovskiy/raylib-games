#include "BaseCharacter.h"

BaseCharacter::BaseCharacter()
{
}

Rectangle BaseCharacter::getCollisionRect()
{
    return Rectangle{
        screenPos.x,
        screenPos.y,
        width * scale,
        height * scale};
}

void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame;
}