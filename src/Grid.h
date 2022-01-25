#ifndef GRID_H
#define GRID_H

#include <glad/glad.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>

#include "Utils.h"

class Grid {
public:
    Grid(bool type, glm::mat4 m, fundamentalPlane mPlane);
    void computeGridPointsCoords(float alfa, float delta, fundamentalPlane fPlane, glm::mat4 matrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                                 int screenWidth,
                                 int screenHeight);
    ~Grid();

    void setIsVisible(bool b);
    void draw();

    void updateMatrix(glm::mat4 m);

    bool isVisible;
    bool isEQ; // true for equatorial grid, false for horizon grid

    glm::vec4 color;

    fundamentalPlane plane;
    glm::mat4 inversePlaneMatrix;

    GLuint handler;

    GLuint vbo;	// VBO to store our geometry info, // 0 positions
    GLuint ibo;	// IBOs to store our geometry indexes: 0 point cloud, 1 wire frame, 2 triangle mesh


    std::vector<glm::vec3> vertices; // vertices
    std::vector<GLuint> indices; // vertices

    std::vector<gridPoint> gridPoints; // Points to show ra and dec numbers on the grid

};


#endif //GRID_H
