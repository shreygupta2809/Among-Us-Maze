#include "button.h"

Button::Button(const std::string &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) : Sprite(
        texture, position, size, rotate, color) {
    this->active = true;
}

Button::~Button() {

}

bool Button::playerCollision(Player *player) {
    bool collisionY = this->position.y + this->size.y >= player->position.y and
                      player->position.y + player->size.y >= this->position.y;
    bool collisionX = this->position.x + this->size.x >= player->position.x and
                      player->position.x + player->size.x >= this->position.x;
    return collisionX and collisionY;
}
