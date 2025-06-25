#ifndef SPRITE_H
#define SPRITE_H

#include <glad/glad.h>
#include <glm/glm.hpp> // Incluir GLM para vetores

class Sprite {
public:
    Sprite();
    // O construtor agora pode inicializar os dados
    Sprite(const char* textureFile, int nAnimations, int nFrames);
    virtual ~Sprite(); // Adicionar um destrutor virtual

    void setup(const char* textureFile, int nAnimations, int nFrames);
    virtual void update(double deltaTime);
    void draw();
    
    // Define qual linha da animação usar
    void setAnimation(int animIndex);

// Mude de 'private' para 'protected' para que CharacterController possa acessar
protected: 
    GLuint shader_programme;
    GLuint VAO, VBO, EBO;
    GLuint texture;
    
    // Unificando a posição em um vetor
    glm::vec2 position; 

    int nAnimations;
    int nFrames;
    int currentFrame;
    int currentAnimation;

    bool isMoving; // Usaremos para saber se a animação deve rodar

    // Controle de tempo da animação
    float animation_speed;
    double last_frame_time;

    // Dimensões de cada frame na textura (0 a 1)
    float frame_width;
    float frame_height;
};

#endif // SPRITE_H
