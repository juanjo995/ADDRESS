#include "Grid.h"

Grid::Grid(bool type, glm::mat4 m, fundamentalPlane mPlane) {

    isEQ = type;

    isVisible = false;

    inversePlaneMatrix = m;

    plane = mPlane;

    float distance = 15.0f;
    float x, y, z, ra, dec;
    vertices.clear();
    indices.clear();

    unsigned int currentIndex = 0;

    // Generate EQGrid
    if(isEQ) {
        // Generate paralel lines
        for(int i = 0; i < 17; ++i) {

            dec = glm::radians(float(i - 8) * 10.0f);

            for(unsigned int j = 0; j < 100; ++j) {
                ra = glm::radians(3.6 * float(j));
                x = glm::cos(ra) * glm::cos(dec);
                y = glm::sin(dec);
                z = glm::sin(ra) * glm::cos(dec);
                vertices.push_back(glm::vec3(x, y, z));

                indices.push_back(currentIndex);
                currentIndex++;
            }
            indices.push_back(std::numeric_limits<GLuint>::max());

        }

        // Generate meridians lines
        for(int i = 0; i < 12; ++i) {

            ra = glm::radians(float(i * 15));

            for(unsigned int j = 0; j < 100; ++j) {
                dec = glm::radians(3.6 * float(j));
                x = glm::cos(ra) * glm::cos(dec);
                y = glm::sin(dec);
                z = glm::sin(ra) * glm::cos(dec);
                vertices.push_back(glm::vec3(x, y, z));

                indices.push_back(currentIndex);
                currentIndex++;
            }
            indices.push_back(std::numeric_limits<GLuint>::max());

        }
    } else { // Generate Horizon Grid
        // Generate paralel lines
        for(int i = 0; i < 17; ++i) {

            dec = glm::radians(float(i - 8) * 10.0f);

            for(unsigned int j = 0; j < 100; ++j) {
                ra = glm::radians(3.6 * float(j));
                x = glm::cos(ra) * glm::cos(dec);
                y = glm::sin(dec);
                z = glm::sin(ra) * glm::cos(dec);
                vertices.push_back(glm::vec3(x, y, z));

                indices.push_back(currentIndex);
                currentIndex++;
            }
            indices.push_back(std::numeric_limits<GLuint>::max());

        }

        // Generate meridians lines
        for(int i = 0; i < 18; ++i) {

            ra = glm::radians(float(i * 10));

            for(unsigned int j = 0; j < 100; ++j) {
                dec = glm::radians(3.6 * float(j));
                x = glm::cos(ra) * glm::cos(dec);
                y = glm::sin(dec);
                z = glm::sin(ra) * glm::cos(dec);
                vertices.push_back(glm::vec3(x, y, z));

                indices.push_back(currentIndex);
                currentIndex++;
            }
            indices.push_back(std::numeric_limits<GLuint>::max());

        }

        for(unsigned int i = 0; i < vertices.size(); ++i) {
            vertices[i] = /*inversePlaneMatrix * */glm::vec4(vertices[i], 1.0f);
        }

    }

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

    if(isEQ) {
        // Generate points to show numbers on grid
        for(int i = -9; i < 10; ++i) {
            // Declination points
            gridPoints.push_back(gridPoint(std::to_string(int(i * 10)) + "º", 0.0f, glm::radians(float(i * 10.0f)), false));
        }
        for(int i = 0; i < 24; ++i) {
            // Ascension points
            gridPoints.push_back(gridPoint(std::to_string(i) + "h", glm::radians(float(i * 15.0f)),0.0f, true));
        }
    } else {
        // Generate points to show numbers on grid
        for(int i = -9; i < 10; ++i) {
            // Declination points
            gridPoints.push_back(gridPoint(std::to_string(int(i * 10)) + "º", 0.0f, glm::radians(float(i * 10.0f)), false));
        }
        if(plane == HORIZON_VIEW) {
            for(int i = 0; i < 36; ++i) {
                // Ascension points
                gridPoints.push_back(gridPoint(std::to_string(i * 10) + "º", -glm::radians(float(i * 10.0f)),0.0f, true));
            }
        } else {
            for(int i = 0; i < 36; ++i) {
                // Ascension points
                gridPoints.push_back(gridPoint(std::to_string(i * 10) + "º", glm::radians(float(i * 10.0f)),0.0f, true));
            }
        }

    }

}

void
Grid::computeGridPointsCoords(  float alfa,
                                float delta,
                                fundamentalPlane fPlane,
                                glm::mat4 matrix,
                                glm::mat4 viewMatrix,
                                glm::mat4 projectionMatrix,
                                int screenWidth,
                                int screenHeight) {

    bool ninetyDegrees = false;
    float origAlfa = alfa;

    glm::mat4 m = glm::mat4(1.0f);

    if(plane != fPlane) {
        m = glm::inverse(inversePlaneMatrix) * glm::inverse(matrix);
    } else {
        m = glm::mat4(1.0f);

        if(glm::abs(delta) == glm::half_pi<float>()) {
            ninetyDegrees = true;
        }

    }

    glm::vec3 point = m * glm::vec4(equatorialToXYZ(-alfa, delta), 1.0f);

        point = glm::normalize(point);
        glm::vec3 projectedPoint;
        projectedPoint.x = point.x;
        projectedPoint.y = 0.0f;
        projectedPoint.z = point.z;
        projectedPoint = glm::normalize(projectedPoint);

        alfa = glm::angle(projectedPoint, glm::vec3(1.0f, 0.0f, 0.0f));
        if(point.z > 0.0f) alfa = glm::two_pi<float>() - alfa;
        delta = glm::angle(point, projectedPoint);
        if(point.y < 0.0f) delta = -delta;

    if(ninetyDegrees) {
        alfa = origAlfa;
    }

    for(int i = 0; i < gridPoints.size(); ++i) {

        if(gridPoints[i].isAlfa) {
            gridPoints[i].delta = delta;
        } else { //is delta
            gridPoints[i].alfa = alfa;
        }

        glm::vec3 projection;
        projection = glm::inverse(m) * glm::vec4(gridPoints[i].get3DPosition(), 1.0f);

        projection = glm::project(projection, viewMatrix, projectionMatrix, glm::vec4(0, 0, screenWidth, screenHeight));
        gridPoints[i].proj.x = projection.x;
        gridPoints[i].proj.y = projection.y;
        gridPoints[i].visible = projection.z < 1.0f && projection.x > 0.0f && projection.x < screenWidth && projection.y > 0.0f && projection.y < screenHeight;
    }
}

Grid::~Grid() {
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &handler);
}

void
Grid::setIsVisible(bool b) {
    isVisible = b;
}

void
Grid::draw() {
    glBindVertexArray(handler);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);


    glDrawElements(GL_LINE_LOOP, indices.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void
Grid::updateMatrix(glm::mat4 m) {
    inversePlaneMatrix = m;
}