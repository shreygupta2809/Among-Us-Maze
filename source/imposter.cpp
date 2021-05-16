#include "imposter.h"
#include "resource_manager.h"
#include <GLFW/glfw3.h>
#include <iostream>

Imposter::Imposter(const std::string &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
        : Sprite(texture, position, size, rotate, color) {
    this->active = true;
    this->target = position + size * 0.5f;
    this->direction = "l";
    this->frames = 0;
}


Imposter::~Imposter() {

}

void Imposter::move(float velocity, unsigned int width, unsigned int height, const std::string &direction) {
    if (direction == "left") {
        this->direction = "l";
        if (this->position.x >= 0.0f) {
            this->position.x -= velocity;
            updateSprite();
        }
    }
    if (direction == "right") {
        this->direction = "r";
        if (this->position.x <= width - this->size.x) {
            this->position.x += velocity;
            updateSprite();
        }
    }
    if (direction == "up") {
        if (this->position.y >= 20.0f) {
            this->position.y -= velocity;
            updateSprite();
        }
    }
    if (direction == "down") {
        if (this->position.y <= height - this->size.y - 30) {
            this->position.y += velocity;
            updateSprite();
        }
    }
}

void Imposter::updateSprite() {
    if(this->frames <= 0) {
        this->frames = 3;
        int index;
        if ((this->direction == "l" and
             this->textureString[8] == 'r') or (this->direction == "r" and this->textureString[8] == 'l'))
            index = 1;
        else
            index = (std::stoi(this->textureString.substr(this->textureString.length() - 2)) + 1) % 24 + 1;
        if (index < 10)
            this->textureString = "impostor" + this->direction + "0" + std::to_string(index);
        else
            this->textureString = "impostor" + this->direction + std::to_string(index);
        this->texture = ResourceManager::GetTexture(this->textureString);
    } else {
        this->frames--;
    }
}

int Imposter::wallCollisions(std::vector<Wall *> walls, float velocity, const std::string &direction) {
    for (auto &wall : walls) {
        if (wall->isWall) {
            if (direction == "left") {
                bool collisionX = wall->position.x + wall->size.x + velocity >= this->position.x and
                                  this->position.x > wall->position.x + wall->size.x;
                bool collisionY = this->position.y + this->size.y >= wall->position.y and
                                  wall->position.y + wall->size.y >= this->position.y;
                if (collisionX and collisionY)
                    return 1;
            }
            if (direction == "right") {
                bool collisionX = this->position.x + this->size.x + velocity >= wall->position.x and
                                  this->position.x + this->size.x < wall->position.x;
                bool collisionY = this->position.y + this->size.y >= wall->position.y and
                                  wall->position.y + wall->size.y >= this->position.y;
                if (collisionX and collisionY)
                    return 1;
            }
            if (direction == "up") {
                bool collisionY = wall->position.y + wall->size.y + velocity >= this->position.y and
                                  this->position.y > wall->position.y + wall->size.y;
                bool collisionX = this->position.x + this->size.x >= wall->position.x and
                                  wall->position.x + wall->size.x >= this->position.x;
                if (collisionX and collisionY)
                    return 1;
            }
            if (direction == "down") {
                bool collisionY = this->position.y + this->size.y + velocity >= wall->position.y and
                                  this->position.y + this->size.y < wall->position.y;
                bool collisionX = this->position.x + this->size.x >= wall->position.x and
                                  wall->position.x + wall->size.x >= this->position.x;
                if (collisionX and collisionY)
                    return 1;
            }
        }
    }
    return 0;
}

bool Imposter::playerCollision(Player *player) {
    bool collisionY = this->position.y + this->size.y >= player->position.y and
                      player->position.y + player->size.y >= this->position.y;
    bool collisionX = this->position.x + this->size.x >= player->position.x and
                      player->position.x + player->size.x >= this->position.x;
    return collisionX and collisionY;
}
