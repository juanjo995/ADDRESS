#ifndef VIRTUALTELESCOPE_PARAMETERS_H
#define VIRTUALTELESCOPE_PARAMETERS_H

#include "Utils.h"
#include "Grid.h"
#include "StarCatalog.h"

struct Parameters {

    bool debugging;


    bool useDepthTest;

    //Time fps variables
    double lastTime;
    double elapsedTime;

    int speedMode;

    float time; // variable ro represent local time [0-24]

    double anglePerSecond;

    int screenWidth;
    int screenHeight;

    int guiVarWidth;

    std::string raDecText;

    double fpsLastTimeMeasure;
    int fpsCount;
    std::string fpsText;
    glm::vec3 fpsColor;

    renderMode activeRenderMode;
    int activeRenderModeInt;

    float plot2DSizeFactor;
    glm::vec2 plot2DOffset;

    float diagramHRSizeFactor;
    glm::vec2 diagramHROffset;

    fundamentalPlane activePlane = EQUATORIAL;
    fundamentalPlane activePlaneForPlot2D = EQUATORIAL;

    glm::mat4 earthPosMatrix;

    glm::mat4 activePlaneMatrix;
    glm::mat4 activePlaneInverseMatrix;

    glm::mat4 eqToGal;
    glm::mat4 galToEq;

    glm::mat4 eqToEcl;
    glm::mat4 eclToEq;

    glm::mat4 eqToGalCen;
    glm::mat4 galCenToEq;

    glm::mat4 identityMatrix;

    viewMode activeViewMode;
    int activeViewModeInt;

    bool orbitStarFlag;

    int activeVisualizationModeInt;

    bool useFixedPointSize;
    bool doNotAttenuate;
    bool doNotDrawHalo;

    bool intelligentDistanceAdjustment;
    float adjustmentFactor;

    ////Star selectedStar;
    Star selectedStarStored;
    Star orbitStarStored;

    Star* selectedStar;
    Star* orbitStar;
    glm::vec2 selectedStarPos;
    bool isSelectedStarVisible;

    std::string selectedStarInfoText;

    bool drawStarsFlag;
    bool drawStarNamesFlag;
    bool drawBackgroundFlag;

    ShaderProgram* currentShader;
    ShaderProgram* star3DShader;
    ShaderProgram* plot2DShader;
    ShaderProgram* diagramHRShader;

    ShaderProgram* ciAbsGraphShader;
    std::vector<StarCatalog*>* catalogs;
    int activeCatalogInt;

    StarCatalog* defaultCatalog;

    bool colorizeWithFilter;
    bool customizeVisualizationFlag;

    bool starsMovementFlag;
    bool drawVelocitiesFlag; // Render stars velocities vectors flag
    float vectorDistanceLimit; // Stars velocities vectors beyond this limit wont be rendered

    bool drawHipCatalogFlag;
    bool drawGaiaCatalogFlag;

    Grid* EQGrid;
    Grid* horizonGrid;
    Grid* galacticGrid;
    Grid* eclipticGrid;
    Grid* galactoCentricGrid;

    bool EQGridFlag;
    bool horizonGridFlag;
    std::vector<gridPoint>* EQNumbers;
    std::vector<gridPoint>* horizonNumbers;

    bool orbitFlag;

    std::vector<NameText> showingNames;

    float latitude;
    float longitude;

    float fixedPointSize;

    float AValue;
    float BValue;

    float sizeAtt;
    float alfaAtt;

    visualizationParameter activeParameter;

    float sizeFactor;
    float sizeOffset;
    float sizeSlope;

    float alfaFactor;
    float alfaOffset;
    float alfaSlope;

    float haloFactor;
    float haloOffset;
    float haloSlope;

    Camera* camera;
    float cameraSpeed;

    float displayRA;   /* Right Ascension for display in radians */
    float displayDec;   /* Declination for display in radians */
    float displayFOV;   /* Field of View for the display in degrees */
    float displayROT;
    bool fovChanged;

    float orbitDistance;

    float magFilter;

    float dragFactor;

    glm::mat4 getMatrixForPlane(fundamentalPlane p) {
        if (p == EQUATORIAL) {
            return identityMatrix;
        } else if (p == GALACTIC) {
            return eqToGal;
        } else if (p == ECLIPTIC) {
            return eqToEcl;
        } else if (p == GALACTOCENTRIC) {
            return eqToGalCen;
        } else if (p == HORIZON_VIEW) {
            return earthPosMatrix;
        }
    }

    ShaderProgram* getCurrentShader() {
        if(activeRenderMode == SPACE_3D) {
            return star3DShader;
        } else if(activeRenderMode == PLOT_2D) {
            return plot2DShader;
        } else if(activeRenderMode == HR_DIAGRAM) {
            return diagramHRShader;
        }
    }

    void setUniforms() {
        if(activeRenderMode == SPACE_3D) {

            star3DShader->use();
            star3DShader->setUniform("fixedSize", fixedPointSize);

            StarCatalog* c = getActiveCatalog();
            star3DShader->setUniform("lowerAbsMagFilter", c->getAbsMagFilter().lowerValue());
            star3DShader->setUniform("upperAbsMagFilter", c->getAbsMagFilter().upperValue());
            star3DShader->setUniform("lowerColorIndexFilter", c->getColorFilter().lowerValue());
            star3DShader->setUniform("upperColorIndexFilter", c->getColorFilter().upperValue());
            star3DShader->setUniform("lowerSpeedModuleFilter", c->getVelocityFilter().lowerValue());
            star3DShader->setUniform("upperSpeedModuleFilter", c->getVelocityFilter().upperValue());
        } else if(activeRenderMode == PLOT_2D) {

            plot2DShader->use();
            plot2DShader->setUniform("aspectRatio", float(screenWidth - guiVarWidth) / float(screenHeight));
            plot2DShader->setUniform("plot2DSizeFactor", plot2DSizeFactor);
            plot2DShader->setUniform("offset", plot2DOffset);

            plot2DShader->setUniform("fixedSize", fixedPointSize);

            StarCatalog* c = getActiveCatalog();
            plot2DShader->setUniform("lowerAbsMagFilter", c->getAbsMagFilter().lowerValue());
            plot2DShader->setUniform("upperAbsMagFilter", c->getAbsMagFilter().upperValue());
            plot2DShader->setUniform("lowerColorIndexFilter", c->getColorFilter().lowerValue());
            plot2DShader->setUniform("upperColorIndexFilter", c->getColorFilter().upperValue());
            plot2DShader->setUniform("lowerSpeedModuleFilter", c->getVelocityFilter().lowerValue());
            plot2DShader->setUniform("upperSpeedModuleFilter", c->getVelocityFilter().upperValue());
        } else if(activeRenderMode == HR_DIAGRAM) {

            diagramHRShader->use();
            diagramHRShader->setUniform("aspectRatio", float(screenWidth - guiVarWidth) / float(screenHeight));
            diagramHRShader->setUniform("diagramHRSizeFactor", diagramHRSizeFactor);
            diagramHRShader->setUniform("offset", diagramHROffset);

            diagramHRShader->setUniform("fixedSize", fixedPointSize);

            StarCatalog* c = getActiveCatalog();
            diagramHRShader->setUniform("lowerAbsMagFilter", c->getAbsMagFilter().lowerValue());
            diagramHRShader->setUniform("upperAbsMagFilter", c->getAbsMagFilter().upperValue());
            diagramHRShader->setUniform("lowerColorIndexFilter", c->getColorFilter().lowerValue());
            diagramHRShader->setUniform("upperColorIndexFilter", c->getColorFilter().upperValue());
            diagramHRShader->setUniform("lowerSpeedModuleFilter", c->getVelocityFilter().lowerValue());
            diagramHRShader->setUniform("upperSpeedModuleFilter", c->getVelocityFilter().upperValue());

        }
    }

    void renderModeChanged() {
        if(activeRenderMode == SPACE_3D) {
            currentShader = star3DShader;
        } else if(activeRenderMode == PLOT_2D) {
            plot2DShader->use();
            plot2DShader->setUniform("aspectRatio", float(screenWidth - guiVarWidth) / float(screenHeight));

            currentShader = plot2DShader;
        } else if (activeRenderMode == HR_DIAGRAM){
            diagramHRShader->use();
            diagramHRShader->setUniform("aspectRatio", float(screenWidth - guiVarWidth) / float(screenHeight));

            currentShader = diagramHRShader;
        }
    }

    void resetVisualizationParams() {

        if( activeRenderMode == SPACE_3D) {
            useFixedPointSize = false;
            fixedPointSize = 5.0f;
            sizeAtt = 1.0f;
            doNotAttenuate = true;
            alfaAtt = 0.2f;
        }

        if( activeRenderMode == PLOT_2D ||
            activeRenderMode == HR_DIAGRAM)
        {
            useFixedPointSize = true;
            fixedPointSize = 2.5f;
            sizeAtt = 1.0f;
            doNotAttenuate = true;
            alfaAtt = 0.28f;
        }

        useDepthTest = false;
        switchDepthTest();

        displayFOV = 60.0f;
        fovChanged = true;

        setUniforms();
    }

    void switchDepthTest() {
        if(useDepthTest) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            //glDepthMask(GL_FALSE);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void switchToOrbitStar() {
        activeViewMode = ORBIT;
        activeViewModeInt = activeViewMode;
        orbitStarFlag = true;

        glm::vec3 polarCoord = polarFromCartesian(camera->getPosition() - getApropiateCartesianPos(orbitStar->get3DPosition(getActiveCatalog()->getEpoch())));
        displayRA = polarCoord.x;
        displayDec = polarCoord.y;
        orbitDistance = polarCoord.z;
        camera->setOrbit(displayRA, displayDec, orbitDistance, getApropiateCartesianPos(orbitStar->get3DPosition(getActiveCatalog()->getEpoch())));

    }

    void orbitToFreeCamera() {
        // We do this when the user is orbiting a star and clicks on empty space so he can switch to free camera mode
        glm::vec3 polarCoord = polarFromCartesian(getApropiateCartesianPos(orbitStar->get3DPosition(getActiveCatalog()->getEpoch())) - camera->getPosition());

        displayRA = polarCoord.x;
        displayDec = polarCoord.y;

        //camera->setLookAt(;)
        camera->setRaLootAk(-displayRA);
        camera->setLookAtForFreeCamera(displayRA, displayDec);

        activeViewMode = FREE_CAMERA;
        activeViewModeInt = activeViewMode;
        orbitStarFlag = false;
    }

    glm::mat4 getTimeMatrix() {
        float hourAngle = time;
        if(hourAngle >= glm::two_pi<float>()) hourAngle = hourAngle - glm::two_pi<float>();
        if(hourAngle < 0.0f) hourAngle = glm::two_pi<float>() + hourAngle;

        return glm::rotate(glm::mat4(1.0f), -hourAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 getApropiateCartesianPos(glm::vec3 pos) {
        return activePlaneMatrix * getTimeMatrix() * glm::vec4(pos, 1.0f);
    }

    void setCameraParameters() {
        if(activeViewMode == FREE_CAMERA) {
            // If we emulate free camera we must change camera lookAt
            // camera lookAt will be stored as alfa delta polar coordrs
            camera->setRaLootAk(-displayRA);
            camera->setLookAtForFreeCamera(displayRA, displayDec);
        }
        if(activeViewMode == ORBIT) {

            if(orbitStar != nullptr) {
                camera->setRaLootAk(glm::mod(-displayRA + glm::pi<float>(),glm::two_pi<float>()));
                camera->setOrbit(displayRA, displayDec, orbitDistance, getApropiateCartesianPos(orbitStar->get3DPosition(getActiveCatalog()->getEpoch())));
            } else {
                // If we orbit we do this
                //camera->setRaLootAk(param.displayRA);
                camera->setRaLootAk(glm::mod(displayRA + glm::pi<float>(),glm::two_pi<float>()));
                camera -> setPosition(displayRA, displayDec, orbitDistance);
            }

        }
        if(activeViewMode == CENTERED_ON_ORIGIN) {
            camera -> setLookAtFromPolarCoords(displayRA, displayDec);
            //computeRaDecText();
        }
    }

    void setViewMode(viewMode v) {
        activeViewMode = v;
        activeViewModeInt = v;
        switch(activeViewMode) {
            case CENTERED_ON_ORIGIN:
                camera -> locate(glm::vec3(0.0f, 0.0f, 0.0f), equatorialInDegreesToXYZ(displayRA, displayDec));
                break;
            case HORIZON_VIEW:
                camera -> locate(glm::vec3(0.0f, 0.0f, 0.0f), equatorialInDegreesToXYZ(displayRA, displayDec));
                break;
            case FREE_CAMERA:
                camera->setRaLootAk(-displayRA);
                camera->setLookAtForFreeCamera(displayRA, displayDec);
                break;
            case ORBIT:
                orbitDistance = 2.0f;
                camera->setRaLootAk(glm::mod(displayRA + glm::pi<float>(),glm::two_pi<float>()));
                camera->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
                camera -> setPosition(displayRA, displayDec, orbitDistance);
                break;
        }
    }

    void updateEarthPosMatrix() {

        float hourAngle = time + glm::radians(longitude);
        if(hourAngle >= glm::two_pi<float>()) hourAngle = hourAngle - glm::two_pi<float>();
        if(hourAngle < 0.0f) hourAngle = glm::two_pi<float>() + hourAngle;

        earthPosMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(latitude - 90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::rotate(glm::mat4(1.0f), -hourAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        activePlaneMatrix = earthPosMatrix;
        activePlaneInverseMatrix = glm::inverse(activePlaneMatrix);
    }

    StarCatalog* getActiveCatalog() {

        if(catalogs->size() != 0) {
            return catalogs->operator[](activeCatalogInt);
        } else {
            return nullptr;
        }

    }

    std::string getRaDecTex() {

        float alfa, delta;
        glm::vec3 polarCoords;

        if(activeViewMode == CENTERED_ON_ORIGIN) {
            alfa = displayRA;
            delta = displayDec;
        }

        if(activeViewMode == FREE_CAMERA || activeViewMode == ORBIT) {
            polarCoords = polarFromCartesian(camera->getPosition());
            alfa = polarCoords.x;
            delta = polarCoords.y;
        }

        int h, m;
        float s;
        std::string raString;
        float ra;

        if(activePlane == GALACTIC || activePlane == ECLIPTIC || activePlane == GALACTOCENTRIC || activePlane == HORIZON_VIEW) {

            if(activePlane == HORIZON_VIEW) alfa = glm::two_pi<float>() - alfa;

            ra = glm::degrees(alfa);

            h = (int) std::trunc(ra);
            m = (int) std::trunc((ra - std::trunc(ra)) * 60.0f);
            s = ((ra - std::trunc(ra)) * 60.0f - float(m)) * 60.0f;

            raString = " ";
            if(h < 10) {
                raString += "  ";
            } else if (h < 100) {
                raString += " ";
            }
            raString += std::to_string(h) + "º ";
            if(m < 10) raString += " ";
            raString += std::to_string(m) + "' ";
            if(s < 10.0f) raString += " ";
            raString += std::to_string(s) + "\"";
        } else if (activePlane == EQUATORIAL) {
            ra = (glm::degrees(alfa) / 15.0f);
            h = (int) std::trunc(ra);
            m = (int) std::trunc((ra - std::trunc(ra)) * 60.0f);
            s = ((ra - std::trunc(ra)) * 60.0f - float(m)) * 60.0f;

            raString = " ";
            if (h < 10) raString += " ";
            raString += std::to_string(h) + "h ";
            if (m < 10) raString += " ";
            raString += std::to_string(m) + "m ";
            if (s < 10.0f) raString += " ";
            raString += std::to_string(s) + "s";
        }

        float dec = glm::abs(glm::degrees(delta));

        h = (int) std::trunc(dec);
        m = (int) std::trunc((dec - std::trunc(dec)) * 60.0f);
        s = ((dec - std::trunc(dec)) * 60.0f - float(m)) * 60.0f;

        std::string decString = " ";
        if(delta < 0.0f) decString = "-";
        if(h < 10) decString = " " + decString;
        decString += std::to_string(h) + "º ";
        if(m < 10) decString += " ";
        decString += std::to_string(m) + "' ";
        if(s < 10.0f) decString += " ";
        decString += std::to_string(s) + "\"";

        std::string out;

        if(activeViewMode == CENTERED_ON_ORIGIN) {
            out = "Looking at:\n";
        }

        if(activeViewMode == FREE_CAMERA || activeViewMode == ORBIT) {
            out = "Camera position:\n";
        }

        if(activePlane == GALACTIC) {
            out += "  l:" + raString + "\n  b: " + decString;
        }

        if(activePlane == EQUATORIAL) {
            out += " Ra: " + raString + "\nDec: " + decString;
        }

        if(activePlane == ECLIPTIC) {
            out += "Ecl lon:" + raString + "\nEcl lat: " + decString;
        }

        if(activePlane == GALACTOCENTRIC) {
            out += "Gal lon:" + raString + "\nGal lat: " + decString;
        }

        if(activePlane == HORIZON_VIEW) {
            out += "Az: " + raString + "\nAlt: " + decString;
        }

        if(activeViewMode == FREE_CAMERA || activeViewMode == ORBIT) {
            if(activePlane == GALACTOCENTRIC) {
                out += "\n" + std::to_string(glm::length(camera->getPosition())) + " parsecs to Sagittarius A*";
            } else {
                out += "\n" + std::to_string(glm::length(camera->getPosition())) + " parsecs to the sun";
            }
        }

        return out;

    }

};


#endif //VIRTUALTELESCOPE_PARAMETERS_H
