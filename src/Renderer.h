#ifndef RENDERER_H_
#define RENDERER_H_

#include <iostream>
#include <algorithm>

#include <glad/glad.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

#include "GUI.h"

#include "Camera.h"
#include "ShaderProgram.h"

#include <fstream> // to read stars file

#include <map>
#include <set>

#include "Grid.h"
#include "Axis.h"
#include "Utils.h"

#include "StarCatalog.h"

class Renderer {

public:

    Renderer();
    ~Renderer();

	void prepareOpenGL();

    void prepareShaders();

    void prepareScene();

	void refreshWindow(double currentTime);

	void resize(int width, int height);

    void setViewMode(viewMode v);

    void drawScope();

    void switchGrid();

    void leftClick(int x, int y);

    void movePan(float angle);
    void moveTilt(float angle);

    void moveOrbitY(float angle);
    void moveOrbitU(float angle);

    void moveTruck(float distance);
    void moveDolly(float distance);

    void moveTruck(bool direction); // true forward, false backwards
    void moveDolly(bool direction); // true right, false left

    void moveCrane(float distance);
    void changeZoom(float zoom);

    void adjustTime(double currentTime);

    void computeAltAzText();
    void computeRaDecText();
    void computeFPSText();

    void changeRA(float angle);
    void changeDec(float angle);

    void wheelMove(bool up);
    void mouseDrag(float x, float y);

    void updateFOVChanges();
    void changeFOV(float angle);
    void computeDragFactor();

    glm::mat4 getPlaneMatrix();

    glm::mat4 getTimeMatrix();
    glm::mat4 getEarthPosMatrix();

    glm::vec3 getApropiateCameraPos();

    float getTimeSpeed();

    float getStarRadius(Star* s, float epoch);

    float magToRadius(float mag);
    void modifyMagFilter(float val);

    glm::mat4 getApropiateMatrix();
    void drawStars();

    void switchGaia(); // Switch gaia catalog
    void switchHip(); // Switch gaia catalog

    void drawGrids();
    void updateGridPointsCoords();

    void drawAxis();

    void advanceTime();

    void computeShowingNames();

    void updateSelectedStarScreenPos();

    void changeMode();
    void setSubroutines();
    void setSubroutinesForMouseClickShader(ShaderProgram* sh);

    void resetCameraPosition();

private:

    std::string appDir;

    bool mode;
    Parameters param;
    bool clicked;

    GUI* gui;
    std::vector<StarCatalog*>* catalogs;

    ShaderProgram star3DShader;
    ShaderProgram plot2DShader;
    ShaderProgram diagramHRShader;
    ShaderProgram scopeShader; // To remark selected star
    ShaderProgram spaceVelocityShader;

    ShaderProgram* getApropiateMouseClickShader();

    ShaderProgram mouseClickShader;
    ShaderProgram mouseClick2DShader;
    ShaderProgram mouseClickHRShader;

    ShaderProgram gridShader;
    ShaderProgram axisShader;
    Axis* axis;

    Camera* camera;

    Grid* EQGrid;
    Grid* horizonGrid;
    Grid* galacticGrid;
    Grid* eclipticGrid;
    Grid* galactoCentricGrid;

};

#endif  // RENDERER_H_
