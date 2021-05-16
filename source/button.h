#ifndef BUTTON_H
#define BUTTON_H

#include <vector>
#include "sprite.h"
#include "player.h"
#include "wall.h"

class Button : public Sprite {
public:
    bool active;

    Button(const std::string &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);

    ~Button();

    bool playerCollision(Player *player);
};

#endif //BUTTON_H
