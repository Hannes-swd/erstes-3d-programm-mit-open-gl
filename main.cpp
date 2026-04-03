#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "input.h"
GLFWwindow* window;

// -------------------------------------------------------
// SHADER QUELLCODE
// -------------------------------------------------------

// Vertex Shader: läuft für jeden Eckpunkt
// Berechnet die finale Position mit Model/View/Projection
const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;   // Position des Vertex
layout (location = 1) in vec3 aColor; // Farbe des Vertex

out vec3 vertexColor; // wird an Fragment Shader weitergegeben

uniform mat4 model;      // Objekt-Transformation
uniform mat4 view;       // Kamera
uniform mat4 projection; // Perspektive

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

// Fragment Shader: läuft für jeden Pixel
// Bestimmt die finale Farbe
const char* fragmentShaderSrc = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

// -------------------------------------------------------
// SHADER KOMPILIEREN & LINKEN
// -------------------------------------------------------
unsigned int createShaderProgram() {
    // Vertex Shader kompilieren
    unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertShader);

    // Fragment Shader kompilieren
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragShader);

    // Beide zu einem Programm linken
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return program;
}

// -------------------------------------------------------
// WÜRFEL DATEN
// 8 Ecken, jede mit Position (xyz) + Farbe (rgb)
// -------------------------------------------------------
float vertices[] = {
    //   X      Y      Z       R     G     B
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // 0 hinten-unten-links  (rot)
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // 1 hinten-unten-rechts (grün)
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // 2 hinten-oben-rechts  (blau)
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // 3 hinten-oben-links   (gelb)
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // 4 vorne-unten-links   (lila)
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // 5 vorne-unten-rechts  (cyan)
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 6 vorne-oben-rechts   (weiß)
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, // 7 vorne-oben-links    (grau)
};

// Index-Buffer: welche 3 Ecken bilden ein Dreieck?
// Ein Würfel = 6 Seiten × 2 Dreiecke = 12 Dreiecke
unsigned int indices[] = {
    0,1,2, 2,3,0, // hinten
    4,5,6, 6,7,4, // vorne
    0,4,7, 7,3,0, // links
    1,5,6, 6,2,1, // rechts
    3,2,6, 6,7,3, // oben
    0,1,5, 5,4,0, // unten
};

int main() {
    //variablöen

    float kammerapositionX = 0;
    float kammerapositionY = 0;
    float kammerapositionZ = 3.0f;
    float targetX = 0.0f;
    float targetY = 0.0f;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "3D Würfel", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Tiefentest aktivieren – wichtig für 3D!
    // Ohne das werden hintere Flächen VOR vorderen gezeichnet
    glEnable(GL_DEPTH_TEST);

    unsigned int shaderProgram = createShaderProgram();

    // VAO, VBO, EBO erstellen
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex-Daten hochladen
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Index-Daten hochladen
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Layout: Attribut 0 = Position (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Layout: Attribut 1 = Farbe (3 floats, nach den ersten 3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window)) {



        if (IsKeyDown(Key_W)) { kammerapositionY += 0.001f; targetY += 0.001f; }
        if (IsKeyDown(Key_S)) { kammerapositionY -= 0.001f; targetY -= 0.001f; }
        if (IsKeyDown(Key_A)) { kammerapositionX -= 0.001f; targetX -= 0.001f; }
        if (IsKeyDown(Key_D)) { kammerapositionX += 0.001f; targetX += 0.001f; }
        if (IsKeyDown(Key_Q)) kammerapositionZ += 0.001f;
        if (IsKeyDown(Key_E)) kammerapositionZ -= 0.001f;



        // WICHTIG: Beide Buffer leeren (Farbe + Tiefe)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // --- Model Matrix: Würfel rotieren ---
        // glfwGetTime() gibt Sekunden seit Start zurück
        float angle = (float)glfwGetTime() * 50.0f; // 50 Grad pro Sekunde
        glm::mat4 model = glm::mat4(1.0f); // Einheitsmatrix als Start
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.0f));

        // --- View Matrix: Kamera 3 Einheiten zurück ---
        glm::mat4 view = glm::lookAt(
            glm::vec3(kammerapositionX, kammerapositionY, kammerapositionZ),
            glm::vec3(targetX, targetY, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        // --- Projection Matrix: Perspektive ---
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), // Field of View
            800.0f / 600.0f,     // Seitenverhältnis
            0.1f,                // Near-Plane
            100.0f               // Far-Plane
        );

        // Matrizen an Shader übergeben
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Zeichnen mit Index-Buffer
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}