#include "wall.h"

Wall::Wall(const std::string &texture, glm::vec2 position, glm::vec2 size, bool isWall, float rotation, glm::vec3 color)
        : Sprite(texture, position, size, rotation, color) {
    this->isWall = isWall;
}

Wall::~Wall() {

}
