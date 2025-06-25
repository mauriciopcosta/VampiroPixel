// Maurício Pereira da Costa
// RA: 1944601

#include "gl_utils.h"
#include "CharacterController.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Variáveis globais da janela
int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow *g_window = NULL;

int main() {
    start_gl();
    
    // Habilita a transparência (canal alfa)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Inverte a imagem na vertical ao carregar, para corrigir a orientação.
    stbi_set_flip_vertically_on_load(true);

    // Cria o personagem: imagem, 4 animações (linhas), 6 frames (colunas).
    CharacterController player("vampiro.png", 4, 6);
    
    double last_time = glfwGetTime();

    // Loop Principal do Jogo
    while (!glfwWindowShouldClose(g_window)) {
        // Cálculo do tempo delta para movimento consistente
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        _update_fps_counter(g_window);
        
        if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(g_window, 1);
        }

        // Processa o input do jogador
        player.handleInput(g_window);
        
        // --- Renderização ---
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, g_gl_width, g_gl_height);

        player.update(delta_time); 
        player.draw();

        // Processa eventos e troca os buffers da tela
        glfwPollEvents();
        glfwSwapBuffers(g_window);
    }

    glfwTerminate();
    return 0;
}