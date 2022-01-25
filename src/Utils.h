#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream>
#include <fstream>

#include <algorithm>

#include <map>

#include "Camera.h"
#include "Star.h"


// FREE CAMERA for WASD movement
enum viewMode{CENTERED_ON_ORIGIN, FREE_CAMERA, ORBIT};

enum renderMode{SPACE_3D, PLOT_2D, HR_DIAGRAM};

enum fundamentalPlane{EQUATORIAL, ECLIPTIC, GALACTIC, GALACTOCENTRIC, HORIZON_VIEW}; // HORIZON VIEW

enum visualizationParameter{SIZE_PARAMS, ALFA_PARAMS, HALO_PARAMS};


struct ColorValue {
    float colorIndex;
    glm::vec3 rgb;

    ColorValue(float ci, glm::vec3 co) {
        colorIndex = ci;
        rgb = co;
    }
};

struct NameText {
    std::string name;
    glm::vec2 pos;

    NameText(std::string n, glm::vec2 p) {
        name = n;
        pos = p;
    }
};

struct starWithProperName {
    std::string name;
    Star star;

    starWithProperName(std::string str, Star s) {
        name = str;
        star = s;
    }
};

struct gridPoint {
    std::string text;
    float alfa; // ra equivalent
    float delta; // dec equivalent

    bool isAlfa; // true is alfa (ascension point), false is delta (declination point)

    glm::vec2 proj;

    bool visible;

    gridPoint(std::string t, float a, float d, bool al) {
        text = t;
        alfa = a;
        delta = d;
        isAlfa = al;
    }

    glm::vec3 get3DPosition() {
        return glm::vec3(glm::cos(-alfa) * glm::cos(delta), glm::sin(delta), glm::sin(-alfa) * glm::cos(delta));
    }
};


struct StarDistanceComparator {
    bool operator()(Star& a, Star& b) const
    {
        return a.getDistance() < b.getDistance();
    }
};

std::vector<std::string> splitLine(std::string line, char c);

float parallaxToDist(float p);

float absMagFromApMag(float apMag, float dist);

std::string vec3ToString(glm::vec3 f);

glm::vec3 polarFromCartesian(glm::vec3 pos);

glm::vec3 polarFromCartesian(glm::vec3 pos, fundamentalPlane p); // returns galactic polar (l, b, dist) from galactic cartesian coord

std::string getCameraPosText(glm::vec3 f);

glm::vec3 equatorialToXYZ(float ra, float dec); // Equatorial coordinates system to cartesian x y z

glm::vec3 equatorialInDegreesToXYZ(float ra, float dec);

glm::vec3 hexToRGB(std::string s);

glm::vec3 getColor(float colorIndex, std::map<float, glm::vec3>* colorIndexTable); // colorIndex (B-V)

glm::vec3 getColor(float colorIndex, std::vector<ColorValue>& colorIndexTable); // colorIndex (B-V)

unsigned int getSpectralClass(float BVColorIndex);

unsigned int getDotsPos(std::string s);

#endif //UTILS_H
