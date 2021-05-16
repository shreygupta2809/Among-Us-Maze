#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "sprite.h"
#include "wall.h"

class Player: public Sprite {
public:
    int health;
    int score;
    std::string direction;
    int frames = 5;
    Player(const std::string &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color,
           int health = 10);
    ~Player();
    void
    move(float velocity, unsigned int width, unsigned int height, bool Keys[1024]);

    int wallCollisions(std::vector<Wall *> walls, float velocity, bool Keys[1024]);

    void updateSprite();
};

#endif //PLAYER_H
