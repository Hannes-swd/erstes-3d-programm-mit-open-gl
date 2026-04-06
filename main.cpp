#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "input.h"
GLFWwindow* window;

const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D myTexture;

void main() {
    FragColor = texture(myTexture, TexCoord);
}
)";

unsigned int createShaderProgram() {
    unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertShader);

    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragShader);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return program;
}

float vertices[] = {
    // Hinten
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

    // Vorne
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    // Links
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

    // Rechts
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

     // Unten
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

     // Oben
     -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
};

unsigned int indices[] = {
     0,  1,  2,   0,  2,  3,  // Hinten
     4,  5,  6,   4,  6,  7,  // Vorne
     8,  9, 10,   8, 10, 11,  // Links
    12, 13, 14,  12, 14, 15,  // Rechts
    16, 17, 18,  16, 18, 19,  // Unten
    20, 21, 22,  20, 22, 23,  // Oben
};

int main() {
    // Würfel-Rotation
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotationSpeed = 0.01f;

    // Kamera-Position
    float kammerapositionX = 0;
    float kammerapositionY = 0;
    float kammerapositionZ = 3.0f;
    float targetX = 0.0f;
    float targetY = 0.0f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "3D Wuerfel", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    unsigned int shaderProgram = createShaderProgram();

    // Textur laden
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("stein.png", &width, &height, &channels, 0);

    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "stein.png geladen! (" << width << "x" << height << ")" << std::endl;
    }
    else {
        std::cout << "FEHLER: stein.png nicht gefunden!" << std::endl;
    }
    stbi_image_free(data);

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window)) {

        // ===== KAMERA-STEUERUNG mit WASD und QE =====
        if (IsKeyDown(Key_W)) { kammerapositionY += 0.001f; targetY += 0.001f; }
        if (IsKeyDown(Key_S)) { kammerapositionY -= 0.001f; targetY -= 0.001f; }
        if (IsKeyDown(Key_A)) { kammerapositionX -= 0.001f; targetX -= 0.001f; }
        if (IsKeyDown(Key_D)) { kammerapositionX += 0.001f; targetX += 0.001f; }
        if (IsKeyDown(Key_Q)) kammerapositionZ += 0.001f;
        if (IsKeyDown(Key_E)) kammerapositionZ -= 0.001f;

        // ===== WÜRFEL-DREHUNG mit PFEILTASTEN =====
        if (IsKeyDown(Key_Up))    rotationX -= rotationSpeed;
        if (IsKeyDown(Key_Down))  rotationX += rotationSpeed;
        if (IsKeyDown(Key_Left))  rotationY -= rotationSpeed;
        if (IsKeyDown(Key_Right)) rotationY += rotationSpeed;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Model-Matrix mit Würfel-Rotation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

        // View-Matrix mit Kamera-Position
        glm::mat4 view = glm::lookAt(
            glm::vec3(kammerapositionX, kammerapositionY, kammerapositionZ),
            glm::vec3(targetX, targetY, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        // Projektions-Matrix
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)fbWidth / (float)fbHeight,
            0.1f,
            100.0f
        );

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}