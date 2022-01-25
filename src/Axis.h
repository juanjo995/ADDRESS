#ifndef AXIS_H
#define AXIS_H

#include <glad/glad.h>

#include <iostream>

#include <glm/glm.hpp>
#include <vector>

class Axis {
public:

    Axis();
    ~Axis();

    void setIsVisible(bool b);
    void draw();

    bool isVisible;


    GLuint handler;

    GLuint vbo;	// VBO to store our geometry info, // 0 positions
    GLuint ibo;	// IBOs to store our geometry indexes: 0 point cloud, 1 wire frame, 2 triangle mesh


    std::vector<glm::vec3> vertices; // vertices
    std::vector<GLuint> indices; // vertices
};


#endif //AXIS_H
