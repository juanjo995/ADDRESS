#ifndef VIRTUALTELESCOPE_STARCATALOG_H
#define VIRTUALTELESCOPE_STARCATALOG_H

#include <cstdio>
#include <vector>

#include <glm/gtc/random.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <CCfits>

#include "Utils.h"
#include "Star.h"

struct LoadOptions {
    bool discartAppMag;
    float defaultAppMag;

    bool discartColorIndex;
    float defaultColorIndex;

    bool discartParallax;
    float defaultDistance;

    bool onlyWithRadialVel;

};

struct limit {
    float min;
    float max;
};

struct filter {
    float min;
    float max;
    float lower;
    float upper;

    std::string rangeText;

    float lowerValue() {
        return (lower - 1.0f) * (max - min) + min;
    }
    float upperValue() {
        return (upper - 1.0f) * (max - min) + min;
    }

    std::string getRangeText() {
        return "(" + std::to_string(lowerValue()) + ", " + std::to_string(upperValue()) + ")";
    }

};

struct stats {
    int entries; // number of stars in the statistic

    float absMagAverage;
    float absMagVariance;
    float absMagMin;
    float absMagMax;

    float ciAverage;
    float ciVariance;
    float ciMin;
    float ciMax;

    float distanceAverage;
    float distanceVariance;
    float distanceMin;
    float distanceMax;

    float velocityAverage;
    float velocityVariance;
    float velocityMin;
    float velocityMax;
};

enum catalogSource{HYG, GAIA300K_RI, GAIA300K_BR};

class StarCatalog {
private:

    LoadOptions loadOptions;
    std::string sourceFileName;
    int totalNumberOfColumnsInCSVFile; // We need this in case that there are nulls fields in the last column
    int totalNumberOfColumnsInFITSFile;

    float refEpoch;

    float epoch; // used to visualize stars movement
    float epochSpeed;
    int epochSpeedInt;

    static unsigned int starID; // ID for mouse-click star detection, unique for each star
    unsigned int firstID;
    unsigned int lastID;

    std::vector<Star> catalog;
    unsigned int catalogSize;
    std::vector<Star> unknownDistanceStars;

    bool checkFilters(Star& s); // checks if a star passes the current filters

    ShaderProgram *CiAbsGraphShader;

    bool drawCatalog; // Flag to force rendering with any active catalog selected
    bool showCiAbsGraph;

    bool useCustomColorFlag;
    float customColor[4];

    int knownDistanceStarsCount;
    int unknownDistanceStarsCount;
    int unknownMagnitudeStarsCount;

    limit distance;
    limit absMag;
    limit BVColorIndex;
    limit speedModule;

    filter distanceFilter;
    filter colorFilter;
    filter absMagFilter;
    filter velocityFilter;

    GLuint starsVAO;
    GLuint starsVBO; // 0 for positions, 1 for distances, 2 magnitude, 3 aparent mag, 4 color, 5 abs mag, 6 int id, 7 color index, 8 veolici, 9 3d position
    GLuint starsIBO;
    std::vector<GLuint> starsIndices;

    std::vector<std::pair<int, float>> indexToDistance; // Index-Distance map
    std::map<float, int> DistanceToIndex; // Distance-Index map

    float apMagModifier;
    float absMagModifier;

    int lowerDistanceFilterIndex;
    int upperDistanceFilterIndex;
    float lowerAbsMagFilter;
    float upperAbsMagFilter;
    float lowerSpeedModuleFilter;
    float upperSpeedModuleFilter;
    float lowerColorIndexFilter;
    float upperColorIndexFilter;
    unsigned int indexFilter;

    // CSV File params column Pos
    int raColumn;
    int decColumn;
    int parallaxColumn;
    int appMagColumn;
    int colorIndexColumn;
    int pmRaColumn;
    int pmDecColumn;
    int radialVelColumn;

    int findColumn(std::vector<std::string>& splittedLine, std::string s);
    std::vector<starWithProperName> starsWithName;

public:

    StarCatalog();
    ~StarCatalog();

    std::string getSourceFileName();

    void setLowerIndexForDistance(float dist);

    bool isIdInCatalog(unsigned int id);
    Star getStarWithId(unsigned int id);

    bool& getDrawCatalog();
    bool& getShowCiAbsGraph();
    bool& getUseCustomColorFlag();
    float* getCustomColor();
    void setCustomColor(glm::vec4 color);

    void setCiAbsGraphShader(ShaderProgram* sh);

    bool checkFirstCSVLine(std::string line);
    bool checkFITSParams(std::vector<std::string> params);

    int size();

    void changeIndexFilter(bool b);

    float& getApMagModifier();
    float& getAbsMagModifier();

    float& getRefEpoch();
    float& getEpoch();
    float& getEpochSpeed();
    int& getEpochSpeedInt();

    std::string getDistanceFilterRangeText();
    std::string getColorIndexFilterRangeText();

    filter& getDistanceFilter();
    filter& getColorFilter();
    filter& getAbsMagFilter();
    filter& getVelocityFilter();

    limit& getDistanceLimits();
    limit& getAbsMagLimits();
    limit& getColorIndexLimits();
    limit& getSpeedModuleLimits();
    int& getLowerDistanceFilter();
    int& getUpperDistanceFilter();
    float& getLowerDistanceFloatFilter();
    float& getUpperDistanceFloatFilter();
    float& getLowerAbsMagFilter();
    float& getUpperAbsMagFilter();
    float& getLowerApMagFilter();
    float& getUpperApMagFilter();
    float& getLowerColorIndexFilter();
    float& getUpperColorIndexFilter();
    float& getLowerSpeedModuleFilter();
    float& getUpperSpeedModuleFilter();

    bool loadGEDR3(std::string filePath, LoadOptions guiOptions);
    void loadHYG(std::string filePath);

    void computeStats(stats& s);

    std::vector<starWithProperName>& getStarsWithName();

    bool saveToCSVFile(std::string path);
    bool saveToFITSFile(std::string path);

    void drawVelocities();

    void update2DCoords(glm::mat4 m);
    void applyColorization(glm::vec3 firstColor, glm::vec3 secondColor, glm::vec3 thirdColor, float mixFactor, float min, float max, unsigned int offset);
    void resetBVColor();
    void nullDraw();

    void draw(renderMode r, ShaderProgram* sp,  glm::mat4 vpm);
    void draw();

};


#endif //VIRTUALTELESCOPE_STARCATALOG_H
