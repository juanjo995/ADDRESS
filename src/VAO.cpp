#include "VAO.h"

VAO::VAO() {
	handler = 0;
    revolutionObject = false;
}

VAO::VAO(std::vector<PosNorm>   PositionsAndNormals,
        std::vector<glm::vec2>  texcoords,
        std::vector<glm::vec3>  tangents,
        std::vector<GLuint>     indicesForPointCloud,
        std::vector<GLuint>     indicesForLineStrip,
        std::vector<GLuint>     indicesForTriangleMesh,
        GLenum drawMode)
{
    this -> vertices = PositionsAndNormals;
    this -> texCoords = texcoords;
    this -> tangents = tangents;

    this -> indexes[0] = indicesForPointCloud;
    this -> indexes[1] = indicesForLineStrip;
    this -> indexes[2] = indicesForTriangleMesh;

    revolutionObject = true;
    roDrawMode = drawMode;
}

void
VAO::test() {
    for(int i = 0; i < vertices.size(); ++i) {
        std::cout << vertices[i].position.x << " " << vertices[i].position.y << " " << vertices[i].position.z << '\n';
    }

    std::cout << '\n';

    for(int i = 0; i < vertices.size(); ++i) {
        std::cout << vertices[i].normal.x << " " << vertices[i].normal.y << " " << vertices[i].normal.z << '\n';
    }

    std::cout << '\n';

    for(int i = 0; i < texCoords.size(); ++i) {
        std::cout << texCoords[i].x << " " << texCoords[i].y << '\n';
    }
}

VAO::~VAO() {
	glDeleteBuffers(2, vbo);
	glDeleteBuffers(3, ibo);
	glDeleteVertexArrays(1, &handler);
}

void
VAO::addVertex(glm::vec3 position, glm::vec3 normal) {
    PosNorm pn;
    pn.position = position;
    pn.normal = normal;
	vertices.push_back(pn);
}

void
VAO::addTexCoord(glm::vec2 texCoord) {
    texCoords.push_back(texCoord);
}

void
VAO::addTangent(glm::vec3 tangent) {
    tangents.push_back(tangent);
}

void
VAO::addIndex(drawMode mode, GLuint index) {
	indexes[mode].push_back(index);
}

void
VAO::generate() {

	// Generate VAO
	glGenVertexArrays(1, &handler);
	if(handler == 0) throw std::runtime_error("Failed to create a VAO.\n");
	glBindVertexArray(handler);

	// Generate VBOs, 0 for Positions and Normals, 1 for texCoords, 2 for tangents
	glGenBuffers(3, vbo);
	if(vbo == 0) throw std::runtime_error("Failed to create a VBO.\n");
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    // Specify geometry positions info to GPU
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(PosNorm), nullptr);

    // Specify geometry normals info to GPU
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(PosNorm), (GLuint*) sizeof(glm::vec3));

    // Copy positions and normals to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(PosNorm) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // TexCoords
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);

    // Tangents
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tangents.size(), tangents.data(), GL_STATIC_DRAW);

    // Generate IBOs
	glGenBuffers(3, ibo);
	if(ibo == 0) throw std::runtime_error("Failed to create a IBO.\n");

	// Point Cloud
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexes[POINT_CLOUD].size(), indexes[POINT_CLOUD].data(), GL_STATIC_DRAW);
	
	// Wire Frame
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexes[WIRE_FRAME].size(), indexes[WIRE_FRAME].data(), GL_STATIC_DRAW);
	
	// Triangle Mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexes[TRIANGLE_MESH].size(), indexes[TRIANGLE_MESH].data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void
VAO::draw(drawMode mode) {

	glBindVertexArray(handler);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[mode]);

	switch (mode) {
		case POINT_CLOUD:
			glDrawElements(GL_POINTS, indexes[mode].size(), GL_UNSIGNED_INT, nullptr);
			break;
		case WIRE_FRAME:
			glDrawElements(GL_LINES, indexes[mode].size(), GL_UNSIGNED_INT, nullptr);
			break;
		case TRIANGLE_MESH:
			//glDrawElements(GL_TRIANGLES, indexes[mode].size(), GL_UNSIGNED_INT, nullptr);
            glDrawElements(revolutionObject ? roDrawMode : GL_TRIANGLES, indexes[mode].size(), GL_UNSIGNED_INT, nullptr);
			break;
	}

	glBindVertexArray(0);
}

bool
VAO::readyToDraw() {
	return handler != 0;
}
