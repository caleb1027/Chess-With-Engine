#include "player.h"

Player::Player(bool isWhite, Game* g): isWhite {isWhite}, g {g} {}

Player::~Player() {}

bool Player::getIsWhite() const {
    return isWhite;
}
