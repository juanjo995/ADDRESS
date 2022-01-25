#ifndef STAR_H
#define STAR_H


#include "ShaderProgram.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

class Star {

public:

    Star();
    Star(double r, double d, double di, double absMa, glm::vec3 c, float ci, float pmr, float pmd, float rv, bool computeVelMode);

    bool operator==(const Star& rhs);

    glm::vec3 compute3DVelocity(float pmr, float pmd, float rv);

    glm::vec3 getPolarFromCartesian(glm::vec3 pos);

    glm::vec2 getPosition();
    glm::vec3 get3DPosition();
    glm::vec3 get3DPosition(float time);
    glm::vec3 get3DVelocity();
    float getDistance();
    float getAbsMag();
    float getAppMag();
    float getColorIndex();
    float getTemperature();
    float getNormalizedMag();
    float getRadius(float potencia, float factor);

    float getParallax();

    float computeApMag();
    float computeApMag(float d);

    glm::vec3 getVelocityParams();
    std::string getInfoLine();
    std::string getInfoText();
    std::string getCSVLine();

    unsigned int id;        // 0
    glm::vec2 position;     // 1
    float distance;         // 2
    glm::vec3 position3D;   // 3
    glm::vec3 velocity;     // 4
    float absMag;           // 5
    float colorIndex;       // 6
    glm::vec3 color;        // 7
    float speedModule;      // 8


private:
    unsigned int getDotsPos(std::string s);
    std::string addBlankSpaces(std::string s, unsigned int n);

};


#endif //STAR_H
