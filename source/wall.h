#ifndef WALL_H
#define WALL_H

#include "sprite.h"

class Wall : public Sprite {
public:
    bool isWall;

    Wall(const std::string &texture, glm::vec2 position, glm::vec2 size, bool isWall = false, float rotation = 0.0f,
         glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

    ~Wall();
};

#endif //WALL_H
