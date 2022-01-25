#include "Axis.h"


Axis::Axis() {

    isVisible = true;


    vertices.clear();
    indices.clear();

    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3(2.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3(0.0f, 2.0f, 0.0f));
    vertices.push_back(glm::vec3(0.0f, 0.0f, 2.0f));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(3);


    // Generate VAO
    glGenVertexArrays(1, &handler);
    if(handler == 0) throw std::runtime_error("Failed to create a VAO.\n");
    glBindVertexArray(handler);

    // Generate VBOs, 0 for Positions and Normals, 1 for texCoords, 2 for tangents
    glGenBuffers(1, &vbo);
    if(vbo == 0) throw std::runtime_error("Failed to create a VBO.\n");
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Specify geometry positions info to GPU
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);


    // Copy positions and normals to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Generate IBOs
    glGenBuffers(1, &ibo);
    if(ibo == 0) throw std::runtime_error("Failed to create a IBO.\n");

    // Point Cloud
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

}

Axis::~Axis() {
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &handler);
}

void
Axis::setIsVisible(bool b) {
    isVisible = b;
}

void
Axis::draw() {
    glBindVertexArray(handler);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}