#include "Renderer.h"

std::string getAppDir() {
    const char* appDir = std::getenv("APPDIR");
    return appDir ? std::string(appDir) : ".";
}

const char * GetGLErrorStr(GLenum err)
{
    switch (err)
    {
        case GL_NO_ERROR:          return "No error";
        case GL_INVALID_ENUM:      return "Invalid enum";
        case GL_INVALID_VALUE:     return "Invalid value";
        case GL_INVALID_OPERATION: return "Invalid operation";
        //case GL_STACK_OVERFLOW:    return "Stack overflow";
        //case GL_STACK_UNDERFLOW:   return "Stack underflow";
        case GL_OUT_OF_MEMORY:     return "Out of memory";
        default:                   return "Unknown error";
    }
}

void CheckGLError()
{
    while (true)
    {
        const GLenum err = glGetError();
        if (GL_NO_ERROR == err)
            break;

        std::cout << "GL Error: " << GetGLErrorStr(err) << std::endl;
    }
}

Renderer::Renderer() {

    clicked = false;
    mode  = true;

    // Init EQ to Galactic matrix
    // Sagitarius EQ coordinates
    float sagRa = glm::radians(266.416837083f);
    float sagDec = glm::radians(-29.007810556f);

    glm::vec3 sag = glm::normalize(equatorialToXYZ(-sagRa, sagDec));

    // North Galactic Pole in EQ coordinates
    float NGPRa = glm::radians(192.859508333f);
    float NGPDec = glm::radians(27.128336111f);

    glm::vec3 NGP = glm::normalize(equatorialToXYZ(-NGPRa, NGPDec));

    // We must correct NCP to make sure angle between NCP and Sag is exactly Pi/2 (90º)

    glm::vec3 east = glm::normalize(glm::cross(sag, NGP));
    glm::vec3 newNGP = glm::normalize(glm::cross(east, sag));

    param.eqToGal = glm::rotate(glm::mat4(1.0f), -glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), sag, newNGP);

    param.galToEq = glm::inverse(param.eqToGal);

    //8178 parsecs to sag*
    param.eqToGalCen =  glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) *
                        glm::translate(glm::mat4(1.0f),glm::vec3(-8178.0f, 0.0f, 0.0f)) * param.eqToGal;
    param.galCenToEq = glm::inverse(param.eqToGalCen);

    // Init EQ to Ecliptic matrix
    // X Axis direction in EQ coordinates, is (0.0f, 0.0f) defined on vernal equinox intersection with the ecliptic
    float xAxisRa = 0.0f;
    float xAxisDec = 0.0f;

    glm::vec3 xAxis = glm::normalize(equatorialToXYZ(-xAxisRa, xAxisDec));

    // North Ecliptic Pole in EQ coordinates
    // Ra: 18h 0m 0s, Dec: 66º 33' 38.55" (Dec is equal to 90º - 23.4º (Earth tilt))
    float NEPRa = glm::radians(270.0f);
    float NEPDec = glm::radians(66.5607f);

    glm::vec3 NEP = glm::normalize(equatorialToXYZ(-NEPRa, NEPDec));

    // We must correct NEP to make sure angle between NEP and xAxis is exactly Pi/2 (90º)
    glm::vec3 eclipticEast = glm::normalize(glm::cross(xAxis, NEP));
    glm::vec3 newNEP = glm::normalize(glm::cross(eclipticEast, xAxis));

    param.eqToEcl = glm::rotate(glm::mat4(1.0f), -glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), xAxis, newNEP);

    param.eclToEq = glm::inverse(param.eqToEcl);

    param.identityMatrix = glm::mat4(1.0f);

    param.activePlane = EQUATORIAL;
    param.activePlaneForPlot2D = EQUATORIAL;

    param.activePlaneMatrix = param.identityMatrix;
    param.activePlaneInverseMatrix = param.identityMatrix;

    param.intelligentDistanceAdjustment = false;
    param.adjustmentFactor = 0.0125f;

    param.selectedStar = nullptr;
    param.orbitStar = nullptr;

    param.useDepthTest = false;
    param.debugging = false;

    param.latitude = 45.0f;
    param.longitude = 0.0f;
    param.time = 0.0;

    param.earthPosMatrix = getEarthPosMatrix();

    EQGrid = new Grid(true, param.identityMatrix, EQUATORIAL);
    EQGrid->color = glm::vec4(0.0f, 1.0f, 0.0f, 0.375f);

    horizonGrid = new Grid(false, getEarthPosMatrix(), HORIZON_VIEW);
    horizonGrid->color = glm::vec4(1.0f, 1.0f, 0.0f, 0.375f);

    galacticGrid = new Grid(false, param.galToEq, GALACTIC);
    galacticGrid->color = glm::vec4(0.65f, 0.0f, 1.0f, 0.5f);

    eclipticGrid = new Grid(false, param.eclToEq, ECLIPTIC);
    eclipticGrid->color = glm::vec4(1.0f, 0.4f, 0.4f, 0.375f);

    galactoCentricGrid = new Grid(false, param.galCenToEq, GALACTOCENTRIC);
    galactoCentricGrid->color = glm::vec4(0.0f, 0.6f, 1.0f, 0.375f);

    param.EQGrid =              EQGrid;
    param.horizonGrid =         horizonGrid;
    param.galacticGrid =        galacticGrid;
    param.eclipticGrid =        eclipticGrid;
    param.galactoCentricGrid =  galactoCentricGrid;

    param.screenWidth = 1280;
    param.screenHeight = 720;

    param.guiVarWidth = 0;
    param.lastTime = 0.0;
    param.speedMode = 0;
    param.anglePerSecond = glm::two_pi<double>() / 86400.0;
    param.fpsLastTimeMeasure = 0.0f;
    param.fpsCount = 0;
    param.fpsText = "";
    param.fpsColor = glm::vec3(0.0f, 1.0f, 0.0f);

    camera = new Camera(param.screenWidth, param.screenHeight);
    param.camera = camera;
    param.cameraSpeed = 10.0f;

    param.activeRenderModeInt = 0;
    param.activeRenderMode = (renderMode) param.activeRenderModeInt;

    param.plot2DSizeFactor = 1.0f;
    param.plot2DOffset = glm::vec2(0.0f, 0.0f);

    param.diagramHRSizeFactor = 2.27f;
    param.diagramHROffset = glm::vec2(-0.13f, 0.11f);

    param.activeViewMode = CENTERED_ON_ORIGIN;
    param.activeViewModeInt = CENTERED_ON_ORIGIN;

    param.orbitStarFlag = false;

    param.activeVisualizationModeInt = 0;
    param.doNotAttenuate = false;
    param.doNotDrawHalo = false;
    param.useFixedPointSize = false;

    param.fixedPointSize = 5.0f;

    param.colorizeWithFilter = false;
    param.customizeVisualizationFlag = false;

    param.starsMovementFlag = false;
    param.drawVelocitiesFlag = false;
    param.vectorDistanceLimit = 100.0f;

    param.drawStarsFlag = true;
    param.drawStarNamesFlag = false;
    param.drawBackgroundFlag = false;

    param.drawHipCatalogFlag = true;
    param.drawGaiaCatalogFlag = true;

    param.EQGridFlag = false;
    param.horizonGridFlag = false;
    param.EQNumbers = &(EQGrid->gridPoints);
    param.horizonNumbers = &(horizonGrid->gridPoints);

    param.orbitFlag = true;

    param.displayRA = glm::radians(88.0f);
    param.displayDec = glm::radians(-5.0f);

    param.displayFOV = 60.0f;
    param.displayROT =  0.0;
    param.fovChanged = false;

    param.orbitDistance = 3.0f;

    param.magFilter = 6.0f;

    camera -> setLookAt(param.displayRA, param.displayDec);
    camera -> setFovX(glm::radians(param.displayFOV));

    computeDragFactor();
    axis = new Axis();

    param.AValue = 1.0f;
    param.BValue = 500.0f;

    param.sizeAtt = 1.0f;
    param.alfaAtt = 0.2f;

    param.activeParameter = SIZE_PARAMS;

    param.sizeFactor = 20.0f;
    param.sizeOffset = 0.0f;
    param.sizeSlope = 2.5f;

    param.alfaFactor = 1.0f;
    param.alfaOffset = 0.0f;
    param.alfaSlope = 6.0f;

    param.haloFactor = 1.0f;
    param.haloOffset = 0.0f;
    param.haloSlope = 0.2f;

    appDir = getAppDir() + "/";

}

Renderer::~Renderer() {
    std::cout << "Releasing resources...\n";

    if(catalogs) {
        for(int i = 0; i < catalogs->size(); ++i) {
            if(catalogs->operator[](i)) {
                delete catalogs->operator[](i);
            }
        }
        catalogs->clear();
        delete catalogs;
    }

    if(camera) delete camera;
    if(axis) delete axis;
    if(gui) delete gui;

    if(EQGrid) delete EQGrid;
    if(horizonGrid) delete horizonGrid;
    if(galacticGrid) delete galacticGrid;
    if(eclipticGrid) delete eclipticGrid;
    if(galactoCentricGrid) delete galactoCentricGrid;
}

void
Renderer::prepareOpenGL() {

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glViewport(0, 0, param.screenWidth, param.screenHeight);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(std::numeric_limits<GLuint>::max());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
Renderer::prepareShaders() {

    star3DShader.createShaderProgram((appDir + "shaders/star3d").c_str());
    star3DShader.use();
    star3DShader.setUniform("sizeFactor", param.sizeFactor);
    star3DShader.setUniform("sizeOffset", param.sizeOffset);
    star3DShader.setUniform("sizeSlope", param.sizeSlope);
    star3DShader.setUniform("alfaFactor", param.alfaFactor);
    star3DShader.setUniform("alfaOffset", param.alfaOffset);
    star3DShader.setUniform("alfaSlope", param.alfaSlope);
    star3DShader.setUniform("haloFactor", param.haloFactor);
    star3DShader.setUniform("haloOffset", param.haloOffset);
    star3DShader.setUniform("haloSlope",  param.haloSlope);
    star3DShader.setStarSoubroutinesIndexes();
    param.star3DShader = &star3DShader;
    param.currentShader = param.star3DShader;

    plot2DShader.createShaderProgram((appDir + "shaders/plot2D").c_str());
    plot2DShader.use();
    plot2DShader.setStarSoubroutinesIndexes();
    plot2DShader.setUniform("plot2DSizeFactor", param.plot2DSizeFactor);
    plot2DShader.setUniform("offset", param.plot2DOffset);
    plot2DShader.setUniform("fixedSize", param.fixedPointSize);
    plot2DShader.setUniform("sizeFactor", param.sizeFactor);
    plot2DShader.setUniform("sizeOffset", param.sizeOffset);
    plot2DShader.setUniform("sizeSlope", param.sizeSlope);
    plot2DShader.setUniform("alfaFactor", param.alfaFactor);
    plot2DShader.setUniform("alfaOffset", param.alfaOffset);
    plot2DShader.setUniform("alfaSlope", param.alfaSlope);
    plot2DShader.setUniform("haloFactor", param.haloFactor);
    plot2DShader.setUniform("haloOffset", param.haloOffset);
    plot2DShader.setUniform("haloSlope",  param.haloSlope);

    param.plot2DShader = &plot2DShader;

    // HR Diagram shader
    diagramHRShader.createShaderProgram((appDir + "shaders/diagramHR").c_str());
    diagramHRShader.use();
    diagramHRShader.setStarSoubroutinesIndexes();
    diagramHRShader.setUniform("diagramHRSizeFactor", param.diagramHRSizeFactor);
    diagramHRShader.setUniform("offset", param.diagramHROffset);
    diagramHRShader.setUniform("fixedSize", param.fixedPointSize);

    diagramHRShader.setUniform("sizeFactor", param.sizeFactor);
    diagramHRShader.setUniform("sizeOffset", param.sizeOffset);
    diagramHRShader.setUniform("sizeSlope", param.sizeSlope);
    diagramHRShader.setUniform("alfaFactor", param.alfaFactor);
    diagramHRShader.setUniform("alfaOffset", param.alfaOffset);
    diagramHRShader.setUniform("alfaSlope", param.alfaSlope);
    diagramHRShader.setUniform("haloFactor", param.haloFactor);
    diagramHRShader.setUniform("haloOffset", param.haloOffset);
    diagramHRShader.setUniform("haloSlope",  param.haloSlope);

    param.diagramHRShader = &diagramHRShader;

    scopeShader.createGeometryShaderProgram((appDir + "shaders/scope").c_str());

    spaceVelocityShader.createGeometryShaderProgram((appDir + "shaders/spaceVelocity").c_str());

    mouseClickShader.createShaderProgram((appDir + "shaders/mouseClick").c_str());
    mouseClickShader.use();
    mouseClickShader.setMouseClickSoubroutinesIndexes();

    mouseClick2DShader.createShaderProgram((appDir + "shaders/mouseClick2D").c_str());
    mouseClick2DShader.use();
    mouseClick2DShader.setMouseClickSoubroutinesIndexes();

    mouseClickHRShader.createShaderProgram((appDir + "shaders/mouseClickHR").c_str());
    mouseClickHRShader.use();
    mouseClickHRShader.setMouseClickSoubroutinesIndexes();

    gridShader.createShaderProgram((appDir + "shaders/grid").c_str());
    axisShader.createShaderProgram((appDir + "shaders/axis").c_str());

}

void
Renderer::prepareScene() {

    catalogs = new std::vector<StarCatalog*>();

    LoadOptions lo;
    lo.discartAppMag = true;
    lo.defaultAppMag = 0.0f;

    lo.discartColorIndex = true;
    lo.defaultColorIndex = 0.0f;

    lo.discartParallax = true;
    lo.defaultDistance = 100.0f;

    lo.onlyWithRadialVel = true;

    // CAT1
    StarCatalog* newCat = new StarCatalog();
    newCat->loadHYG((appDir + "datasets/hygdata_v3.csv").c_str());
    newCat->setCustomColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // RED
    catalogs->push_back(newCat);

    param.defaultCatalog = newCat;

    // CAT2
    newCat = new StarCatalog();
    newCat->loadGDR3((appDir + "datasets/KnownRV500K.fits").c_str(), lo);
    newCat->setCustomColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); // GREEN
    catalogs->push_back(newCat);

    param.catalogs = catalogs;
    param.activeCatalogInt = 0;

    StarCatalog* c = param.getActiveCatalog();
    star3DShader.use();
    star3DShader.setUniform("lowerAbsMagFilter", c->getLowerAbsMagFilter());
    star3DShader.setUniform("upperAbsMagFilter", c->getUpperAbsMagFilter());
    star3DShader.setUniform("lowerColorIndexFilter", c->getLowerColorIndexFilter());
    star3DShader.setUniform("upperColorIndexFilter", c->getUpperColorIndexFilter());
    star3DShader.setUniform("lowerSpeedModuleFilter", c->getLowerSpeedModuleFilter());
    star3DShader.setUniform("upperSpeedModuleFilter", c->getUpperSpeedModuleFilter());

    plot2DShader.use();
    plot2DShader.setUniform("lowerAbsMagFilter", c->getLowerAbsMagFilter());
    plot2DShader.setUniform("upperAbsMagFilter", c->getUpperAbsMagFilter());
    plot2DShader.setUniform("lowerColorIndexFilter", c->getLowerColorIndexFilter());
    plot2DShader.setUniform("upperColorIndexFilter", c->getUpperColorIndexFilter());
    plot2DShader.setUniform("lowerSpeedModuleFilter", c->getLowerSpeedModuleFilter());
    plot2DShader.setUniform("upperSpeedModuleFilter", c->getUpperSpeedModuleFilter());

    diagramHRShader.use();
    diagramHRShader.setUniform("lowerAbsMagFilter", c->getLowerAbsMagFilter());
    diagramHRShader.setUniform("upperAbsMagFilter", c->getUpperAbsMagFilter());
    diagramHRShader.setUniform("lowerColorIndexFilter", c->getLowerColorIndexFilter());
    diagramHRShader.setUniform("upperColorIndexFilter", c->getUpperColorIndexFilter());
    diagramHRShader.setUniform("lowerSpeedModuleFilter", c->getLowerSpeedModuleFilter());
    diagramHRShader.setUniform("upperSpeedModuleFilter", c->getUpperSpeedModuleFilter());

    // This must be done after init catalogs
    // IM GUI
    gui = new GUI(&param);

    glViewport(param.guiVarWidth, 0, param.screenWidth - param.guiVarWidth, param.screenHeight);

    camera -> setAspectRatio(float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));
    camera -> setFovX(glm::radians(param.displayFOV));

}

bool b = true;

void
Renderer::refreshWindow(double currentTime) {

    if (param.debugging) {
        glClearColor(1.0, 1.0, 1.0, 1.0);
    } else {
        glClearColor(0.0, 0.0, 0.0, 1.0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // FPS related
    param.fpsCount++;
    if(currentTime - param.fpsLastTimeMeasure > 0.05) {
        param.fpsCount = (int) float(param.fpsCount) * (1.0f / (currentTime - param.fpsLastTimeMeasure));
        param.fpsText = "FPS: " + std::to_string(param.fpsCount);

        if(param.fpsCount <= 30) {
            param.fpsColor = glm::mix(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), float(param.fpsCount) / 30.0f);
        } else if (param.fpsCount <= 60) {
            param.fpsColor = glm::mix(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), float(param.fpsCount - 30) / 30.0f);
        } else {
            param.fpsColor = glm::vec3(0.0f, 1.0f, 0.0f);
        }

        // Intelligent distance filter to stabilize fps
        if(param.intelligentDistanceAdjustment) {
            filter& f = param.getActiveCatalog()->getDistanceFilter();
            if(param.fpsCount < 30) {
                f.lower += (f.upper - f.lower) * 0.1f;
                param.getActiveCatalog()->getLowerDistanceFilter() = (int) f.lowerValue();
            } else if (param.fpsCount > 50) {
                f.lower -= (f.lower - 1.0f) * 0.1f;
                param.getActiveCatalog()->getLowerDistanceFilter() = (int) f.lowerValue();
            }
        }

        param.fpsLastTimeMeasure = currentTime;
        param.fpsCount = 0;
    }

    // Update time
    param.elapsedTime = currentTime - param.lastTime;
    param.lastTime = currentTime;

    if (param.fovChanged) updateFOVChanges();

    advanceTime();

    if( param.starsMovementFlag &&
        param.getActiveCatalog()->getEpochSpeedInt() != 0) {
        param.getActiveCatalog()->getEpoch() += param.getActiveCatalog()->getEpochSpeed() * param.elapsedTime;
        if(param.orbitStar != nullptr)
            camera->setOrbit(param.displayRA, param.displayDec, param.orbitDistance, param.getApropiateCartesianPos(param.orbitStar->get3DPosition(param.getActiveCatalog()->getEpoch())));
    }

    updateGridPointsCoords();
    drawGrids();

    if(param.drawStarNamesFlag && param.getActiveCatalog() == param.defaultCatalog) {
        computeShowingNames();
    }

    drawStars();
    drawAxis();

    if (param.selectedStar != nullptr) {
        updateSelectedStarScreenPos();
        drawScope();
    }

    ImGui_ImplGlfwGL3_NewFrame();
    gui -> draw();
    ImGui::Render();
}

void
Renderer::resize(int width, int height) {
    param.screenWidth = width;
    param.screenHeight = height;

    glViewport(param.guiVarWidth, 0, param.screenWidth - param.guiVarWidth, param.screenHeight);

	camera -> setAspectRatio(float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));
    camera -> setFovX(glm::radians(param.displayFOV));

    computeDragFactor();
    updateGridPointsCoords();

    plot2DShader.use();
    plot2DShader.setUniform("aspectRatio", float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));
}

void
Renderer::setViewMode(viewMode v) {
    param.setViewMode(v);
}

void
Renderer::leftClick(int x, int y) {

        if (!clicked) {
            clicked = true;

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            glClearColor(1.0, 1.0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ShaderProgram* sh = getApropiateMouseClickShader();

            sh->use();

            sh->setUniform("sizeFactor", param.sizeFactor);
            sh->setUniform("sizeOffset", param.sizeOffset);
            sh->setUniform("sizeSlope",  param.sizeSlope);

            sh->setUniform("lowerAbsMagFilter", param.getActiveCatalog()->getAbsMagFilter().lowerValue());
            sh->setUniform("upperAbsMagFilter", param.getActiveCatalog()->getAbsMagFilter().upperValue());
            sh->setUniform("lowerColorIndexFilter", param.getActiveCatalog()->getColorFilter().lowerValue());
            sh->setUniform("upperColorIndexFilter", param.getActiveCatalog()->getColorFilter().upperValue());
            sh->setUniform("lowerSpeedModuleFilter", param.getActiveCatalog()->getVelocityFilter().lowerValue());
            sh->setUniform("upperSpeedModuleFilter", param.getActiveCatalog()->getVelocityFilter().upperValue());

            if(param.activeRenderMode == SPACE_3D) {
                sh->setUniform("mModelViewProj",
                               getApropiateMatrix());
                sh->setUniform("cameraPos", getApropiateCameraPos());
            } else if (param.activeRenderMode == PLOT_2D) {
                sh->setUniform("aspectRatio", float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));
                sh->setUniform("plot2DSizeFactor", param.plot2DSizeFactor);
                sh->setUniform("offset", param.plot2DOffset);
                sh->setUniform("fixedSize", param.fixedPointSize);
            } else if(param.activeRenderMode == HR_DIAGRAM) {
                sh->setUniform("aspectRatio", float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));
                sh->setUniform("diagramHRSizeFactor", param.diagramHRSizeFactor);
                sh->setUniform("offset", param.diagramHROffset);
                sh->setUniform("fixedSize", param.fixedPointSize);
            }

            setSubroutinesForMouseClickShader(sh);

            param.getActiveCatalog()->draw(param.activeRenderMode, sh, camera->getViewProjectionMatrix());

            glFlush();

            // In theory we must call glFinish() before reading from the framebuffer
            // but on Windows that call completely destroys the performance
            // so we'll only make it on Linux
#ifdef __linux__
            //std::cout << "Calling to glFinish()\n";
            glFinish();
#elif _WIN32
            //std::cout << "NOT calling to glFinish()\n";
#endif

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glm::vec4 getColor;
            glReadPixels(x, param.screenHeight - y, 1, 1, GL_RGBA, GL_FLOAT, &getColor);

            getColor.w = 0.0f;
            unsigned int pickedID = glm::packUnorm4x8(getColor);

            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // If ID is 16777215 the picked color is white so no star clicked
            if (pickedID != 16777215) {

                for (int i = 0; i < catalogs->size(); ++i) {
                    if (catalogs->operator[](i)->isIdInCatalog(pickedID)) {

                        if (param.selectedStar != nullptr &&
                            param.selectedStarStored == catalogs->operator[](i)->getStarWithId(pickedID)) {

                            param.orbitStarStored = param.selectedStarStored;
                            param.orbitStar = &param.orbitStarStored;
                            param.switchToOrbitStar();

                        }

                        param.selectedStarStored = catalogs->operator[](i)->getStarWithId(pickedID);
                        param.selectedStar = &param.selectedStarStored;
                        // Update text
                        param.selectedStarInfoText = param.selectedStar->getInfoText();
                    }
                }

                param.selectedStarPos.x = x;
                param.selectedStarPos.y = y;
                updateSelectedStarScreenPos();

            } else {

                if (param.orbitStar != nullptr) {
                    param.orbitToFreeCamera();
                    param.orbitStar = nullptr;
                    param.selectedStar = nullptr;
                }
                param.selectedStar = nullptr;

            }

            param.switchDepthTest(); // We only want to use depth test on mouse clicking

            clicked = false;
        }

}

void
Renderer::drawScope() {

    //get xy star screen coords
    glm::vec3 vectorcito = param.getApropiateCartesianPos(param.selectedStar->get3DPosition(param.getActiveCatalog()->getEpoch()));

    vectorcito = glm::vec4(vectorcito, 1.0f);

    glm::vec3 proj = glm::project(vectorcito, camera -> getViewMatrix(), camera->getProjectionMatrix(), glm::vec4(0, 0, 2.0f, 2.0f));

    if(param.activeRenderMode == PLOT_2D) {
        float x;
        if(param.selectedStar->position.x > glm::pi<float>()) {
            x = ((-param.selectedStar->position.x + glm::pi<float>()) * 0.15915494309189533 * 2.0f) + 1.0f;
        } else {
            x = ((-param.selectedStar->position.x - glm::pi<float>()) * 0.15915494309189533 * 2.0f) + 1.0f;
        }

        float y = param.selectedStar->position.y * 0.63661977236758134 * 0.5f * (float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));

        x += param.plot2DOffset.x;
        y += param.plot2DOffset.y;

        x *= param.plot2DSizeFactor;
        y *= param.plot2DSizeFactor;

        proj.x = x + 1.0f;
        proj.y = y + 1.0f;
        proj.z = 1.0f;
    } else if(param.activeRenderMode == HR_DIAGRAM) {
        float x = param.selectedStar->colorIndex * 0.1;
        float y = -param.selectedStar->absMag * (float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight)) * 0.3f * 0.1f;

        x += param.diagramHROffset.x;
        y += param.diagramHROffset.y;

        x *= param.diagramHRSizeFactor;
        y *= param.diagramHRSizeFactor;

        proj.x = x + 1.0f;
        proj.y = y + 1.0f;
        proj.z = 1.0f;
    }

    if(proj.z <= 1.0f && proj.x > 0.0f && proj.x < 2.0f && proj.y > 0.0f && proj.y < 2.0f) {

        param.isSelectedStarVisible = true;
        float x = proj.x;
        float y = proj.y;
        x -= 1.0f;
        y -= 1.0f;

        scopeShader.use();
        scopeShader.setUniform("pos", glm::vec2(x, y));
        scopeShader.setUniform("aspectRatio", float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));
        scopeShader.setUniform("radius", (getStarRadius(param.selectedStar, param.getActiveCatalog()->getEpoch()) + 3.0f) / float(param.screenWidth - param.guiVarWidth));
        param.getActiveCatalog()->nullDraw();
    } else {
        param.isSelectedStarVisible = false;
    }
}

void
Renderer::switchGrid() {
    EQGrid -> setIsVisible(!(EQGrid -> isVisible));
}

void
Renderer::movePan(float angle) {
    camera -> pan(angle);
}

void
Renderer::moveTilt(float angle) {
    camera -> tilt(angle);
}

void
Renderer::moveOrbitY(float angle) {
    camera -> orbitY(angle);
}

void
Renderer::moveOrbitU(float angle) {
    camera -> orbitU(angle);
}

void
Renderer::moveTruck(float distance) {
    camera -> truck(distance);
}

void
Renderer::moveDolly(float distance){
    camera -> dolly(distance);
}

void
Renderer::moveTruck(bool direction) {
    if(param.activeViewMode == FREE_CAMERA) {
        if(direction) {
            camera->truck(param.elapsedTime * param.cameraSpeed);
        }
        else {
            camera->truck(-param.elapsedTime * param.cameraSpeed);
        }
    }
}

void
Renderer::moveDolly(bool direction){
    if(param.activeViewMode == FREE_CAMERA) {
        if(direction) {
            camera->dolly(param.elapsedTime * param.cameraSpeed);
        }
        else {
            camera->dolly(-param.elapsedTime * param.cameraSpeed);
        }
    }
}

void
Renderer::moveCrane(float distance){
    camera -> crane(distance);
}

void
Renderer::changeZoom(float zoom) {
    camera -> zoom(zoom);
}

void
Renderer::adjustTime(double currentTime) {
    param.elapsedTime = currentTime - param.lastTime;
    param.lastTime = currentTime;
    computeFPSText();
}

void
Renderer::computeFPSText() {
    param.fpsText = "FPS: " + std::to_string((int) (1.0f / param.elapsedTime));
}

void
Renderer::computeAltAzText() {
    int h, m;
    float s;
    float ra = (param.displayRA / 15.0f);
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

    float dec = glm::abs(param.displayDec);

    h = (int) std::trunc(dec);
    m = (int) std::trunc((dec - std::trunc(dec)) * 60.0f);
    s = ((dec - std::trunc(dec)) * 60.0f - float(m)) * 60.0f;

    std::string decString = " ";
    if(param.displayDec < 0.0f) decString = "-";
    if(h < 10) decString = " " + decString;
    decString += std::to_string(h) + "º ";
    if(m < 10) decString += " ";
    decString += std::to_string(m) + "' ";
    if(s < 10.0f) decString += " ";
    decString += std::to_string(s) + "\"";

    param.raDecText = " RA: " + raString + "\nDEC: " + decString;
}

void
Renderer::computeRaDecText() {
    int h, m;
    float s;
    float ra = (glm::degrees(param.displayRA) / 15.0f);
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

    float dec = glm::abs(glm::degrees(param.displayDec));

    h = (int) std::trunc(dec);
    m = (int) std::trunc((dec - std::trunc(dec)) * 60.0f);
    s = ((dec - std::trunc(dec)) * 60.0f - float(m)) * 60.0f;

    std::string decString = " ";
    if(param.displayDec < 0.0f) decString = "-";
    if(h < 10) decString = " " + decString;
    decString += std::to_string(h) + "º ";
    if(m < 10) decString += " ";
    decString += std::to_string(m) + "' ";
    if(s < 10.0f) decString += " ";
    decString += std::to_string(s) + "\"";

    param.raDecText = " RA: " + raString + "\nDEC: " + decString;
}

void
Renderer::changeRA(float angle) {
    param.displayRA += angle;
    if(param.displayRA > 360.0f) param.displayRA = param.displayRA - 360.0f;
    if(param.displayRA < 0.0f) param.displayRA = 360.0f + param.displayRA;

    camera -> setLookAt(param.displayRA, param.displayDec);
}

void
Renderer::changeDec(float angle) {
    param.displayDec += angle;
    if(param.displayDec > 88.0f) param.displayDec = 88.0f;
    if(param.displayDec < -88.0f) param.displayDec = -88.0f;

    camera -> setLookAt(param.displayRA, param.displayDec);
}

void
Renderer::computeDragFactor() {
    param.dragFactor = glm::radians(param.displayFOV) / float(param.screenWidth - param.guiVarWidth);
}

glm::mat4
Renderer::getPlaneMatrix() {
    return param.activePlaneMatrix;
}

glm::mat4
Renderer::getTimeMatrix() {
    float hourAngle = param.time;
    if(hourAngle >= glm::two_pi<float>()) hourAngle = hourAngle - glm::two_pi<float>();
    if(hourAngle < 0.0f) hourAngle = glm::two_pi<float>() + hourAngle;

    return glm::rotate(glm::mat4(1.0f), -hourAngle, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4
Renderer::getEarthPosMatrix() {

    float hourAngle = param.time + glm::radians(param.longitude);
    if(hourAngle >= glm::two_pi<float>()) hourAngle = hourAngle - glm::two_pi<float>();
    if(hourAngle < 0.0f) hourAngle = glm::two_pi<float>() + hourAngle;

    return  glm::rotate(glm::mat4(1.0f), glm::radians(param.latitude - 90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::rotate(glm::mat4(1.0f), -hourAngle, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3
Renderer::getApropiateCameraPos() {
    // If we are not on Equatorial fundamental plane
    // we must apply an inverse transformation to
    // our camera position because aparent magnitude
    // will be computed from EQ coordinates in the
    // vertex shader. The camera position is always
    // considered to be representing coords in the
    // active fundamental plane.
    return glm::inverse(getTimeMatrix()) * param.activePlaneInverseMatrix * glm::vec4(camera->getPosition(), 1.0f);
}

float
Renderer::getStarRadius(Star* s, float epoch) {
    float size;
    if(param.useFixedPointSize) {
        size = param.fixedPointSize;
    } else {
        if(param.activeRenderMode == SPACE_3D) {
            float dist = glm::length(param.camera->getPosition() - param.getApropiateCartesianPos(s->get3DPosition(epoch)));
            float appMag = 5.0f * (log(dist / 10.0f) / log(10.0f)) + s->absMag;
            if(appMag - param.sizeOffset < 0.0f) {
                size = param.sizeFactor;
            } else {
                size = param.sizeFactor * pow(glm::e<float>(), -pow((appMag - param.sizeOffset),2.0f)/(2.0f * pow(param.sizeSlope, 2.0f))) + 2.2;
            }
        } else if(param.activeRenderMode == PLOT_2D || param.activeRenderMode == HR_DIAGRAM) {
            float dist = glm::length(s->get3DPosition());
            float appMag = 5.0f * (log(dist / 10.0f) / log(10.0f)) + s->absMag;
            if(appMag - param.sizeOffset < 0.0f) {
                size = param.sizeFactor;
            } else {
                size = param.sizeFactor * pow(glm::e<float>(), -pow((appMag - param.sizeOffset),2.0f)/(2.0f * pow(param.sizeSlope, 2.0f))) + 2.2;
            }
        }
    }
    return size;
}

float
Renderer::magToRadius(float mag) {
    Star s(0.0f, 0.0f, 0.0f, mag, hexToRGB("0xffffff"), 0.0f, 0.0f, 0.0f, 0.0f, true);
    return 2.0f;
}

void
Renderer::modifyMagFilter(float val) {
    param.magFilter += val;
    float min = -1.44f, max = 21.0f;
    if(param.magFilter > max) param.magFilter = max;
    if(param.magFilter < min) param.magFilter = min;
}

void
Renderer::wheelMove(bool up) {

    if(param.activeRenderMode == SPACE_3D) {

        if(param.activeViewMode == ORBIT) {
            if(up) {
                param.orbitDistance *= 0.9f;
            } else {
                param.orbitDistance *= 1.1f;
            }

            if(param.orbitStar != nullptr) {
                camera->setOrbit(param.displayRA, param.displayDec, param.orbitDistance, param.getApropiateCartesianPos(param.orbitStar->get3DPosition(param.getActiveCatalog()->getEpoch())));
            } else {
                camera -> setPosition(param.displayRA, param.displayDec, param.orbitDistance);
            }
        }

        if(param.activeViewMode == CENTERED_ON_ORIGIN || param.activeViewMode == HORIZON_VIEW) {
            if(up) {
                param.displayFOV *= 0.9f;
            } else {
                param.displayFOV *= 1.1f;
            }
            if(param.displayFOV > 180.0f) param.displayFOV = 180.0f;
            param.fovChanged = true;
        }

        if(param.activeViewMode == FREE_CAMERA) {
            if(up) {
                param.cameraSpeed *= 1.15f;
                if(param.cameraSpeed > 100000.0f) param.cameraSpeed = 100000.0f;
            } else {
                param.cameraSpeed *= 0.85f;
            }
        }

    } else if(param.activeRenderMode == PLOT_2D) {

        if(up) {
            param.plot2DSizeFactor *= 1.1f;
        } else {
            param.plot2DSizeFactor *= 0.9f;
        }

        plot2DShader.use();
        plot2DShader.setUniform("plot2DSizeFactor", param.plot2DSizeFactor);

    } else if(param.activeRenderMode == HR_DIAGRAM) {
        if(up) {
            param.diagramHRSizeFactor *= 1.1f;
        } else {
            param.diagramHRSizeFactor *= 0.9f;
        }
        diagramHRShader.use();
        diagramHRShader.setUniform("diagramHRSizeFactor", param.diagramHRSizeFactor);
    }

}

void
Renderer::updateGridPointsCoords() {

    if(param.activeViewMode == CENTERED_ON_ORIGIN == param.activeRenderMode == SPACE_3D) {

        if(param.activePlane != GALACTOCENTRIC) {

            if (param.EQGrid->isVisible) {
                EQGrid->computeGridPointsCoords(param.displayRA, param.displayDec, param.activePlane,
                                                param.getMatrixForPlane(param.activePlane) * getTimeMatrix(), camera->getViewMatrix(),
                                                camera->getProjectionMatrix(), param.screenWidth - param.guiVarWidth,
                                                param.screenHeight);
            }

            if (param.galacticGrid->isVisible) {
                galacticGrid->computeGridPointsCoords(param.displayRA, param.displayDec, param.activePlane,
                                                      param.getMatrixForPlane(param.activePlane) * getTimeMatrix(),
                                                      camera->getViewMatrix(), camera->getProjectionMatrix(),
                                                      param.screenWidth - param.guiVarWidth, param.screenHeight);
            }

            if (param.eclipticGrid->isVisible) {
                eclipticGrid->computeGridPointsCoords(param.displayRA, param.displayDec, param.activePlane,
                                                      param.getMatrixForPlane(param.activePlane) * getTimeMatrix(),
                                                      camera->getViewMatrix(), camera->getProjectionMatrix(),
                                                      param.screenWidth - param.guiVarWidth, param.screenHeight);
            }

            if (param.horizonGrid->isVisible && param.activePlane == HORIZON_VIEW) {
                horizonGrid->computeGridPointsCoords(param.displayRA, param.displayDec, param.activePlane,
                                                     param.getMatrixForPlane(param.activePlane),
                                                     camera->getViewMatrix(), camera->getProjectionMatrix(),
                                                     param.screenWidth - param.guiVarWidth, param.screenHeight);
            }
        } else {

            if (param.galactoCentricGrid->isVisible) {
                param.galactoCentricGrid->computeGridPointsCoords(param.displayRA, param.displayDec, param.activePlane,
                                                     param.getMatrixForPlane(param.activePlane),
                                                     camera->getViewMatrix(), camera->getProjectionMatrix(),
                                                     param.screenWidth - param.guiVarWidth, param.screenHeight);
            }

        }
    }

}

void
Renderer::mouseDrag(float x, float y) {

    if(param.activeRenderMode == SPACE_3D) {

        if(param.activeViewMode == ORBIT && param.orbitStar != nullptr) {
            param.displayRA += x * param.dragFactor;
            param.displayDec += -y * param.dragFactor;
        } else {
            param.displayRA += -x * param.dragFactor;
            param.displayDec += -y * param.dragFactor;
        }

        param.displayRA = glm::mod(param.displayRA, glm::two_pi<float>());

        if(glm::abs(param.displayDec) > glm::half_pi<float>()) {
            if(param.displayDec < 0.0f) {
                param.displayDec = -glm::half_pi<float>();
            } else {
                param.displayDec = glm::half_pi<float>();
            }
        }

        if(param.activeViewMode == FREE_CAMERA) {
            // If we emulate free camera we must change camera lookAt
            // camera lookAt will be stored as alfa delta polar coordrs
            camera->setRaLootAk(-param.displayRA);
            camera->setLookAtForFreeCamera(param.displayRA, param.displayDec);
        }

        if(param.activeViewMode == ORBIT) {

            if(param.orbitStar != nullptr) {
                camera->setRaLootAk(glm::mod(-param.displayRA + glm::pi<float>(),glm::two_pi<float>()));
                camera->setOrbit(param.displayRA, param.displayDec, param.orbitDistance, param.getApropiateCartesianPos(param.orbitStar->get3DPosition(param.getActiveCatalog()->getEpoch())));
            } else {
                // If we orbit we do this
                camera->setRaLootAk(glm::mod(param.displayRA + glm::pi<float>(),glm::two_pi<float>()));
                camera -> setPosition(param.displayRA, param.displayDec, param.orbitDistance);
            }
        }

        if(param.activeViewMode == CENTERED_ON_ORIGIN || param.activeViewMode == HORIZON_VIEW) {
            camera -> setLookAtFromPolarCoords(param.displayRA, param.displayDec);
            computeRaDecText();
        }

        updateGridPointsCoords();

    } else if(param.activeRenderMode == PLOT_2D) {
        param.plot2DOffset.x += (1.0f / param.plot2DSizeFactor) * -2.0f * x / (param.screenWidth - param.guiVarWidth);
        param.plot2DOffset.y += (1.0f / param.plot2DSizeFactor) * 2.0f * y / param.screenHeight;

        plot2DShader.use();
        plot2DShader.setUniform("offset", param.plot2DOffset);

    } else if(param.activeRenderMode == HR_DIAGRAM) {
        param.diagramHROffset.x += (1.0f / param.diagramHRSizeFactor) * -2.0f * x / (param.screenWidth - param.guiVarWidth);
        param.diagramHROffset.y += (1.0f / param.diagramHRSizeFactor) * 2.0f * y / param.screenHeight;

        diagramHRShader.use();
        diagramHRShader.setUniform("offset", param.diagramHROffset);
    }
}

void
Renderer::updateFOVChanges() {
    camera -> setFovX(glm::radians(param.displayFOV));

    computeDragFactor();
    updateGridPointsCoords();

    param.fovChanged = false;
}

void
Renderer::changeFOV(float angle) {
    param.displayFOV += angle;
    if(param.displayFOV > 1000.0f) param.displayFOV = 1000.0f;
    if(param.displayFOV < 0.0f) param.displayFOV = 0.0f;

    updateFOVChanges();
}

void
Renderer::drawGrids() {
    if(param.activeViewMode == CENTERED_ON_ORIGIN && param.activeRenderMode == SPACE_3D) {
        gridShader.use();

        if(param.activePlane != GALACTOCENTRIC) {

            if(EQGrid->isVisible) {
                gridShader.setUniform("color", EQGrid->color);
                gridShader.setUniform("mModelViewProj", camera->getViewProjectionMatrix() * param.getMatrixForPlane(param.activePlane) * getTimeMatrix());
                EQGrid->draw();
            }

            if(galacticGrid->isVisible) {
                gridShader.setUniform("color", galacticGrid->color);
                gridShader.setUniform("mModelViewProj", camera->getViewProjectionMatrix() * param.getMatrixForPlane(param.activePlane) * getTimeMatrix() * param.galToEq);
                galacticGrid->draw();
            }

            if(eclipticGrid->isVisible) {
                gridShader.setUniform("color", eclipticGrid->color);
                gridShader.setUniform("mModelViewProj", camera->getViewProjectionMatrix() * param.getMatrixForPlane(param.activePlane) * getTimeMatrix() * param.eclToEq);
                eclipticGrid->draw();
            }

            if(horizonGrid->isVisible && param.activePlane == HORIZON_VIEW) {
                gridShader.setUniform("color", horizonGrid->color);
                gridShader.setUniform("mModelViewProj", camera->getViewProjectionMatrix());
                horizonGrid->draw();
            }

        } else {
            if(galactoCentricGrid->isVisible) {
                gridShader.setUniform("color", galactoCentricGrid->color);
                gridShader.setUniform("mModelViewProj", camera->getViewProjectionMatrix());
                galactoCentricGrid->draw();
            }
        }
    }

}

void
Renderer::drawAxis() {
    if(param.activeRenderMode == SPACE_3D) {
        axisShader.use();
        axisShader.setUniform("mModelViewProj", camera -> getViewProjectionMatrix());
        axis -> draw();
    }
}

float
Renderer::getTimeSpeed() {
    switch (param.speedMode) {
        case 0:
            return 0.0f; // No move
        case 1:
            return 1.0f; // 1 second per second
        case 2:
            return 10.0f; // 10 seconds per second
        case 3:
            return 60.0f; // 1 minute per second
        case 4:
            return 600.0f; // 10 minutes per second
        case 5:
            return 1800.0f; // 30 minutes per second
        case 6:
            return 3600.0f; // 1 hour per second
        default:
            return 0.0f;
    }
}

void
Renderer::advanceTime() {
    param.time += getTimeSpeed() * param.anglePerSecond * param.elapsedTime;
    if(param.time >= glm::two_pi<float>()) param.time -= glm::two_pi<float>();
}

void
Renderer::switchGaia() {
    param.drawGaiaCatalogFlag = !param.drawGaiaCatalogFlag;
}

void
Renderer::switchHip() {
    param.drawHipCatalogFlag = !param.drawHipCatalogFlag;
}

glm::mat4
Renderer::getApropiateMatrix() {
    return camera->getViewProjectionMatrix() * getPlaneMatrix() * getTimeMatrix();
}

void
Renderer::drawStars() {

    if(param.activeRenderMode == SPACE_3D) {

        ShaderProgram *sh;

        sh = &star3DShader;
        sh->use();
        sh->setUniform("mModelViewProj", getApropiateMatrix());
        sh->setUniform("cameraPos", getApropiateCameraPos());
        setSubroutines();

        if (param.debugging) {

            ShaderProgram* clickShader = getApropiateMouseClickShader();

            clickShader->use();

            clickShader->setUniform("mModelViewProj",
                                    getApropiateMatrix());
            clickShader->setUniform("cameraPos", getApropiateCameraPos());

            clickShader->setUniform("lowerAbsMagFilter", param.getActiveCatalog()->getAbsMagFilter().lowerValue());
            clickShader->setUniform("upperAbsMagFilter", param.getActiveCatalog()->getAbsMagFilter().upperValue());
            clickShader->setUniform("lowerColorIndexFilter", param.getActiveCatalog()->getColorFilter().lowerValue());
            clickShader->setUniform("upperColorIndexFilter", param.getActiveCatalog()->getColorFilter().upperValue());
            clickShader->setUniform("lowerSpeedModuleFilter", param.getActiveCatalog()->getVelocityFilter().lowerValue());
            clickShader->setUniform("upperSpeedModuleFilter", param.getActiveCatalog()->getVelocityFilter().upperValue());


            clickShader->setUniform("sizeFactor", param.sizeFactor);
            clickShader->setUniform("sizeOffset", param.sizeOffset);
            clickShader->setUniform("sizeSlope",  param.sizeSlope);

            setSubroutinesForMouseClickShader(clickShader);
            param.getActiveCatalog()->draw(param.activeRenderMode, clickShader, camera->getViewProjectionMatrix());
        } else {
            param.getActiveCatalog()->draw(param.activeRenderMode, sh, camera->getViewProjectionMatrix());
        }


        if (param.drawVelocitiesFlag) {
            spaceVelocityShader.use();
            spaceVelocityShader.setUniform("mModelViewProj",
                                           getApropiateMatrix());
            spaceVelocityShader.setUniform("cameraPos", getApropiateCameraPos());
            spaceVelocityShader.setUniform("epoch", param.getActiveCatalog()->getEpoch());
            spaceVelocityShader.setUniform("distanceLimit", param.vectorDistanceLimit);

            StarCatalog* c = param.getActiveCatalog();
            spaceVelocityShader.setUniform("lowerAbsMagFilter", c->getAbsMagFilter().lowerValue());
            spaceVelocityShader.setUniform("upperAbsMagFilter", c->getAbsMagFilter().upperValue());
            spaceVelocityShader.setUniform("lowerColorIndexFilter", c->getColorFilter().lowerValue());
            spaceVelocityShader.setUniform("upperColorIndexFilter", c->getColorFilter().upperValue());
            spaceVelocityShader.setUniform("lowerSpeedModuleFilter", c->getVelocityFilter().lowerValue());
            spaceVelocityShader.setUniform("upperSpeedModuleFilter", c->getVelocityFilter().upperValue());

            param.getActiveCatalog()->drawVelocities();
        }


    } else if(param.activeRenderMode == PLOT_2D) {
        ShaderProgram *sh;

        sh = &plot2DShader;
        sh->use();
        sh->setUniform("aspectRatio", float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));

        setSubroutines();

        if(param.debugging) {
            ShaderProgram* clickShader = getApropiateMouseClickShader();

            clickShader->use();

            clickShader->setUniform("sizeFactor", param.sizeFactor);
            clickShader->setUniform("sizeOffset", param.sizeOffset);
            clickShader->setUniform("sizeSlope",  param.sizeSlope);

            clickShader->setUniform("lowerAbsMagFilter", param.getActiveCatalog()->getAbsMagFilter().lowerValue());
            clickShader->setUniform("upperAbsMagFilter", param.getActiveCatalog()->getAbsMagFilter().upperValue());
            clickShader->setUniform("lowerColorIndexFilter", param.getActiveCatalog()->getColorFilter().lowerValue());
            clickShader->setUniform("upperColorIndexFilter", param.getActiveCatalog()->getColorFilter().upperValue());
            clickShader->setUniform("lowerSpeedModuleFilter", param.getActiveCatalog()->getVelocityFilter().lowerValue());
            clickShader->setUniform("upperSpeedModuleFilter", param.getActiveCatalog()->getVelocityFilter().upperValue());

            clickShader->setUniform("aspectRatio", float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));
            clickShader->setUniform("plot2DSizeFactor", param.plot2DSizeFactor);
            clickShader->setUniform("offset", param.plot2DOffset);
            clickShader->setUniform("fixedSize", param.fixedPointSize);

            setSubroutinesForMouseClickShader(clickShader);
            param.getActiveCatalog()->draw(param.activeRenderMode, clickShader, camera->getViewProjectionMatrix());

        } else {
            param.getActiveCatalog()->draw(param.activeRenderMode, sh, camera->getViewProjectionMatrix());
        }

    } else if(param.activeRenderMode == HR_DIAGRAM) {
        ShaderProgram *sh;

        sh = &diagramHRShader;
        sh->use();
        sh->setUniform("aspectRatio", float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));

        setSubroutines();

        if(param.debugging) {
            ShaderProgram* clickShader = getApropiateMouseClickShader();

            clickShader->use();

            clickShader->setUniform("sizeFactor", param.sizeFactor);
            clickShader->setUniform("sizeOffset", param.sizeOffset);
            clickShader->setUniform("sizeSlope",  param.sizeSlope);

            clickShader->setUniform("lowerAbsMagFilter", param.getActiveCatalog()->getAbsMagFilter().lowerValue());
            clickShader->setUniform("upperAbsMagFilter", param.getActiveCatalog()->getAbsMagFilter().upperValue());
            clickShader->setUniform("lowerColorIndexFilter", param.getActiveCatalog()->getColorFilter().lowerValue());
            clickShader->setUniform("upperColorIndexFilter", param.getActiveCatalog()->getColorFilter().upperValue());
            clickShader->setUniform("lowerSpeedModuleFilter", param.getActiveCatalog()->getVelocityFilter().lowerValue());
            clickShader->setUniform("upperSpeedModuleFilter", param.getActiveCatalog()->getVelocityFilter().upperValue());

            clickShader->setUniform("aspectRatio", float(param.screenWidth - param.guiVarWidth) / float(param.screenHeight));
            clickShader->setUniform("diagramHRSizeFactor", param.diagramHRSizeFactor);
            clickShader->setUniform("offset", param.diagramHROffset);
            clickShader->setUniform("fixedSize", param.fixedPointSize);


            setSubroutinesForMouseClickShader(clickShader);
            param.getActiveCatalog()->draw(param.activeRenderMode, clickShader, camera->getViewProjectionMatrix());

        } else {
            param.getActiveCatalog()->draw(param.activeRenderMode, sh, camera->getViewProjectionMatrix());
        }
    }
}

void
Renderer::computeShowingNames() {
    param.showingNames.clear();

    std::string name;
    Star s;

    for(unsigned int i = 0; i < param.defaultCatalog->getStarsWithName().size(); ++i) {

        Star s = param.defaultCatalog->getStarsWithName()[i].star;
        name = param.defaultCatalog->getStarsWithName()[i].name;

        glm::vec3 pos = param.getApropiateCartesianPos(s.get3DPosition(param.getActiveCatalog()->getEpoch()));


        glm::vec3 proj = glm::project(pos, camera -> getViewMatrix(), camera->getProjectionMatrix(), glm::vec4(0, 0, param.screenWidth - param.guiVarWidth, param.screenHeight));

        if(proj.z <= 1.0f && proj.x > 0.0f && proj.x < param.screenWidth - param.guiVarWidth && proj.y > 0.0f && proj.y < param.screenHeight) {
            param.showingNames.push_back(NameText(name, glm::vec2(proj.x, proj.y)));
        }
    }
}

void
Renderer::updateSelectedStarScreenPos() {

    if(param.activeRenderMode == SPACE_3D) {
        glm::vec3 pos = param.getApropiateCartesianPos(param.selectedStar->get3DPosition(param.getActiveCatalog()->getEpoch()));

        glm::vec3 proj = glm::project(pos, camera->getViewMatrix(), camera->getProjectionMatrix(), glm::vec4(0, 0, param.screenWidth - param.guiVarWidth, param.screenHeight));

        if(proj.z <= 1.0f && proj.x > 0.0f && proj.x < param.screenWidth - param.guiVarWidth && proj.y > 0.0f && proj.y < param.screenHeight) {
            param.selectedStarPos.x = proj.x;
            param.selectedStarPos.y = param.screenHeight - proj.y;
            param.isSelectedStarVisible = true;
        } else {
            param.isSelectedStarVisible = false;
        }
    }
}

void
Renderer::changeMode() {
    mode = !mode;
}

ShaderProgram*
Renderer::getApropiateMouseClickShader() {
    if(param.activeRenderMode == SPACE_3D) {
        return &mouseClickShader;
    } else if(param.activeRenderMode == PLOT_2D) {
        return &mouseClick2DShader;
    } else if(param.activeRenderMode == HR_DIAGRAM) {
        return &mouseClickHRShader;
    }
}

void
Renderer::setSubroutines() {

    ShaderProgram* sh;
    if(param.activeRenderMode == SPACE_3D) {
        sh = &star3DShader;
    } else if(param.activeRenderMode == PLOT_2D) {
        sh = &plot2DShader;
    } else if(param.activeRenderMode == HR_DIAGRAM) {
        sh = &diagramHRShader;
    }

    GLuint sizeIndex;
    GLuint alfaIndex;
    GLuint haloIndex;

    if(param.useFixedPointSize) {
        sizeIndex = glGetSubroutineIndex(sh->getHandler(), GL_VERTEX_SHADER, "fixedSizeFunc");
        sh->setUniform("fixedSize", param.fixedPointSize);
    } else {
        sizeIndex = glGetSubroutineIndex(sh->getHandler(), GL_VERTEX_SHADER, "distanceBasedSizeFunc");
    }

    if(!param.doNotAttenuate) {
        alfaIndex = glGetSubroutineIndex(sh->getHandler(), GL_VERTEX_SHADER, "distanceBasedAlfa");
    } else {
        alfaIndex = glGetSubroutineIndex(sh->getHandler(), GL_VERTEX_SHADER, "noAlfa");
    }

    if(param.doNotDrawHalo) {
        haloIndex = glGetSubroutineIndex(sh->getHandler(), GL_FRAGMENT_SHADER, "noHalo");
    } else {
        haloIndex = glGetSubroutineIndex(sh->getHandler(), GL_FRAGMENT_SHADER, "getHalo");
    }

    sh->applyStarSubroutines(sizeIndex, alfaIndex, haloIndex);
}

void
Renderer::setSubroutinesForMouseClickShader(ShaderProgram* sh) {

    GLuint sizeIndex;
    GLuint alfaIndex;

    if(param.useFixedPointSize) {
        sizeIndex = glGetSubroutineIndex(sh->getHandler(), GL_VERTEX_SHADER, "fixedSizeFunc");
        sh->setUniform("fixedSize", param.fixedPointSize);
    } else {
        sizeIndex = glGetSubroutineIndex(sh->getHandler(), GL_VERTEX_SHADER, "distanceBasedSizeFunc");
    }
    sh->applyMouseClickSubroutines(sizeIndex);

}

void
Renderer::resetCameraPosition() {
    param.camera->setPosition(glm::vec3(0, 0, 0));
    param.camera->setLookAt(0.0f, 0.0f);
    param.orbitDistance = 1.0f;
}
