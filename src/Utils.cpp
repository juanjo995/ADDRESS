#include "Utils.h"

std::vector<std::string>
splitLine(std::string line, char c) {
    std::vector<std::string> strings;
    std::istringstream f(line);
    std::string s;
    while (getline(f, s, c)) {
        strings.push_back(s);
    }

    return strings;
}

// Parallax p must be given in mas (milliarcsec), distance will be returned in parsecs
float
parallaxToDist(float p) {
    return 1000.0f / p;
}

float
absMagFromApMag(float apMag, float dist) {
    return apMag - 5 * (glm::log(dist / 10.0f)) / glm::log(10.0f);
}

std::string
vec3ToString(glm::vec3 f) {
    return "x: " + std::to_string(f.x) + "\ny: " + std::to_string(f.y) + "\nz: " + std::to_string(f.z);
}

// x is alfa, y is delta, z is distance;
glm::vec3
polarFromCartesian(glm::vec3 pos) {
    glm::vec3 polar;
    float alfa, delta, distance;

    glm::vec3 normalizedDirection = glm::normalize(pos);

    glm::vec3 alfaAngle;
    alfaAngle.x = normalizedDirection.x;
    alfaAngle.y = 0.0f;
    alfaAngle.z = normalizedDirection.z;
    alfaAngle = glm::normalize(alfaAngle);

    alfa = glm::orientedAngle(glm::vec3(1.0f, 0.0f, 0.0f), alfaAngle, glm::vec3(0.0f, -1.0f, 0.0f));
    if(alfa < 0.0f) {
        alfa = glm::two_pi<float>() + alfa;
    }
    alfa = glm::two_pi<float>() - alfa;

    polar.x = alfa;

    // Compute delta
    delta = glm::orientedAngle(alfaAngle, normalizedDirection, glm::normalize(glm::cross(alfaAngle, normalizedDirection)));
    if(normalizedDirection.y < 0.0f)
        delta = -delta;

    polar.y = delta;
    polar.z = glm::length(pos);

    return polar;
}

glm::vec3
polarFromCartesian(glm::vec3 pos, fundamentalPlane p) {

    glm::vec3 v = polarFromCartesian(pos);

    if(p == EQUATORIAL) {

    } else if(p == GALACTIC) {

    } else if(p == ECLIPTIC) {

    }

    return v;
}

std::string
getCameraPosText(glm::vec3 pos) {

    glm::vec3 polar = polarFromCartesian(pos);

    std::string out;
    out =   "Camera position:\n"
            "x: " + std::to_string(pos.x) + "\n" +
            "y: " + std::to_string(pos.y) + "\n" +
            "z: " + std::to_string(pos.z);

    return out;
}

glm::vec3
equatorialToXYZ(float ra, float dec) {
    float distance = 1.0f;
    float x = glm::cos(ra) * glm::cos(dec);
    float y = glm::sin(dec);
    float z = glm::sin(ra) * glm::cos(dec);
    return distance * glm::vec3(x, y, z);
}

glm::vec3
equatorialInDegreesToXYZ(float ra, float dec) {
    return equatorialToXYZ(glm::radians(-ra), glm::radians(dec));
}

glm::vec3
hexToRGB(std::string s) {
    int n;
    std::stringstream ss;
    ss << std::hex << s;
    ss >> n;

    glm::vec3 color;

    color.x = ((n >> 16) & 0xFF) / 255.0;  // Extract the RR byte
    color.y = ((n >> 8) & 0xFF) / 255.0;   // Extract the GG byte
    color.z = ((n) & 0xFF) / 255.0;        // Extract the BB byte

    return color;
}

glm::vec3
getColor(float colorIndex, std::map<float, glm::vec3>* colorIndexTable) {
    glm::vec3 lowerColor, upperColor;
    lowerColor = std::prev(colorIndexTable->lower_bound(colorIndex))->second;
    upperColor = colorIndexTable->upper_bound(colorIndex)->second;

    float lower, upper, factor;
    if (colorIndex < -0.35f) {
        return upperColor;
    } else if(colorIndex > 1.95f) {
        return lowerColor;
    }else {
        lower = std::prev((colorIndexTable->lower_bound(colorIndex)))->first;
        upper = colorIndexTable->upper_bound(colorIndex)->first;
        factor = (colorIndex - lower) / (upper - lower);
    }

    std::cout << colorIndex << " " << lower << " " << upper << " " << factor << "\n";

    return glm::mix(lowerColor, upperColor, factor);
}

glm::vec3
getColor(float colorIndex, std::vector<ColorValue>& colorIndexTable) {

    if(colorIndex < colorIndexTable.front().colorIndex) {
        return colorIndexTable.front().rgb;
    } else if (colorIndex >= colorIndexTable.back().colorIndex) {
        return colorIndexTable.back().rgb;
    }

    for(unsigned int i = 0; i < colorIndexTable.size() - 1; ++i) {
        if(     colorIndex >= colorIndexTable[i].colorIndex
            &&  colorIndex < colorIndexTable[i + 1].colorIndex) {
            glm::vec3 lowerColor, upperColor;
            lowerColor = colorIndexTable[i].rgb;
            upperColor = colorIndexTable[i + 1].rgb;
            float lower, upper, factor;
            lower = colorIndexTable[i].colorIndex;
            upper = colorIndexTable[i + 1].colorIndex;
            factor = (colorIndex - lower) / (upper - lower);
            return glm::mix(lowerColor, upperColor, factor);
        }
    }

}

unsigned int
getSpectralClass(float BVColorIndex) {

    if(BVColorIndex >= std::numeric_limits<float>().min() && BVColorIndex < -0.315f) {
        return 0; // O
    }

    if(BVColorIndex >= -0.315f && BVColorIndex < -0.16f) {
        return 1; // B
    }

    if(BVColorIndex >= -0.16f && BVColorIndex < 0.14f) {
        return 2; // A
    }

    if(BVColorIndex >= 0.14f && BVColorIndex < 0.44f) {
        return 3; // F
    }

    if(BVColorIndex >= 0.44f && BVColorIndex < 0.695f) {
        return 4; // G
    }

    if(BVColorIndex >= 0.695f && BVColorIndex < 1.105f) {
        return 5; // K
    }

    if(BVColorIndex >= 1.105f && BVColorIndex <= std::numeric_limits<float>().max()) {
        return 6; // M
    }
}

unsigned int getDotsPos(std::string s) {
    for(unsigned int i = 0; i < s.length(); ++i) {
        if(s[i] == ':') return i;
    }
}