#include "obstacle.h"

Obstacle::Obstacle(const std::string &texture, glm::vec2 position, glm::vec2 size, const std::string type, float rotate, glm::vec3 color)
        : Sprite(texture, position, size, rotate, color) {
    this->active = true;
    this->type = type;
}

Obstacle::~Obstacle() {

}

bool Obstacle::playerCollision(Player *player) {
    bool collisionY = this->position.y + this->size.y >= player->position.y and
                      player->position.y + player->size.y >= this->position.y;
    bool collisionX = this->position.x + this->size.x >= player->position.x and
                      player->position.x + player->size.x >= this->position.x;
    return collisionX and collisionY;
}
