#ifndef VAO_H_
#define VAO_H_

#include <glad/glad.h>

#include <iostream>

#include <glm/glm.hpp>
#include <vector>

// Structure to store vertex position and normal together
struct PosNorm {
    glm::vec3 position;
    glm::vec3 normal;

    PosNorm() {}
    PosNorm(glm::vec3 p, glm::vec3 n) {position = p, normal = n;}
};

class VAO {
public:

	enum drawMode{POINT_CLOUD, WIRE_FRAME, TRIANGLE_MESH};

	VAO();

	// Revolution Object part specific constructor
	VAO(std::vector<PosNorm>    PositionsAndNormals,
        std::vector<glm::vec2>  texcoords,
        std::vector<glm::vec3>  tangents,
        std::vector<GLuint>     indicesForPointCloud,
        std::vector<GLuint>     indicesForLineStrip,
        std::vector<GLuint>     indicesForTriangleMesh,
        GLenum drawMode);

	~VAO();

	void addVertex(glm::vec3 position, glm::vec3 normal);
    void addTexCoord(glm::vec2 texCoord);
    void addTangent(glm::vec3 tangent);
	void addIndex(drawMode mode, GLuint index);

	void generate();

	void draw(drawMode mode);

	bool readyToDraw();

	void test();

private:
	GLuint handler;

	GLuint vbo[3];	// VBO to store our geometry info, // 0 positions and normals, 1 texCoords, 2 tangents
	GLuint ibo[3];	// IBOs to store our geometry indexes: 0 point cloud, 1 wire frame, 2 triangle mesh

	std::vector<PosNorm> vertices;	// Vertices and Normals vector
    std::vector<glm::vec2> texCoords; // Texture coordinates
    std::vector<glm::vec3> tangents; // Texture coordinates

	std::vector<GLuint> indexes[3];	// Indexes vectors: 0 point cloud, 1 wire frame, 2 triangle mesh


	bool revolutionObject; // Flag to indicate if the VAO is of a Revolution Object
	GLenum roDrawMode; // Flag to set the revolution object part drawing mode

};

#endif  // VAO_H_
