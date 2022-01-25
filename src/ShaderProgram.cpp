#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {

	handler = 0;
	linked = false;
	logString = "";

	hasSubroutines = false;
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(handler);	// Free resources
	linked = false;
	logString = "";
}

GLuint
ShaderProgram::createShaderProgram(const char* shaderPath) noexcept (false) {

	// Create shader program an store its ID
	handler = glCreateProgram();
	if (handler == 0) {
		throw std::runtime_error("Failed to create a shader program.\n");
	}

	// Load and compile each shader object
	char path[256];
	strcpy(path, shaderPath);
	strcat(path, "-vert.glsl");
	GLuint vertexShaderObject = compileShader(path, GL_VERTEX_SHADER);
	if (vertexShaderObject == 0) {
		throw std::runtime_error("Failed to compile vertex shader object.\n");
	}

	strcpy(path, shaderPath);
	strcat(path, "-frag.glsl");
	GLuint fragmentShaderObject = compileShader(path, GL_FRAGMENT_SHADER);
	if (fragmentShaderObject == 0) {
		throw std::runtime_error("Failed to compile fragment shader object.\n");
	}

	// Attach shader objects to our shader program
	glAttachShader(handler, vertexShaderObject);
	glAttachShader(handler, fragmentShaderObject);

	// Link the shader program, check and log errors
	glLinkProgram(handler);
	GLint linkSuccess = 0;
	glGetProgramiv(handler, GL_LINK_STATUS, &linkSuccess);
	if (linkSuccess == GL_FALSE) {
		GLint logLen = 0;
		glGetProgramiv(handler, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char* cLogString = new char[logLen];
			GLint written = 0;
			glGetProgramInfoLog(handler, logLen, &written, cLogString);
			logString.assign(cLogString);
			delete [] cLogString;
			std::cout << "Cannot link shader.\n" << logString << std::endl;
		}
		throw std::runtime_error("Failed to link shader program.\n");
	}
	else {
		linked = true;

		// We can free shader objects memory
		glDeleteShader(vertexShaderObject);
		glDeleteShader(fragmentShaderObject);
	}
	return handler;
}

GLuint
ShaderProgram::createGeometryShaderProgram(const char* shaderPath) noexcept (false) {

    // Create shader program an store its ID
    handler = glCreateProgram();
    if (handler == 0) {
        throw std::runtime_error("Failed to create a shader program.\n");
    }

    // Load and compile each shader object
    char path[256];
    strcpy(path, shaderPath);
    strcat(path, "-vert.glsl");
    GLuint vertexShaderObject = compileShader(path, GL_VERTEX_SHADER);
    if (vertexShaderObject == 0) {
        throw std::runtime_error("Failed to compile vertex shader object.\n");
    }

    strcpy(path, shaderPath);
    strcat(path, "-geom.glsl");
    GLuint geometryShaderObject = compileShader(path, GL_GEOMETRY_SHADER);
    if (geometryShaderObject == 0) {
        throw std::runtime_error("Failed to compile fragment shader object.\n");
    }

    strcpy(path, shaderPath);
    strcat(path, "-frag.glsl");
    GLuint fragmentShaderObject = compileShader(path, GL_FRAGMENT_SHADER);
    if (fragmentShaderObject == 0) {
        throw std::runtime_error("Failed to compile fragment shader object.\n");
    }

    // Attach shader objects to our shader program
    glAttachShader(handler, vertexShaderObject);
    glAttachShader(handler, geometryShaderObject);
    glAttachShader(handler, fragmentShaderObject);

    // Link the shader program, check and log errors
    glLinkProgram(handler);
    GLint linkSuccess = 0;
    glGetProgramiv(handler, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLint logLen = 0;
        glGetProgramiv(handler, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char* cLogString = new char[logLen];
            GLint written = 0;
            glGetProgramInfoLog(handler, logLen, &written, cLogString);
            logString.assign(cLogString);
            delete [] cLogString;
            std::cout << "Cannot link shader.\n" << logString << std::endl;
        }
        throw std::runtime_error("Failed to link shader program.\n");
    }
    else {
        linked = true;

        // We can free shader objects memory
        glDeleteShader(vertexShaderObject);
        glDeleteShader(geometryShaderObject);
        glDeleteShader(fragmentShaderObject);
    }
    return handler;
}

GLuint
ShaderProgram::getHandler() {
    return handler;
}

bool
ShaderProgram::use() {

	// Before activating a shader we should check if its properly created and linked
	if ((handler > 0) && (linked)) {
		glUseProgram(handler);
		return true;
	}
	else {
		std::cout << "Cannot use shader program.\n";
		return false;
	}
}

void
ShaderProgram::setUniform(std::string name, GLint value) {
	GLint location = glGetUniformLocation(handler, name.c_str());
	if (location >= 0) {
		glUniform1i(location, value);
	}
	else {
		std::cout << "Cannot find location for Uniform: " << name << std::endl;
		throw std::runtime_error("Failed while setting an Uniform.\n");
	}
}

void
ShaderProgram::setUniform(std::string name, GLfloat value) {
	GLint location = glGetUniformLocation(handler, name.c_str());
	if (location >= 0) {
		glUniform1f(location, value);
	}
	else {
		std::cout << "Cannot find location for Uniform: " << name << std::endl;
		throw std::runtime_error("Failed while setting an Uniform.\n");
	}
}

void
ShaderProgram::setUniform(std::string name, glm::vec2 value) {
    GLint location = glGetUniformLocation(handler, name.c_str());
    if (location >= 0) {
        glUniform2fv(location, 1, &value[0]);
    }
    else {
        std::cout << "Cannot find location for Uniform: " << name << std::endl;
        throw std::runtime_error("Failed while setting an Uniform.\n");
    }
}

void
ShaderProgram::setUniform(std::string name, glm::vec3 value) {
	GLint location = glGetUniformLocation(handler, name.c_str());
	if (location >= 0) {
		glUniform3fv(location, 1, &value[0]);
	}
	else {
		std::cout << "Cannot find location for Uniform: " << name << std::endl;
		throw std::runtime_error("Failed while setting an Uniform.\n");
	}
}

void
ShaderProgram::setUniform(std::string name, glm::vec4 value) {
    GLint location = glGetUniformLocation(handler, name.c_str());
    if (location >= 0) {
        glUniform4fv(location, 1, &value[0]);
    }
    else {
        std::cout << "Cannot find location for Uniform: " << name << std::endl;
        throw std::runtime_error("Failed while setting an Uniform.\n");
    }
}

void
ShaderProgram::setUniform(std::string name, glm::mat4 value) {
	GLint location = glGetUniformLocation(handler, name.c_str());
	if (location >= 0) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}
	else {
		std::cout << "Cannot find location for Uniform: " << name << std::endl;
		throw std::runtime_error("Failed while setting an Uniform.\n");
	}
}

void
ShaderProgram::setUniform(std::string name, GLint unit, GLuint texture) {
    GLint location = glGetUniformLocation(handler, name.c_str());
    if (location >= 0) {
        glUniform1i(location, unit);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    else {
        std::cout << "Cannot find localization for Uniform: " << name << std::endl;
        throw std::runtime_error("Failed while setting an Uniform.\n");
    }
}

GLuint
ShaderProgram::compileShader(const char* filename, GLenum shaderType) noexcept (false) {

	// Check if the given file actually exists
	if (!existsFile(filename)) {
		fprintf(stderr, "Shader source file %s not found.\n", filename);
		throw std::runtime_error("Tried to read a non existing file.\n");
	}

	// Read file content to a string
	std::ifstream shaderSourceFile;
	shaderSourceFile.open(filename);
	if (!shaderSourceFile) {
		fprintf(stderr, "Cannot open shader source file.\n");
		throw std::runtime_error("Failed to open shader source file.\n");
	}
	std::stringstream shaderSourceStream;
	shaderSourceStream << shaderSourceFile.rdbuf();
	std::string shaderSourceString = shaderSourceStream.str();
	shaderSourceFile.close();

	// Create a shader object for the loaded file
	GLuint shaderHandler = glCreateShader(shaderType);
	if (shaderHandler == 0) {
		fprintf(stderr, "Cannot create shader object.\n");
		throw std::runtime_error("Failed to create a shader object.\n");
	}

	// Compile our source code
	const char* shaderSourceCString = shaderSourceString.c_str();
	glShaderSource(shaderHandler, 1, &shaderSourceCString, nullptr);
	glCompileShader(shaderHandler);

	// Check and log errors
	GLint compileResult;
	glGetShaderiv(shaderHandler, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		GLint logLen = 0;
		logString = "";
		glGetShaderiv(shaderHandler, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char* cLogString = new char[logLen];
			GLint written = 0;
			glGetShaderInfoLog(shaderHandler, logLen, &written, cLogString);
			logString.assign(cLogString);
			delete [] cLogString;
			std::cout << "Cannot compile shader " << shaderType << std::endl
				  << logString << std::endl;
			throw std::runtime_error("Failed to compile a shader.\n");
		}
	}
	return shaderHandler;
}

bool
ShaderProgram::existsFile(const std::string& fileName) {
	struct stat info;
	int ret = -1;
	ret = stat(fileName.c_str(), &info);
	return 0 == ret;
}

void
ShaderProgram::setSoubroutinesIndexes() {
    selectLightFuncUniformLocation = glGetSubroutineUniformLocation(handler, GL_FRAGMENT_SHADER, "selectLightFuncUniform");
    selectColorFuncUniformLocation = glGetSubroutineUniformLocation(handler, GL_FRAGMENT_SHADER, "selectColorFuncUniform");
    hasSubroutines = true;
}

void
ShaderProgram::applySubroutines(GLuint lightFuncIndex, GLuint colorFuncIndex) {
    if(hasSubroutines) {
        GLuint subroutines[2];
        subroutines[selectLightFuncUniformLocation] = lightFuncIndex;
        subroutines[selectColorFuncUniformLocation] = colorFuncIndex;

        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines);
    }
}

void
ShaderProgram::setStarSoubroutinesIndexes() {
    selectSizeFuncUniformLocation = glGetSubroutineUniformLocation(handler, GL_VERTEX_SHADER, "selectSizeFuncUniform");
    selectAlfaFuncUniformLocation = glGetSubroutineUniformLocation(handler, GL_VERTEX_SHADER, "selectAlfaFuncUniform");

    selectHaloFuncUniformLocation = glGetSubroutineUniformLocation(handler, GL_FRAGMENT_SHADER, "selectHaloFuncUniform");

    hasSubroutines = true;
}

void
ShaderProgram::applyStarSubroutines(GLuint SizeFuncIndex, GLuint AlfaFuncIndex, GLuint HaloFuncIndex) {
    /*
    if(hasSubroutines) {
        GLuint subroutines[1];
        subroutines[selectMagFuncUniformLocation] = MagFuncIndex;

        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, subroutines);
    }
    */
    if(hasSubroutines) {
        GLuint subroutines[2];
        subroutines[selectSizeFuncUniformLocation] = SizeFuncIndex;
        subroutines[selectAlfaFuncUniformLocation] = AlfaFuncIndex;

        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 2, subroutines);

        GLuint subroutinesFrag[1];
        subroutinesFrag[selectHaloFuncUniformLocation] = HaloFuncIndex;
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, subroutinesFrag);
    }
}

void
ShaderProgram::setMouseClickSoubroutinesIndexes() {
    selectSizeFuncUniformLocation = glGetSubroutineUniformLocation(handler, GL_VERTEX_SHADER, "selectSizeFuncUniform");
    hasSubroutines = true;
}

void
ShaderProgram::applyMouseClickSubroutines(GLuint SizeFuncIndex) {
    if(hasSubroutines) {
        GLuint subroutines[1];
        subroutines[selectSizeFuncUniformLocation] = SizeFuncIndex;

        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, subroutines);
    }
}

void
ShaderProgram::setVisualizationSoubroutinesIndexes() {
    selectAlfaFuncUniformLocation = glGetSubroutineUniformLocation(handler, GL_VERTEX_SHADER, "selectAlfaFuncUniform");
    hasSubroutines = true;
}

void
ShaderProgram::applyVisualizationSubroutines(GLuint VisualizationFuncIndex) {
    if(hasSubroutines) {
        GLuint subroutines[1];
        subroutines[selectAlfaFuncUniformLocation] = VisualizationFuncIndex;

        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, subroutines);
    }
}