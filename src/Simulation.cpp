#include "Simulation.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Simulation::Simulation() : time_step_(1000.0), sphereVAO_(0), sphereVBO_(0) {
    setupSphere();
}

Simulation::~Simulation() {
    glDeleteVertexArrays(1, &sphereVAO_);
    glDeleteBuffers(1, &sphereVBO_);
}

void Simulation::initializeBodies() {
    bodies_.push_back(Body(Vector(0, 0, 0), Vector(0, 0, 0), 1.0e24));  // Central mass
    bodies_.push_back(Body(Vector(1e11, 0, 0), Vector(0, 30000, 0), 1.0e22)); // Orbiting body
}

void Simulation::setupSphere() {
    int latitudeBands = 16;
    int longitudeBands = 16;
    float radius = 1.0f; // Default radius, scaled later
    std::vector<float> vertices;

    // Generate vertices for the sphere
    for (int lat = 0; lat <= latitudeBands; ++lat) {
        float theta = lat * M_PI / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= longitudeBands; ++lon) {
            float phi = lon * 2 * M_PI / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = radius * cosPhi * sinTheta;
            float y = radius * cosTheta;
            float z = radius * sinPhi * sinTheta;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    // Generate and bind VAO
    glGenVertexArrays(1, &sphereVAO_);
    glBindVertexArray(sphereVAO_);

    // Generate and bind VBO
    glGenBuffers(1, &sphereVBO_);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Simulation::drawSphere() const {
    glBindVertexArray(sphereVAO_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 16 * 16); // Adjust the count based on vertices count
    glBindVertexArray(0);
}

void Simulation::initializeShaders() {
    std::string vertexShaderSource, fragmentShaderSource;
    
    // Load vertex shader source
    std::ifstream vShaderFile("shaders/vertex_shader.glsl");
    std::stringstream vShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    vertexShaderSource = vShaderStream.str();
    
    // Load fragment shader source
    std::ifstream fShaderFile("shaders/fragment_shader.glsl");
    std::stringstream fShaderStream;
    fShaderStream << fShaderFile.rdbuf();
    fragmentShaderSource = fShaderStream.str();

    const char* vShaderCode = vertexShaderSource.c_str();
    const char* fShaderCode = fragmentShaderSource.c_str();

    // Compile vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // Check for vertex shader compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile fragment shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders into a program
    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertex);
    glAttachShader(shaderProgram_, fragment);
    glLinkProgram(shaderProgram_);

    // Check for linking errors
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // Retrieve uniform locations
    modelLoc_ = glGetUniformLocation(shaderProgram_, "model");
    viewLoc_ = glGetUniformLocation(shaderProgram_, "view");
    projectionLoc_ = glGetUniformLocation(shaderProgram_, "projection");
}


void Simulation::update() {
    for (auto& body : bodies_) {
        body.updatePosition(time_step_);
    }
}

void Simulation::render() const {
    glViewport(0, 0, 800, 600);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram_);

    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / 600, 0.1f, 100.0f);

    for (const auto& body : bodies_) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(body.getPosition().x / 1e11,
                                                                    body.getPosition().y / 1e11,
                                                                    body.getPosition().z / 1e11)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(body.getRadius() / 1e11));

        glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc_, 1, GL_FALSE, glm::value_ptr(projection));

        drawSphere();
    }

    glDisable(GL_DEPTH_TEST);
}
