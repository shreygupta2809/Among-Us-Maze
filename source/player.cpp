#include "player.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "resource_manager.h"

Player::Player(const std::string &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color,
               int health) : Sprite(texture, position, size, rotate, color) {
    this->health = health;
    this->score = 0;
    this->direction = "r";
}


Player::~Player() {

}

void Player::move(float velocity, unsigned int width, unsigned int height, bool Keys[1024]) {
    if (Keys[GLFW_KEY_A]) {
        this->direction = "l";
        if (this->position.x >= 0.0f) {
            this->position.x -= velocity;
            updateSprite();
        }
    }
    if (Keys[GLFW_KEY_D]) {
        this->direction = "r";
        if (this->position.x <= width - this->size.x) {
            this->position.x += velocity;
            updateSprite();
        }
    }
    if (Keys[GLFW_KEY_W]) {
        if (this->position.y >= 20.0f) {
            this->position.y -= velocity;
            updateSprite();
        }
    }
    if (Keys[GLFW_KEY_S]) {
        if (this->position.y <= height - this->size.y - 30) {
            this->position.y += velocity;
            updateSprite();
        }
    }
}

void Player::updateSprite() {
    if(this->frames <= 0) {
        this->frames = 5;
        int index;
        if ((this->direction == "l" and
             this->textureString[6] == 'r') or (this->direction == "r" and this->textureString[6] == 'l'))
            index = 1;
        else
            index = (std::stoi(this->textureString.substr(this->textureString.length() - 2)) + 1) % 12 + 1;
        if (index < 10)
            this->textureString = "player" + this->direction + "0" + std::to_string(index);
        else
            this->textureString = "player" + this->direction + std::to_string(index);
        this->texture = ResourceManager::GetTexture(this->textureString);
    } else {
        this->frames--;
    }
}

int Player::wallCollisions(std::vector<Wall *> walls, float velocity, bool Keys[1024]) {
    for (auto &wall : walls) {
        if (wall->isWall) {
            if (Keys[GLFW_KEY_A]) {
                bool collisionX = wall->position.x + wall->size.x + velocity >= this->position.x and
                                  this->position.x > wall->position.x + wall->size.x;
                bool collisionY = this->position.y + this->size.y >= wall->position.y and
                                  wall->position.y + wall->size.y >= this->position.y;
                if (collisionX and collisionY)
                    return 1;
            }
            if (Keys[GLFW_KEY_D]) {
                bool collisionX = this->position.x + this->size.x + velocity >= wall->position.x and
                                  this->position.x + this->size.x < wall->position.x;
                bool collisionY = this->position.y + this->size.y >= wall->position.y and
                                  wall->position.y + wall->size.y >= this->position.y;
                if (collisionX and collisionY)
                    return 1;
            }
            if (Keys[GLFW_KEY_W]) {
                bool collisionY = wall->position.y + wall->size.y + velocity >= this->position.y and
                                  this->position.y > wall->position.y + wall->size.y;
                bool collisionX = this->position.x + this->size.x >= wall->position.x and
                                  wall->position.x + wall->size.x >= this->position.x;
                if (collisionX and collisionY)
                    return 1;
            }
            if (Keys[GLFW_KEY_S]) {
                bool collisionY = this->position.y + this->size.y + velocity >= wall->position.y and
                                  this->position.y + this->size.y < wall->position.y;
                bool collisionX = this->position.x + this->size.x >= wall->position.x and
                                  wall->position.x + wall->size.x >= this->position.x;
                if (collisionX and collisionY)
                    return 1;
            }
        }
//        if (wall->isWall) {
//            bool collisionX = this->position.x + this->size.x + velocity >= wall->position.x and
//                              wall->position.x + wall->size.x + velocity >= this->position.x;
//            bool collisionY = this->position.y + this->size.y + velocity >= wall->position.y and
//                              wall->position.y + wall->size.y + velocity >= this->position.y;
//            std::cout << (collisionX and collisionY) << "\n";
//            return collisionX and collisionY;
//        }
    }
    return 0;
}
