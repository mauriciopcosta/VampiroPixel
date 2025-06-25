#pragma once
#include "Sprite.h"
#include <GLFW/glfw3.h> 

class CharacterController : public Sprite
{
public:
    // O construtor recebe o arquivo, o número de animações (linhas) e frames (colunas)
    CharacterController(const char* filename, int nAnimations, int nFrames);
    
    // Novas funções
    void handleInput(GLFWwindow* window);
    virtual void update(double deltaTime) override; // 'override' é uma boa prática

private:
    float speed;
    glm::vec2 velocity;
};
