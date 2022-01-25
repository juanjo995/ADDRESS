#include "Star.h"

Star::Star() {
    position = glm::vec2(0.0f, 0.0f);
}

Star::Star(double r, double d, double di, double absMa, glm::vec3 c, float ci, float pmr, float pmd, float rv, bool computeVelMode) {

    position.x = r;
    position.y = d;

    distance = di;

    position3D = get3DPosition();

    velocity = compute3DVelocity(pmr, pmd, rv);

    if(computeVelMode) {
        if((pmr == 0.0f && pmd == 0.0f) || rv == 0.0f) {
            velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            speedModule = glm::length(velocity);
        }
    }

    absMag = absMa;
    colorIndex = ci;
    color = c;
}

bool
Star::operator==(const Star& rhs) {
    return  this->id == rhs.id &&
            this->position == rhs.position &&
            this->distance == rhs.distance &&
            this->position3D == rhs.position3D &&
            this->velocity == rhs.velocity &&
            this->absMag == rhs.absMag &&
            this->colorIndex == rhs.colorIndex &&
            this->color == rhs.color &&
            this->speedModule == rhs.speedModule;

}

glm::vec2
Star::getPosition() {
    return position;
}

glm::vec3
Star::get3DPosition() {
    return distance * glm::vec3(glm::cos(-position.x) * glm::cos(position.y), glm::sin(position.y), glm::sin(-position.x) * glm::cos(position.y));

}

// time is epoch
glm::vec3
Star::get3DPosition(float time) {
    glm::vec3 pos = distance * glm::vec3(glm::cos(-position.x) * glm::cos(position.y), glm::sin(position.y), glm::sin(-position.x) * glm::cos(position.y));
    glm::vec3 vel = this->get3DVelocity();
    return pos + vel * time;
}

glm::vec3
Star::compute3DVelocity(float pmr, float pmd, float rv) {

    if(pmr == 0.0f && pmd == 0.0f && rv == 0.0f) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 vel;

    glm::vec3 pmVec(0.0f, pmd, -pmr);
    float pmAngle;

    if(pmd == 0.0f && pmr == 0.0f) {
        pmAngle = 0.0f;
    } else {
        pmAngle = glm::length(pmVec);
    }

    float pmVelocity = 4.74f * pmAngle * distance / 1000.0f; // Km/s

    glm::vec3 tr;
    if(pmd == 0.0f && pmr == 0.0f) {
        tr = glm::vec3(0.0f, 0.0f, 0.0f);
    } else {
        tr = glm::normalize(pmVec);
    }

    if(pmAngle == 0.0f) {
        vel = glm::vec3(rv, 0.0f, 0.0f);
    } else {
        vel = tr * pmVelocity + glm::vec3(rv, 0.0f, 0.0f);
    }

    speedModule = glm::length(vel);

    glm::mat3 decRot(glm::rotate(glm::mat4(1.0f), float(position.y), glm::vec3(0.0f, 0.0f, 1.0f)));
    glm::mat3 raRot(glm::rotate(glm::mat4(1.0f), float(position.x), glm::vec3(0.0f, 1.0f, 0.0f)));

    return (raRot * decRot * vel) * 0.0000010227121650456f; // from km/s to parsecs/year;
}

glm::vec3 // x - pmRa*, y pmDec, z radialVelocity
Star::getVelocityParams() {

    glm::vec3 params;

    glm::vec3 polarCoords = getPolarFromCartesian(position3D);

    glm::mat3 decRot(glm::rotate(glm::mat4(1.0f), float(polarCoords.y), glm::vec3(0.0f, 0.0f, 1.0f)));
    glm::mat3 raRot(glm::rotate(glm::mat4(1.0f), float(polarCoords.x), glm::vec3(0.0f, 1.0f, 0.0f)));

    glm::vec3 vel = glm::inverse(raRot * decRot) * (velocity / 0.0000010227121650456f);

    params.z = vel.x;
    vel.x = 0.0f;

    float pmVelocity = glm::length(vel);
    float pmAnge = (pmVelocity * 1000.0f) / (4.74f * distance);

    vel = glm::normalize(vel) * pmAnge;

    params.x = -vel.z;
    params.y = vel.y;


    //Check for nans or low radial vel
    if(params.x != params.x) { params.x = 0.0f; }
    if(params.y != params.y) { params.y = 0.0f; }
    if(glm::abs(params.z) < 0.0001f) {params.z = 0.0f; } // values below 0.0001 are problably 0
                                                         // but due to matrix operations and decimal precision
                                                         // we dont get 0

    return params;
}

glm::vec3
Star::get3DVelocity() {
    return velocity;
}

float
Star::getDistance() {
    return distance;
}

float
Star::getAbsMag() {
    return absMag;
}

float
Star::getAppMag() {
    return 5.0f * (glm::log(distance / 10.0f) / glm::log(10.0f)) + absMag;
}

float
Star::getColorIndex() {
    return colorIndex;
}

float
Star::getTemperature() {
    return 4600.0f * ((1.0f / (0.92f * colorIndex + 1.7f)) + (1.0f / (0.92f * colorIndex + 0.62f)));
}

float
Star::getParallax() {
    return 1000.0f / distance;
}

std::string
Star::getInfoLine() {
    return "(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ") - "
                + std::to_string(distance) + " - " + std::to_string(absMag);
}

std::string
Star::getInfoText() {
    std::string text = "";

    int h, m;
    float s;
    float ra = (glm::degrees(this->position.x) / 15.0f);
    h = (int) std::trunc(ra);
    m = (int) std::trunc((ra - std::trunc(ra)) * 60.0f);
    s = ((ra - std::trunc(ra)) * 60.0f - float(m)) * 60.0f;

    std::string raString = " ";
    if(h < 10) raString += " ";
    raString += std::to_string(h) + "h ";
    if(m < 10) raString += " ";
    raString += std::to_string(m) + "m ";
    if(s < 10.0f) raString += " ";
    raString += std::to_string(s) + "s";


    float dec = glm::abs(glm::degrees(this->position.y));

    h = (int) std::trunc(dec);
    m = (int) std::trunc((dec - std::trunc(dec)) * 60.0f);
    s = ((dec - std::trunc(dec)) * 60.0f - float(m)) * 60.0f;

    std::string decString = " ";
    if(this->position.y < 0.0f) decString = "-";
    if(h < 10) decString = " " + decString;
    decString += std::to_string(h) + "º ";
    if(m < 10) decString += " ";
    decString += std::to_string(m) + "' ";
    if(s < 10.0f) decString += " ";
    decString += std::to_string(s) + "\"";


    int maxLineIndex = 0;
    int maxLineSize = 0;
    int maxLineDotsPosition = 0;
    std::vector<std::string> lines;
    lines.clear();

    std::string raStr = " Ra:" + raString;
    lines.push_back(raStr);

    std::string decStr = "Dec:" + decString;
    lines.push_back(decStr);

    std::string parallaxString = "Parallax: " + std::to_string(getParallax()) + " mas";
    lines.push_back(parallaxString);

    std::string distanceString = "Distance: " + std::to_string(distance) + " parsecs";
    lines.push_back(distanceString);

    std::string appMagString = "App mag: " + std::to_string(getAppMag());
    lines.push_back(appMagString);

    std::string absMagString = "Abs mag: " + std::to_string(absMag);
    lines.push_back(absMagString);

    std::string BVIndexString = "B-V Index: " + std::to_string(colorIndex);
    lines.push_back(BVIndexString);

    std::string temperatureString = "Temperature: " + std::to_string(getTemperature()) + "º K";
    lines.push_back(temperatureString);

    std::string velocityString = "Velocity: " + std::to_string(speedModule) + " Km/s";
    lines.push_back(velocityString);

    glm::vec3 velParams = getVelocityParams();

    std::string pmRaString = "pmRa*: " + std::to_string(velParams.x) + " mas/year";
    lines.push_back(pmRaString);

    std::string pmDecString = "pmDec: " + std::to_string(velParams.y) + " mas/year";
    lines.push_back(pmDecString);

    std::string radialVelocityString = "Radial vel.: " + std::to_string(velParams.z) + " Km/s";
    lines.push_back(radialVelocityString);

    for(unsigned int i = 0; i < lines.size(); ++i) {
        if(lines[i].length() > maxLineSize) {
            maxLineSize = lines[i].length();
            maxLineIndex = i;
        }
    }
    maxLineDotsPosition = getDotsPos(lines[maxLineIndex]);

    for(unsigned int i = 0; i < lines.size(); ++i) {

        if(getDotsPos(lines[i]) < maxLineDotsPosition) {
            lines[i] = addBlankSpaces(lines[i], maxLineDotsPosition - getDotsPos(lines[i]));
        }

    }

    maxLineDotsPosition = 0;
    maxLineIndex = 0;

    for(unsigned int i = 0; i < lines.size(); ++i) {
        if(getDotsPos(lines[i]) > getDotsPos(lines[maxLineIndex])) maxLineIndex = i;
    }
    maxLineDotsPosition = getDotsPos(lines[maxLineIndex]);

    for(unsigned int i = 0; i < lines.size(); ++i) {

        if(getDotsPos(lines[i]) < maxLineDotsPosition) {
            lines[i] = addBlankSpaces(lines[i], maxLineDotsPosition - getDotsPos(lines[i]));
        }

    }

    return  lines[0] + "\n" +
            lines[1] + "\n\n" +
            lines[2] + "\n" +
            lines[3] + "\n\n" +
            lines[4] + "\n" +
            lines[5] + "\n\n" +
            lines[6] + "\n" +
            lines[7] + "\n\n" +
            lines[8] + "\n\n" +
            lines[9] + "\n" +
            lines[10] + "\n\n" +
            lines[11] + "\n";

    return text;

}

std::string
Star::getCSVLine() {
    //"ra,dec,parallax,phot_g_mean_mag,bp_g,pmra,pmdec,dr2_radial_velocity\n";
    glm::vec3 polarCoords = getPolarFromCartesian(position3D);
    polarCoords.x = glm::degrees(polarCoords.x);
    polarCoords.y = glm::degrees(polarCoords.y);

    glm::vec3 velParams = getVelocityParams();

    return  std::to_string(polarCoords.x) + ',' +
            std::to_string(polarCoords.y) + ',' +
            std::to_string(getParallax()) + ',' +
            std::to_string(getAppMag()) + ',' +
            std::to_string(getColorIndex()) + ',' +
            std::to_string(velParams.x) + ',' +
            std::to_string(velParams.y) + ',' +
            std::to_string(velParams.z) + '\n';

}

unsigned int
Star::getDotsPos(std::string s) {
    for(unsigned int i = 0; i < s.length(); ++i) {
        if(s[i] == ':') return i;
    }
}

std::string
Star::addBlankSpaces(std::string s, unsigned int n) {
    for(unsigned int i = 0; i < n; ++i) {
        s = " " + s;
    }
    return s;
}

glm::vec3
Star::getPolarFromCartesian(glm::vec3 pos) {
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