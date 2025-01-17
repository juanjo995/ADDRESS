#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"

Renderer* r;

// Global variables for the callbacks
bool firstTimeCursorMove = true;
int cursorPrevX;
int cursorPrevY;

// Mouse variables
bool leftClick = false;
bool rightClick = false;

bool mouseDraging = false;
double lastTimeLeftClickWasPushed; // To manage click detection

// Refresh callback, called everytime the drawing area must be redrawn
void
window_refresh_callback(GLFWwindow *window) {
	r -> refreshWindow(glfwGetTime());
	glfwSwapBuffers(window);
}

// Called everytime the drawing area changes its size
void
framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	r -> resize(width, height);
}

// Callback to finish aplication by pressing ESC key
void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch(key) {
		    case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
		        break;
            case GLFW_KEY_A:
                r -> moveTruck(false);
                break;
            case GLFW_KEY_D:
                r -> moveTruck(true);
                break;
            case GLFW_KEY_W:
                r -> moveDolly(true);
                break;
            case GLFW_KEY_S:
                r -> moveDolly(false);
                break;
            case GLFW_KEY_UP:
                r -> wheelMove(true);
                break;
            case GLFW_KEY_DOWN:
                r -> wheelMove(false);
                break;
		}
	}

}

// Callback to get mouse clicks
void
mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if(action == GLFW_PRESS) {
		if(button == 0) {
		    leftClick = true;
		    lastTimeLeftClickWasPushed = glfwGetTime();
		}

		if(button == 1) rightClick = true;
	}
	else if(action == GLFW_RELEASE) {
        if(button == 0){
            leftClick = false;

            if(!mouseDraging && !ImGui::IsMouseHoveringAnyWindow()) { // Mouse left click detected
                r -> leftClick(cursorPrevX, cursorPrevY);
            }
            mouseDraging = false;
        }

        if(button == 1) rightClick = false;
	}
}

// Callback to get mouse coords
void cursor_pos_callback(GLFWwindow* window, double x, double y) {
    if (ImGui::IsMouseHoveringAnyWindow()) {

    } else {
        if (firstTimeCursorMove) {
            firstTimeCursorMove = false;
            cursorPrevX = x;
            cursorPrevY = y;
        }

        float sensitivity = 0.0025;
        float orbitSensitivity = 0.005;


        float xOffset = cursorPrevX - x;
        float yOffset = cursorPrevY - y;

        if (leftClick) {
            mouseDraging = true;
            r -> mouseDrag(xOffset, yOffset);
        }
        cursorPrevX = x;
        cursorPrevY = y;
    }
}

// Callback to get mouse wheel movement
void
scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset > 0) {
        r -> wheelMove(true);
    }
    else if (yoffset < 0) {
        r -> wheelMove(false);
    }
}

// Finish GLFW
void
stopGLFW(GLFWwindow* &window) {
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
}

// Inits GLFW and GLEW
void
startGLFW(GLFWwindow* &window) {

	if(glfwInit() != GLFW_TRUE) {
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_SAMPLES,0); // 0 -> disable antialiasing, != 0 destroys performance
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessary for macOS

	window = glfwCreateWindow(1280, 720, "ADDRESS", nullptr, nullptr);

	if(window == nullptr) {
		glfwTerminate();
		throw std::runtime_error("Failed to create Window");
	}

	glfwMakeContextCurrent(window);

    // Init glad.
    //if (!gladLoadGL()) {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        exit(EXIT_FAILURE);
    }

	// Set callbacks
	glfwSetWindowRefreshCallback(window, window_refresh_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);

    // ImGui stuff
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr; // Disable saving/loading settings

    ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = ImVec4(0.42f, 0.67f, 1.0f, 0.6f);
    ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.7f, 1.0f, 1.0f);
    ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.42f, 0.67f, 1.0f, 0.6f);
    ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.7f, 1.0f, 1.0f);
    ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.7f, 1.0f, 1.0f);

}

int main() {

    try {
        GLFWwindow *mainWindow;
        startGLFW(mainWindow);

        r = new Renderer();

        r->prepareOpenGL();
        r->prepareShaders();
        r->prepareScene();

        // Main loop
        while (!glfwWindowShouldClose(mainWindow)) {
            window_refresh_callback(mainWindow);
            glfwPollEvents();
        }

        delete r;

        ImGui_ImplGlfwGL3_Shutdown();
        stopGLFW(mainWindow);

    } catch (const std::runtime_error &e) {
        std::cerr << "error: " << e.what();
        return -1;
    }

    return 0;
}
