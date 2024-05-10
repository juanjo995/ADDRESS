#ifndef GUI_H
#define GUI_H

#include <iostream>

#include "FileDialog.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include "Parameters.h"

struct spectralClassValue {
    float ci;
    glm::vec3 rgb;
    char c;
    void draw(filter& f) {

        ImVec4 outColor = ImVec4(rgb.x, rgb.y, rgb.z, 1.0f);

        if( !(ci >= f.lowerValue() &&
            ci <= f.upperValue()) ) {
            outColor.w = 0.6f;
        }

        ImGui::GetStyle().Colors[ImGuiCol_Button] = outColor;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = outColor;

        std::string buttonName = "";
        buttonName += c;
        buttonName += "###";
        buttonName += c;
        buttonName += "Class";

        ImGui::Button(buttonName.c_str());
    }
};

class GUI {
private:

    Parameters *param;

    char *catalogsList;
    static const char *filtersList[];
    static const char *planesList[];
    static const char *speedsList[];
    static const char *viewModeList[];
    static const char *renderModeList[];

    static const char *visualizationModeList[];

    bool lastFrameCollapsed;
    int fpsTextYOffset;

    LoadOptions guiLoadOptions;

    bool loadingFile;
    bool chooseFileFormat;
    bool toSaveFileFormat; // true for CSV, false for FITS
    std::string loadingFilePath;
    std::string loadingFileName;

    bool showStats;
    stats s;
    std::string statsText;

    bool showCameraCoordsMode; //true Polar, false cartesian

    int activeFilter;
    int activePlaneInt;

    int activePlaneIntForPlot2D;

    glm::vec3 firstColor;
    glm::vec3 secondColor;
    glm::vec3 thirdColor;
    float mixFactor;

    ImVec4 co;

    spectralClassValue O;
    spectralClassValue B;
    spectralClassValue A;
    spectralClassValue F;
    spectralClassValue G;
    spectralClassValue K;
    spectralClassValue M;
    spectralClassValue L;
    spectralClassValue T;
    spectralClassValue Y;

public:

    GUI(Parameters *p);
    ~GUI();
    void draw();

    void drawGridPoints(std::vector<gridPoint>* g, glm::vec4 c, std::string s);

    glm::vec2 getLowerUpperLimits(unsigned int& offset);

    void showSpectralClassRange();

    std::string getEpochText();
    std::string getEpochSpeedText();
    void updateEpochSpeed();
    void updateCatalogsList();

    void saveToFile();
};


#endif //GUI_H
