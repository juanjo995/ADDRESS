#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cstring> // For strcpy() and strcat() functions

#include <glad/glad.h>

#include <glm/glm.hpp>

// This class encapsulates load, compilation, linking and usage of a shader program
class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	// Creates a shader program with the given vertex and frangment sources files paths
	GLuint createShaderProgram(const char* shaderPath) noexcept (false);
    GLuint createGeometryShaderProgram(const char* shaderPath) noexcept (false);

    GLuint getHandler();

	bool use();	// Activates the shader program

	void setUniform(std::string name, GLint value);
	void setUniform(std::string name, GLfloat value);
    void setUniform(std::string name, glm::vec2 value);
	void setUniform(std::string name, glm::vec3 value);
    void setUniform(std::string name, glm::vec4 value);
	void setUniform(std::string name, glm::mat4 value);
    void setUniform(std::string name, GLint unit, GLuint texture);

    void setSoubroutinesIndexes(); // Only for lighting shader
    void applySubroutines(GLuint lightFuncIndex, GLuint colorFuncIndex); // Only for lighting shader

    void setStarSoubroutinesIndexes(); // Only for 3D star shader
    void applyStarSubroutines(GLuint SizeFuncIndex, GLuint AlfaFuncIndex, GLuint HaloFuncIndex); // Only for 3D star shader

    void setMouseClickSoubroutinesIndexes(); // Only for mouse click shader
    void applyMouseClickSubroutines(GLuint SizeFuncIndex);

    void setVisualizationSoubroutinesIndexes(); // Only for 3D star shader
    void applyVisualizationSubroutines(GLuint VisualizationFuncIndex); // Only for 3D star shader

private:

	GLuint handler;		// Shader program ID
	bool linked;		// Flag indicating that the shader program was linked properly
	std::string logString;	// Stores the possible error message produced on any operation

	// Private method to compile each part of our shader
	GLuint compileShader(const char *filename, GLenum shaderType) noexcept (false);

	// Auxiliary method to check if the given file path exists
	bool existsFile(const std::string& fileName);


    // Variables to store subroutines indexes, only used by lighting shader
    GLint selectLightFuncUniformLocation;
    GLint selectColorFuncUniformLocation;

    // Variables to store subroutines indexes, only used by 3D STAR SHADER
    GLint selectSizeFuncUniformLocation;
    GLint selectAlfaFuncUniformLocation;
    GLint selectHaloFuncUniformLocation;

    bool hasSubroutines;
};

#endif  // SHADERPROGRAM_H_
