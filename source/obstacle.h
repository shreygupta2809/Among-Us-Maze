#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>
#include "sprite.h"
#include "player.h"
#include "wall.h"

class Obstacle : public Sprite {
public:
    bool active;
    std::string type;
    Obstacle(const std::string &texture, glm::vec2 position, glm::vec2 size, const std::string type, float rotate, glm::vec3 color);

    ~Obstacle();

    bool playerCollision(Player *player);
};


#endif //OBSTACLE_H
