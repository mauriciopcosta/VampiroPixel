// Maurício Pereira da Costa
// RA: 1944601

#include "CharacterController.h"


#define ANIM_SOUTH 0 
#define ANIM_NORTH 1 
#define ANIM_EAST  2 
#define ANIM_WEST  3 

CharacterController::CharacterController(const char* filename, int nAnimations, int nFrames)
    // Chama o construtor da classe mãe (Sprite)
    : Sprite(filename, nAnimations, nFrames) {
    this->speed = 0.5f; // Velocidade ajustada para coordenadas normalizadas (-1 a 1)
    this->velocity = glm::vec2(0.0f);
}

void CharacterController::handleInput(GLFWwindow* window) {
    velocity = glm::vec2(0.0f);
    isMoving = false; // Começa como parado

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        velocity.y = 1.0f;
        setAnimation(ANIM_NORTH);
        isMoving = true;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        velocity.y = -1.0f;
        setAnimation(ANIM_SOUTH);
        isMoving = true;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        velocity.x = -1.0f;
        setAnimation(ANIM_WEST);
        isMoving = true;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        velocity.x = 1.0f;
        setAnimation(ANIM_EAST);
        isMoving = true;
    }

    // Normaliza o vetor de velocidade para movimento diagonal consistente
    if (glm::length(velocity) > 0.0f) {
        velocity = glm::normalize(velocity);
    }
}

void CharacterController::update(double deltaTime) {
    // Atualiza a posição com base na velocidade, velocidade e tempo
    position += velocity * speed * static_cast<float>(deltaTime);
    
    // Chama o update da classe mãe (Sprite) para cuidar da animação de frames
    Sprite::update(deltaTime);
}