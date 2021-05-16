#ifndef IMPOSTER_H
#define IMPOSTER_H

#include <vector>
#include "sprite.h"
#include "player.h"
#include "wall.h"

class Imposter : public Sprite {
public:
    bool active;
    glm::vec2 target;
    std::string direction;
    int frames;

    Imposter(const std::string &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);

    ~Imposter();

    void move(float velocity, unsigned int width, unsigned int height, const std::string &direction);

    int wallCollisions(std::vector<Wall *> walls, float velocity, const std::string &direction);

    bool playerCollision(Player *player);

    void updateSprite();
};

#endif //IMPOSTER_H
