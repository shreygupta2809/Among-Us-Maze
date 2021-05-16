#include "sprite.h"
#include "resource_manager.h"

Sprite::Sprite(const std::string &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    this->position = position;
    this->size = size;
    this->rotate = rotate;
    this->color = color;
    this->textureString = texture;
    this->texture = ResourceManager::GetTexture(texture);
    this->position = position;
    this->visible = false;
}

Sprite::~Sprite() {
}