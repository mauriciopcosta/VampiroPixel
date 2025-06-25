#include "Sprite.h"
#include "gl_utils.h" // Para carregar shaders
#include "stb_image.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp> // Para passar glm::vec2 para o shader

Sprite::Sprite() : 
    shader_programme(0), texture(0), VAO(0), VBO(0), EBO(0),
    position(0.0f, 0.0f), nAnimations(0), nFrames(0),
    currentFrame(0), currentAnimation(0), isMoving(false),
    animation_speed(0.1f), last_frame_time(0.0),
    frame_width(0.0f), frame_height(0.0f) {}

Sprite::Sprite(const char* textureFile, int nAnimations, int nFrames) : Sprite() {
    setup(textureFile, nAnimations, nFrames);
}

// É uma boa prática ter um destrutor para limpar os buffers da GPU
Sprite::~Sprite() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_programme);
}

void Sprite::setup(const char* textureFile, int numAnimations, int numFrames) {
    this->nAnimations = numAnimations;
    this->nFrames = numFrames;
    this->frame_width = 1.0f / static_cast<float>(nFrames);
    this->frame_height = 1.0f / static_cast<float>(nAnimations);
    
    shader_programme = create_programme_from_files("_sprites_vs.glsl", "_sprites_fs.glsl");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    float scale = 0.2f; 
    float vertices[] = {
        // Posições     // Coordenadas de Textura (com Y invertido)
         scale,  scale,  1.0f, 0.0f, // top right
         scale, -scale,  1.0f, 1.0f, // bottom right
        -scale, -scale,  0.0f, 1.0f, // bottom left
        -scale,  scale,  0.0f, 0.0f  // top left
    };
    
    unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Posição (vec2)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Coordenada de Textura (vec2)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Sprite::setAnimation(int animIndex) {
    if (animIndex < nAnimations) {
        currentAnimation = animIndex;
    }
}

void Sprite::update(double deltaTime) {
    // A animação só avança se o 'isMoving' for verdadeiro
    if (isMoving) {
        double current_time = glfwGetTime();
        if ((current_time - last_frame_time) > animation_speed) {
            last_frame_time = current_time;
            currentFrame = (currentFrame + 1) % nFrames;
        }
    } else {
        // Se estiver parado, fica no primeiro frame da animação
        currentFrame = 0;
    }
}

void Sprite::draw() {
    glUseProgram(shader_programme);
    
    float offsetX = frame_width * static_cast<float>(currentFrame);
    float offsetY = frame_height * static_cast<float>(currentAnimation);

    // Passa os uniformes para o shader
    glUniform2fv(glGetUniformLocation(shader_programme, "pos"), 1, glm::value_ptr(position));
    glUniform1f(glGetUniformLocation(shader_programme, "offsetX"), offsetX);
    glUniform1f(glGetUniformLocation(shader_programme, "offsetY"), offsetY);
    glUniform1f(glGetUniformLocation(shader_programme, "frameW"), frame_width);
    glUniform1f(glGetUniformLocation(shader_programme, "frameH"), frame_height);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
