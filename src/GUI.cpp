#include "GUI.h"

const char *GUI::filtersList[]{"Distance",          // 0
                               "Absolute Magnitude",// 1
                               //"Aparent Magnitude", // 2 ya no
                               "B-V Index", // 2
                               "Speed"};        // 3

const char *GUI::planesList[]{"Equatorial",
                              "Ecliptic",
                              "Galactic",
                              "Galactocentric",
                              "Horizon View"};

const char *GUI::speedsList[]{"STOP",
                         "1 s/s",
                         "10 s/s",
                         "1 m/s",
                         "10 m/s",
                         "30 m/s",
                         "1 h/s"};

const char *GUI::viewModeList[]{    "Centered on origin",
                                    "Free camera view",
                                    "Orbit"};

const char *GUI::renderModeList[]{  "3D Space",
                                    "2D Plot",
                                    "HR Diagram"};

const char *GUI::visualizationModeList[]{   "Attenuation alfa",
                                            "Point size"};

void
GUI::updateEpochSpeed() {
    if(param->getActiveCatalog()->getEpochSpeedInt() != 0) {
        float speed = glm::pow(10.0f, glm::abs(param->getActiveCatalog()->getEpochSpeedInt()));
        if(param->getActiveCatalog()->getEpochSpeedInt() < 0) speed = -speed;
        param->getActiveCatalog()->getEpochSpeed() = speed;
    }
    else {
        param->getActiveCatalog()->getEpochSpeed() = 0.0f;
    }
}

glm::vec2 // x lower, y upper
GUI::getLowerUpperLimits(unsigned int &offset) { // returns the lower-upper values of the active filter
    glm::vec2 limits;
    if(activeFilter == 0) {
        limits.x = param->getActiveCatalog()->getLowerDistanceFloatFilter();
        limits.y = param->getActiveCatalog()->getUpperDistanceFloatFilter();
        offset = offsetof(Star, distance);
    }else if(activeFilter == 1) {
        limits.x = param->getActiveCatalog()->getAbsMagFilter().lowerValue();
        limits.y = param->getActiveCatalog()->getAbsMagFilter().upperValue();
        offset = offsetof(Star, absMag);
    } else if(activeFilter == 2) {
        limits.x = param->getActiveCatalog()->getColorFilter().lowerValue();
        limits.y = param->getActiveCatalog()->getColorFilter().upperValue();
        offset = offsetof(Star, colorIndex);
    } else if(activeFilter == 3) {
        limits.x = param->getActiveCatalog()->getVelocityFilter().lowerValue();
        limits.y = param->getActiveCatalog()->getVelocityFilter().upperValue();
        offset = offsetof(Star, speedModule);
    }
    return limits;
}



void
GUI::showSpectralClassRange() {
    ImVec4 selectedColor = ImVec4(0.0f, 0.7f, 1.0f, 1.0f);
    ImVec4 unselectedColor = ImVec4(0.42f, 0.67f, 1.0f, 0.6f);

    filter& f = param->getActiveCatalog()->getColorFilter();

    ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);


    O.draw(f); ImGui::SameLine();
    B.draw(f); ImGui::SameLine();
    A.draw(f); ImGui::SameLine();
    F.draw(f); ImGui::SameLine();
    G.draw(f); ImGui::SameLine();
    K.draw(f); ImGui::SameLine();
    M.draw(f); ImGui::SameLine();
    L.draw(f); ImGui::SameLine();
    T.draw(f); ImGui::SameLine();
    Y.draw(f);

    ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.42f, 0.67f, 1.0f, 0.6f);
    ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.7f, 1.0f, 1.0f);
}

std::string
GUI::getEpochText() {
    return "Epoch: " + std::to_string(param->getActiveCatalog()->getEpoch() + param->getActiveCatalog()->getRefEpoch());
}

std::string
GUI::getEpochSpeedText() {
    std::string str = "10^";
    if(param->getActiveCatalog()->getEpochSpeedInt() != 0) {
        str += std::to_string(glm::abs(param->getActiveCatalog()->getEpochSpeedInt()));
        if(param->getActiveCatalog()->getEpochSpeedInt() < 0){
            str = "-" + str;
        }
        str += " years/s";
    }
    else {
        str = "0 years/s";
    }
    return str;
}

void
GUI::updateCatalogsList() {
    std::vector<char> vList;
    vList.clear();
    for(int i = 0; i < param->catalogs->size(); ++i) {
        std::string t;
        //t = element.second.getSourceFileName();
        t = param->catalogs->operator[](i)->getSourceFileName();
        for(int i = 0; i < t.length(); ++i) {
            vList.push_back(t[i]);
        }
        vList.push_back('\0');
    }
    vList.push_back('\0'); // Separate items by \0, final item must be followed by \0\0\0
    vList.push_back('\0');

    if(catalogsList != nullptr) delete catalogsList; // Free previous list resources

    catalogsList = new char[vList.size()];
    for(int i = 0; i < vList.size(); ++i) {
        catalogsList[i] = vList[i];
    }
}

GUI::GUI(Parameters *p)
{
    param = p;
    activeFilter = 2;
    activePlaneInt = 0;
    activePlaneIntForPlot2D = 0;

    showCameraCoordsMode = true;

    O.ci = -0.3f; O.rgb = glm::vec3(0.6f, 0.69f, 1.0f); O.c =  'O';
    B.ci = -0.12f; B.rgb = glm::vec3(0.76f, 0.82f, 1.0f);B.c = 'B';
    A.ci = 0.13f; A.rgb = glm::vec3(0.89f, 0.91f, 1.0f); A.c = 'A';
    F.ci = 0.41f; F.rgb = glm::vec3(0.99f, 0.97f, 1.0f); F.c = 'F';
    G.ci = 0.63f; G.rgb = glm::vec3(1.0f, 0.94f, 0.89f); G.c = 'G';
    K.ci = 1.04f; K.rgb = glm::vec3(1.0f, 0.89f, 0.76f); K.c = 'K';
    M.ci = 1.68f; M.rgb = glm::vec3(1.0f, 0.78f, 0.52f); M.c = 'M';
    L.ci = 2.3f; L.rgb = glm::vec3(1.0f, 0.5f, 0.333f); L.c = 'L';
    T.ci = 3.3f; T.rgb = glm::vec3(1.0f, 0.333f, 0.222f); T.c = 'T';
    Y.ci = 4.0f; Y.rgb = glm::vec3(1.0f, 0.222f, 0.148f); Y.c = 'Y';

    loadingFile = false;
    chooseFileFormat = false;
    guiLoadOptions.discartAppMag = true;
    guiLoadOptions.defaultAppMag = 0.0f;

    showStats = false;

    guiLoadOptions.discartColorIndex = true;
    guiLoadOptions.defaultColorIndex = 0.0f;

    guiLoadOptions.discartParallax = true;
    guiLoadOptions.defaultDistance = 100.0f;

    guiLoadOptions.onlyWithRadialVel = true;

    fpsTextYOffset = 0;

    firstColor = glm::vec3(1.0f, 0.0f, 0.0f);
    secondColor = glm::vec3(0.0f, 1.0f, 0.0f);
    thirdColor = glm::vec3(0.0f, 0.0f, 1.0f);
    mixFactor = 1.0f;

    catalogsList = nullptr;

    // Init catalogs list
    updateCatalogsList();
}

GUI::~GUI() {
    if(catalogsList) delete catalogsList;
}

void
GUI::drawGridPoints(std::vector<gridPoint>* g, glm::vec4 c, std::string s) {
    for (unsigned int i = 0; i < g->size(); ++i) {
        if (g->operator[](i).visible) {

            std::string tag;
            if (g->operator[](i).isAlfa) {
                tag = "a";
            } else {
                tag = "d";
            }

            if (!ImGui::Begin((g->operator[](i).text + s + tag).c_str(), NULL,
                              ImVec2(0.0f, 0.0f), 0.0f,
                              ImGuiWindowFlags_NoSavedSettings |
                              ImGuiWindowFlags_NoMove |
                              ImGuiWindowFlags_AlwaysAutoResize |
                              ImGuiWindowFlags_NoCollapse |
                              ImGuiWindowFlags_NoTitleBar |
                              ImGuiWindowFlags_NoBringToFrontOnFocus // To keep text beyond other windows
            )) {
                ImGui::End();
                std::cout << "Failed to create ImGui window.\n";
                return;
            } else {
                ImGui::SetWindowPos(ImVec2(g->operator[](i).proj.x - ImGui::GetWindowSize().x / 2 +
                                           param->guiVarWidth,
                                           param->screenHeight - g->operator[](i).proj.y -
                                           ImGui::GetWindowSize().y), 0);
                ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(c.x, c.y, c.z, 1.0f);
                ImGui::Text(g->operator[](i).text.c_str());
                ImGui::End();
            }
        }
    }
}

void
GUI::saveToFile() {
    std::string filePath = "";
    if(toSaveFileFormat) {
        filePath = saveFileDialog("csv");
    } else {
        filePath = saveFileDialog("fits");
    }

    if(filePath != "") {
        if(toSaveFileFormat) {
            if(param->getActiveCatalog()->saveToCSVFile(filePath)) {
                std::cout << "CSV File saved.\n";
            }
        } else {
            if(param->getActiveCatalog()->saveToFITSFile(filePath)) {
                std::cout << "FITS File saved.\n";
            }
        }
    } else {
        std::cout << "Something went wrong with SaveFileDialog().\n";
    }
}

void
GUI::draw() {

    if (param->drawStarNamesFlag && param->getActiveCatalog() == param->defaultCatalog
    && param->activeRenderMode == SPACE_3D) {
        // Draw stars names text
        for (unsigned int i = 0; i < param->showingNames.size(); ++i) {
            if (!ImGui::Begin(param->showingNames[i].name.c_str(), NULL, ImVec2(0.0f, 0.0f), 0.0f,
                              ImGuiWindowFlags_NoSavedSettings |
                              ImGuiWindowFlags_NoMove |
                              ImGuiWindowFlags_AlwaysAutoResize |
                              ImGuiWindowFlags_NoCollapse |
                              ImGuiWindowFlags_NoTitleBar |
                              ImGuiWindowFlags_NoInputs |
                              ImGuiWindowFlags_NoBringToFrontOnFocus // To keep text beyond other windows
            )) {
                ImGui::End();
                std::cout << "Failed to create ImGui window.\n";
                return;
            }

            ImGui::SetWindowPos(ImVec2(param->showingNames[i].pos.x - ImGui::GetWindowSize().x / 2 + param->guiVarWidth,
                                       param->screenHeight - param->showingNames[i].pos.y - ImGui::GetWindowSize().y), 0);
            ImGui::Text(param->showingNames[i].name.c_str());
            ImGui::End();

        }
    }

    if(param->activeViewMode == CENTERED_ON_ORIGIN && param->activeRenderMode == SPACE_3D) {

        if(param->activePlane != GALACTOCENTRIC) {

            // Equatorial
            if (param->EQGrid->isVisible) {
                drawGridPoints(&param->EQGrid->gridPoints,
                               param->EQGrid->color,
                               "EQNumber");
            }

            // Galactic
            if (param->galacticGrid->isVisible) {
                drawGridPoints(&param->galacticGrid->gridPoints,
                               param->galacticGrid->color,
                               "galacticNumber");
            }

            // Ecliptic
            if (param->eclipticGrid->isVisible) {
                drawGridPoints(&param->eclipticGrid->gridPoints,
                               param->eclipticGrid->color,
                               "eclipticNumber");
            }

            // Horizon
            if (param->horizonGrid->isVisible && param->activePlane == HORIZON_VIEW) {
                drawGridPoints(&param->horizonGrid->gridPoints,
                               param->horizonGrid->color,
                               "horizonNumber");
            }

        } else {
            // Galactocentric
            if (param->galactoCentricGrid->isVisible) {
                drawGridPoints(&param->galactoCentricGrid->gridPoints,
                               param->galactoCentricGrid->color,
                               "galCenNumber");
            }
        }

        ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    }

    // Load file window
    if(loadingFile) {
        if (!ImGui::Begin("Loading settings", NULL, ImVec2(0.0f, 0.0f), 1.0f,
                          ImGuiWindowFlags_NoSavedSettings    |
                          ImGuiWindowFlags_NoMove             |
                          ImGuiWindowFlags_AlwaysAutoResize |
                          ImGuiWindowFlags_NoCollapse
        )) {
            ImGui::End();
            std::cout << "Failed to create ImGui window.\n";
            return;
        }
        ImGui::SetWindowPos(ImVec2(param->screenWidth / 2 - ImGui::GetWindowSize().x / 2, param->screenHeight / 2 -  ImGui::GetWindowSize().y / 2), 0);

        ImGui::Text(("File name: " + loadingFileName).c_str());

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Discart null fields flags:");
        ImGui::NewLine();
        ImGui::Checkbox("Apparent magnitude", &guiLoadOptions.discartAppMag);
        if(!guiLoadOptions.discartAppMag) {
            ImGui::SliderFloat("Dafault appMag:", &guiLoadOptions.defaultAppMag, -15.0f, 15.0f);
        }

        ImGui::Checkbox("B-V Color index", &guiLoadOptions.discartColorIndex);
        if(!guiLoadOptions.discartColorIndex) {
            ImGui::SliderFloat("Dafault color index:", &guiLoadOptions.defaultColorIndex, -5.0f, 5.0f);
        }

        ImGui::Checkbox("Parallax null/negative", &guiLoadOptions.discartParallax);
        if(!guiLoadOptions.discartParallax) {
            ImGui::SliderFloat("Dafault distance (parsecs):", &guiLoadOptions.defaultDistance, guiLoadOptions.defaultDistance * 0.95f, guiLoadOptions.defaultDistance * 1.05f);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("How to compute 3D Space velocity:");
        ImGui::Checkbox("", &guiLoadOptions.onlyWithRadialVel);
        ImGui::SameLine();
        if(guiLoadOptions.onlyWithRadialVel) {
            ImGui::Text("When radial velocity AND proper motion are known");
        } else {
            ImGui::Text("When radial velocity OR  proper motion is  known");
        }

        ImGui::NewLine();

        if(ImGui::Button("Load###loadButton")) {

            StarCatalog* newCat = new StarCatalog();
            if(newCat->loadGEDR3(loadingFilePath, guiLoadOptions)){
                newCat->setCustomColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); // GREEN
                newCat->setCiAbsGraphShader(param->ciAbsGraphShader);
                param->catalogs->push_back(newCat);

                updateCatalogsList();
                param->activeCatalogInt = param->catalogs->size() - 1;
            } else {
                std::cout << "Something went wrong while loading GEDR3 dataset.\n";
            }
            loadingFile = false;
        }

        ImGui::NewLine();

        ImGui::End();
    }

    if(chooseFileFormat) {
        if (!ImGui::Begin("Save to file", NULL, ImVec2(0.0f, 0.0f), 1.0f,
                          ImGuiWindowFlags_NoSavedSettings    |
                          ImGuiWindowFlags_NoMove             |
                          ImGuiWindowFlags_AlwaysAutoResize |
                          ImGuiWindowFlags_NoCollapse
        )) {
            ImGui::End();
            std::cout << "Failed to create ImGui window.\n";
            return;
        }
        ImGui::SetWindowPos(ImVec2(param->screenWidth / 2 - ImGui::GetWindowSize().x / 2, param->screenHeight / 2 -  ImGui::GetWindowSize().y / 2), 0);
        ImGui::Text("Choose the file format:");
        if(ImGui::Button("CSV (plain text)###csvButton")) {
            toSaveFileFormat = true;
            chooseFileFormat = false;
            saveToFile();
        }
        ImGui::SameLine();
        if(ImGui::Button("FITS (binary)###fitsButton")) {
            toSaveFileFormat = false;
            chooseFileFormat = false;
            saveToFile();
        }
        ImGui::End();
    }

    if(showStats) {
        if (!ImGui::Begin("Statistics", NULL, ImVec2(0.0f, 0.0f), 1.0f,
                          ImGuiWindowFlags_NoSavedSettings    |
                          ImGuiWindowFlags_NoMove             |
                          ImGuiWindowFlags_AlwaysAutoResize |
                          ImGuiWindowFlags_NoCollapse
        )) {
            ImGui::End();
            std::cout << "Failed to create ImGui window.\n";
            return;
        }
        ImGui::SetWindowPos(ImVec2(param->screenWidth / 2 - ImGui::GetWindowSize().x / 2, param->screenHeight / 2 -  ImGui::GetWindowSize().y / 2), 0);

        if (param->getActiveCatalog() != nullptr) {
            ImGui::Text(statsText.c_str());
        } else {
            ImGui::Text("No catalog loaded.");
        }

        if(ImGui::Button("Close###closeStats")) {
            showStats = false;
        }
        ImGui::End();
    }

    // BAR EXPERIMENT WINDOW
    if (!ImGui::Begin("Settings", NULL, ImVec2(0.0f, 0.0f), 1.0f,
                      ImGuiWindowFlags_NoSavedSettings |
                      ImGuiWindowFlags_NoMove |
                      ImGuiWindowFlags_AlwaysAutoResize |
                      ImGuiWindowFlags_MenuBar
    )) {

        if(ImGui::GetCurrentWindow()->Collapsed && !lastFrameCollapsed) {
            param->guiVarWidth = 0;
            lastFrameCollapsed = true;

            glViewport(param->guiVarWidth, 0, param->screenWidth - param->guiVarWidth, param->screenHeight);
            param->camera->setAspectRatio(float(param->screenWidth - param->guiVarWidth) / float(param->screenHeight));
            param->camera->setFovX(glm::radians(param->displayFOV));

            fpsTextYOffset = ImGui::GetWindowSize().y;

            param->plot2DShader->use();
            param->plot2DShader->setUniform("aspectRatio", float(param->screenWidth - param->guiVarWidth) / float(param->screenHeight));

        }

        ImGui::End();

    } else {

        if(lastFrameCollapsed) {
            param->guiVarWidth = ImGui::GetWindowSize().x;
            fpsTextYOffset = 0;
            lastFrameCollapsed = false;

            glViewport(param->guiVarWidth, 0, param->screenWidth - param->guiVarWidth, param->screenHeight);
            param->camera->setAspectRatio(float(param->screenWidth - param->guiVarWidth) / float(param->screenHeight));
            param->camera->setFovX(glm::radians(param->displayFOV));
            param->plot2DShader->use();
            param->plot2DShader->setUniform("aspectRatio", float(param->screenWidth - param->guiVarWidth) / float(param->screenHeight));

        }

        ImGui::SetWindowPos(ImVec2(0, 0), 0);

        //draw menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open...")) {
                    std::string fileName = "";

                    fileName = openFileDialog();

                    if(fileName != "") {
                        loadingFilePath = fileName;
                        std::vector<std::string> pathSegments = splitLine(fileName, '/');
                        loadingFileName = pathSegments.back();
                        loadingFile = true;

                    } else {
                        std::cout << "Something went wrong with OpenFileDialog().\n";
                    }
                }

                if (ImGui::MenuItem("Save as...")) {
                    chooseFileFormat = true;
                }

                if (ImGui::MenuItem("Close")) {
                    if (param->catalogs->size() != 0) {

                        if(param->getActiveCatalog() == param->defaultCatalog) {
                            param->defaultCatalog = nullptr;
                        }

                        delete param->getActiveCatalog();
                        param->catalogs->erase(param->catalogs->begin() + param->activeCatalogInt);
                        param->activeCatalogInt = 0;
                        updateCatalogsList();
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (ImGui::Combo("###Catalog", &param->activeCatalogInt, catalogsList)) {

            param->setUniforms();

            param->orbitStar = nullptr;
            param->selectedStar = nullptr;
            if(param->activeViewMode == ORBIT) {
                param->camera->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
            }
        }
        ImGui::SameLine();
        ImGui::Text("Catalog");

        if(ImGui::Button("Show stats###showStats")) {
            if(param->getActiveCatalog() != nullptr) {
                param->getActiveCatalog()->computeStats(s);
                showStats = true;

                statsText = "";
                statsText += "File: " + param->getActiveCatalog()->getSourceFileName() + "\n\n";
                statsText += "Total: " + std::to_string(s.entries) + "\n\n";
                statsText += "Parameter\n\n(MIN value, MAX value)\nAverage | Variance | Standard Desviation\n\n";
                statsText += "Absolute Magnitude\n\n(" + std::to_string(s.absMagMin) + ", " + std::to_string(s.absMagMax) + ")\n" + std::to_string(s.absMagAverage) + " | " + std::to_string(s.absMagVariance) + " | " + std::to_string(sqrt(s.absMagVariance)) + "\n\n";
                statsText += "BV Color index\n\n(" + std::to_string(s.ciMin) + ", " + std::to_string(s.ciMax) + ")\n" + std::to_string(s.ciAverage) + " | " + std::to_string(s.ciVariance) + " | " + std::to_string(sqrt(s.ciVariance)) + "\n\n";
                statsText += "Distance\n\n(" + std::to_string(s.distanceMin) + ", " + std::to_string(s.distanceMax) + ")\n" + std::to_string(s.distanceAverage) + " | " + std::to_string(s.distanceVariance) + " | " + std::to_string(sqrt(s.distanceVariance)) + "\n\n";
                statsText += "Velocity\n\n(" + std::to_string(s.velocityMin) + ", " + std::to_string(s.velocityMax) + ")\n" + std::to_string(s.velocityAverage) + " | " + std::to_string(s.velocityVariance) + " | " + std::to_string(sqrt(s.velocityVariance)) + "\n\n";

            }
        }

        if(param->getActiveCatalog() == param->defaultCatalog) {
            ImGui::Checkbox("Show star names", &param->drawStarNamesFlag);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Self-adjusting lower distance filter", &param->intelligentDistanceAdjustment);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Filters:");

        if(ImGui::Button("Distance###DistanceFilter")) {
            activeFilter = 0;
        }
        ImGui::SameLine();
        if(ImGui::Button("AbsMag###AbsMagFilter")) {
            activeFilter = 1;
        }
        ImGui::SameLine();
        if(ImGui::Button("B-V Index###BVIndexFilter")) {
            activeFilter = 2;
        }
        ImGui::SameLine();
        if(ImGui::Button("Velocity###speedFilter")) {
            activeFilter = 3;
        }

        ShaderProgram* s = param->getCurrentShader();

        if (activeFilter == 0) {
            // Distance Filters
            filter& f = param->getActiveCatalog()->getDistanceFilter();

            ImGui::Text(param->getActiveCatalog()->getDistanceFilterRangeText().c_str());
            if(ImGui::SliderFloat("###LowerDistance",
                             &f.lower,
                             std::max(1.0f, f.lower * 0.995f),
                             std::min(f.upper, f.lower * 1.005f),
                             "%0.9")) {

                param->getActiveCatalog()->getLowerDistanceFilter() = (int) f.lowerValue();
            }
            ImGui::SameLine();
            ImGui::Text("Lower");


            if(ImGui::SliderFloat("###UpperDistancer",
                               &f.upper,
                               std::max(f.lower, f.upper * 0.995f),
                               std::min(2.0f, f.upper * 1.005f),
                               "%0.9")) {

                param->getActiveCatalog()->getUpperDistanceFilter() = (int) f.upperValue();
            }

            ImGui::SameLine();
            ImGui::Text("Upper");

            if(ImGui::Button("Reset###resetDistanceFilter")) {
                f.lower = 1.0f;
                f.upper = 2.0f;
                param->getActiveCatalog()->getLowerDistanceFilter() = (int) f.lowerValue();
                param->getActiveCatalog()->getUpperDistanceFilter() = (int) f.upperValue();
            }

        }
        if (activeFilter == 1) {
            // Abs Mag Filters
            filter& f = param->getActiveCatalog()->getAbsMagFilter();
            ImGui::Text(("Absolute magnitude range:\n" + f.getRangeText()).c_str());

            if(ImGui::SliderFloat("###LowerAbsMag",
                                  &f.lower,
                                  std::max(1.0f, f.lower * 0.995f),
                                  std::min(f.upper, f.lower * 1.005f),
                                  "%0.9")) {
                s->use();
                s->setUniform("lowerAbsMagFilter", f.lowerValue());
            }
            ImGui::SameLine();
            ImGui::Text("Lower");
            if(ImGui::SliderFloat("###UpperAbsMag",
                                  &f.upper,
                                  std::max(f.lower, f.upper * 0.995f),
                                  std::min(2.0f, f.upper * 1.005f),
                                  "%0.9")) {
                s->use();
                s->setUniform("upperAbsMagFilter", f.upperValue());
            }
            ImGui::SameLine();
            ImGui::Text("Upper");
            if(ImGui::Button("Reset###resetAbsMagFilter")) {
                f.lower = 1.0f;
                f.upper = 2.0f;
                s->use();
                s->setUniform("lowerAbsMagFilter", f.lowerValue());
                s->setUniform("upperAbsMagFilter", f.upperValue());
            }
        }

        // V-B index filter
        if (activeFilter == 2) {
            filter& f = param->getActiveCatalog()->getColorFilter();
            ImGui::Text(("B-V Color index range:\n" + f.getRangeText()).c_str());

            if(ImGui::SliderFloat("###LowerColorIndex",
                                  &f.lower,
                                  std::max(1.0f, f.lower * 0.995f),
                                  std::min(f.upper, f.lower * 1.005f),
                                  "%0.9")) {
                s->use();
                s->setUniform("lowerColorIndexFilter", f.lowerValue());
            }
            ImGui::SameLine();
            ImGui::Text("Lower");

            if(ImGui::SliderFloat("###UpperColorIndex",
                                  &f.upper,
                                  std::max(f.lower, f.upper * 0.995f),
                               std::min(2.0f, f.upper * 1.005f),
                               "%0.9")) {
                s->use();
                s->setUniform("upperColorIndexFilter", f.upperValue());
            }
            ImGui::SameLine();
            ImGui::Text("Upper");

            if(ImGui::Button("Reset###resetColorIndexFilter")) {
                f.lower = 1.0f;
                f.upper = 2.0f;
                s->use();
                s->setUniform("lowerColorIndexFilter", f.lowerValue());
                s->setUniform("upperColorIndexFilter", f.upperValue());
            }

            ImGui::SameLine();
            showSpectralClassRange();
        }

        if (activeFilter == 3) {
            // Speed module filter
            filter& f = param->getActiveCatalog()->getVelocityFilter();
            ImGui::Text(("Velocity range:\n" + f.getRangeText() + " Km/s").c_str());
            if(ImGui::SliderFloat("###LowerSpeedIndex",
                                  &f.lower,
                                  std::max(1.0f, f.lower * 0.995f),
                                  std::min(f.upper, f.lower * 1.005f),
                                  "%0.9")) {
                s->use();
                s->setUniform("lowerSpeedModuleFilter", f.lowerValue());
            }
            ImGui::SameLine();
            ImGui::Text("Lower");
            if(ImGui::SliderFloat("###UpperSpeedIndex",
                                  &f.upper,
                                  std::max(f.lower, f.upper * 0.995f),
                                  std::min(2.0f, f.upper * 1.005f),
                                  "%0.9")) {
                s->use();
                s->setUniform("upperSpeedModuleFilter", f.upperValue());
            }

            ImGui::SameLine();
            ImGui::Text("Upper");

            if(ImGui::Button("Reset###resetVelocityFilter")) {
                f.lower = 1.0f;
                f.upper = 2.0f;
                s->use();
                s->setUniform("lowerSpeedModuleFilter", f.lowerValue());
                s->setUniform("upperSpeedModuleFilter", f.upperValue());
            }
        }

        ImGui::Spacing();

        // Colorize with filter
        ImGui::Checkbox("Colorize with filter###colorizeWithFilter", &param->colorizeWithFilter);

        if(param->colorizeWithFilter) {

            ImGui::ColorEdit3("Lower color###firstColor", (float*) &firstColor);
            ImGui::ColorEdit3("Middle color###secondColor", (float*) &secondColor);
            ImGui::ColorEdit3("Upper color###thirdColor", (float*) &thirdColor);
            ImGui::SliderFloat("Middle color factor###mixFactor", &mixFactor, mixFactor * 0.95f, mixFactor * 1.05f);

            if(ImGui::Button("Apply###applyColorization")) {
                unsigned int offset;
                glm::vec2 filterLimits = getLowerUpperLimits(offset);
                param->getActiveCatalog()->applyColorization(firstColor, secondColor, thirdColor, mixFactor, filterLimits.x, filterLimits.y, offset);
            }

            ImGui::SameLine();
            if(ImGui::Button("Reset###resetColorParams")) {
                firstColor = glm::vec3(1.0f, 0.0f, 0.0f);
                secondColor = glm::vec3(0.0f, 1.0f, 0.0f);
                thirdColor = glm::vec3(0.0f, 0.0f, 1.0f);
                mixFactor = 1.0f;
            }

            if(ImGui::Button("Reset B-V Color###resetBVColor")) {
                 param->getActiveCatalog()->resetBVColor();
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // If fov value changed
        if (ImGui::SliderFloat("Fov###Fov", &param->displayFOV, 0.0f, 180.0f)) {
            param->fovChanged = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("Reset###resetFov")) {
            param->displayFOV = 60.0f;
            param->fovChanged = true;
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("      Render mode:");
        ImGui::SameLine();
        if(ImGui::Combo("###RenderMode", &param->activeRenderModeInt, renderModeList, IM_ARRAYSIZE(renderModeList))){
            param->activeRenderMode = (renderMode) param->activeRenderModeInt;
            param->renderModeChanged();
            param->setUniforms();
        }

        if(param->activeRenderMode != HR_DIAGRAM) {
            ImGui::Text("Fundamental plane:");
            ImGui::SameLine();
            if (param->activeRenderMode == PLOT_2D) {
                if (ImGui::Combo("###Fundamental plane", &activePlaneIntForPlot2D, planesList,
                                 IM_ARRAYSIZE(planesList))) {

                    param->activePlaneForPlot2D = (fundamentalPlane) activePlaneIntForPlot2D;

                    param->getActiveCatalog()->update2DCoords(param->getMatrixForPlane(param->activePlaneForPlot2D));

                }
            } else {
                if (ImGui::Combo("###Fundamental plane", &activePlaneInt, planesList,
                                 IM_ARRAYSIZE(planesList))) {

                    param->activePlane = (fundamentalPlane) activePlaneInt;

                    // Update actual plane matrix
                    switch (param->activePlane) {
                        case EQUATORIAL:
                            param->activePlaneMatrix = param->identityMatrix;
                            param->activePlaneInverseMatrix = param->identityMatrix;
                            break;
                        case GALACTIC:
                            param->activePlaneMatrix = param->eqToGal;
                            param->activePlaneInverseMatrix = param->galToEq;
                            break;
                        case ECLIPTIC:
                            param->activePlaneMatrix = param->eqToEcl;
                            param->activePlaneInverseMatrix = param->eclToEq;
                            break;
                        case GALACTOCENTRIC:
                            param->activePlaneMatrix = param->eqToGalCen;
                            param->activePlaneInverseMatrix = param->galCenToEq;
                            break;
                        case HORIZON_VIEW:
                            param->activePlaneMatrix = param->earthPosMatrix;
                            param->activePlaneInverseMatrix = glm::inverse(param->activePlaneMatrix);
                            break;

                    }

                    // Make sure that when we exit horizon mode speed and time are 0
                    if(param->activePlane != HORIZON_VIEW) {
                        param->speedMode = 0;
                        param->time = 0.0f;
                    }

                    param->setCameraParameters();

                }
            }
        }

        if(param->activeRenderMode == SPACE_3D) {
            ImGui::Text("      Camera mode:");
            ImGui::SameLine();
            if (ImGui::Combo("###Camera mode", &param->activeViewModeInt, viewModeList,
                             IM_ARRAYSIZE(viewModeList))) {
                param->setViewMode((viewMode) param->activeViewModeInt);
            }

            if (param->activeViewMode == FREE_CAMERA) {
                ImGui::Spacing();
                if(ImGui::Button("Look at origin###lookAtOrigin") && param->camera->getPosition() != glm::vec3(0.0f, 0.0f, 0.0f)) {
                    glm::vec3 rd = polarFromCartesian(-param->camera->getPosition());
                    param->displayRA = rd.x;
                    param->displayDec = rd.y;
                    param->camera->setLookAtForFreeCamera(param->displayRA, param->displayDec);
                }
                ImGui::Text("\nCamera Speed:");
                ImGui::SliderFloat("parsecs/s###parsecs/s", &param->cameraSpeed, param->cameraSpeed * 0.85f, std::min(param->cameraSpeed * 1.15f, 100000.0f));
                ImGui::Spacing();
                if(ImGui::Button("Reset###resetSpeed")) {
                    param->cameraSpeed = 10.0f;
                }
            }

            if(param->activeViewMode == ORBIT) {
                ImGui::Spacing();
                if(ImGui::Button("Orbit origin###orbitOrigin")) {
                    param->orbitStar = nullptr;

                    glm::vec3 rd = polarFromCartesian(param->camera->getPosition());
                    param->orbitDistance = rd.z;

                    param->camera->setRaLootAk(glm::mod(param->displayRA + glm::pi<float>(),glm::two_pi<float>()));
                    param->camera->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
                    param->camera->setPosition(param->displayRA, param->displayDec, param->orbitDistance);
                }
            }

            // Grids
            if(param->activeViewMode == CENTERED_ON_ORIGIN) {
                if(param->activePlane != GALACTOCENTRIC) {
                    ImGui::Spacing();
                    ImGui::Checkbox("Equatorial Grid###EQGrid", &param->EQGrid->isVisible);
                    ImGui::SameLine();
                    ImGui::Checkbox("Galactic Grid###galacticGrid", &param->galacticGrid->isVisible);
                    ImGui::Checkbox("Ecliptic Grid###eclipticGrid", &param->eclipticGrid->isVisible);

                } else {
                    ImGui::Checkbox("Galactocentric Grid", &param->galactoCentricGrid->isVisible);
                }

            }

            if(param->activePlane == HORIZON_VIEW) {

                if(param->activeViewMode == CENTERED_ON_ORIGIN) {
                    ImGui::SameLine();
                    ImGui::Checkbox("Horizon Grid###horizonGrid", &param->horizonGrid->isVisible);
                }
                ImGui::Spacing();

                ImGui::Text("Earth position:");
                if(ImGui::SliderFloat("Longitude###Longitude", &param->longitude, -180.0f, 180.0f)) {
                    param->updateEarthPosMatrix();
                    param->horizonGrid->updateMatrix(param->earthPosMatrix);
                }
                if(ImGui::SliderFloat("Latitude###Latitude", &param->latitude, -90.0f, 90.0f)) {
                    param->updateEarthPosMatrix();
                    param->horizonGrid->updateMatrix(param->earthPosMatrix);
                }
                ImGui::Text("\nEarth rotation:");
                ImGui::SliderFloat("Time###Time", &param->time, 0.0f, 24.0f);

                ImGui::Text("Time speed: ");
                ImGui::SameLine();
                if(ImGui::Button("Slower###Slower")) {
                    if(param->speedMode != 0)
                        param->speedMode--;
                }
                ImGui::SameLine();
                if(ImGui::Button("Faster###Faster")) {
                    if(param->speedMode != 6)
                        param->speedMode++;
                }
                ImGui::SameLine();
                if(ImGui::Button("Stop###Stop")) {
                    param->speedMode = 0;
                }
                ImGui::SameLine();
                ImGui::Text(speedsList[param->speedMode]);

            }

        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if(param->activeRenderMode == SPACE_3D) {

            ImGui::Checkbox("Stars movement###starsMovement", &param->starsMovementFlag);

            if (param->starsMovementFlag) {
                ImGui::Checkbox("Render speed vectors###renderSpeedVectors", &param->drawVelocitiesFlag);

                if (param->drawVelocitiesFlag) {
                    if (ImGui::SliderFloat("Distance Limit###distanceLimit",
                                           &param->vectorDistanceLimit,
                                           param->vectorDistanceLimit * 0.9f,
                                           param->vectorDistanceLimit * 1.1f)) {
                    }
                }

                if (ImGui::Button("<<###RewindTime")) {
                    if (param->getActiveCatalog()->getEpochSpeedInt() > -20) {
                        param->getActiveCatalog()->getEpochSpeedInt()--;
                        updateEpochSpeed();
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Pause###pauseTime")) {
                    param->getActiveCatalog()->getEpochSpeedInt() = 0;
                    param->getActiveCatalog()->getEpochSpeed() = 0.0f;
                }
                ImGui::SameLine();
                if (ImGui::Button(">>###AdvanceTime")) {
                    if (param->getActiveCatalog()->getEpochSpeedInt() < 20) {
                        param->getActiveCatalog()->getEpochSpeedInt()++;
                        updateEpochSpeed();
                    }
                }
                ImGui::SameLine();
                ImGui::Text(getEpochSpeedText().c_str());
                ImGui::Text(getEpochText().c_str());
                if (ImGui::Button("Reset###ResetTime")) {
                    param->getActiveCatalog()->getEpochSpeedInt() = 0;
                    param->getActiveCatalog()->getEpochSpeed() = 0.0f;
                    param->getActiveCatalog()->getEpoch() = 0.0f;
                }
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }

        // Visualization parameters section
        ImGui::Checkbox("Customize visualization:###customizeVisualization", &param->customizeVisualizationFlag);
        if(param->customizeVisualizationFlag) {

            if(ImGui::Button("Size")) {
                param->activeParameter = SIZE_PARAMS;
            }
            ImGui::SameLine();
            if(ImGui::Button("Alfa")) {
                param->activeParameter = ALFA_PARAMS;
            }
            ImGui::SameLine();
            if(ImGui::Button("Halo")) {
                param->activeParameter = HALO_PARAMS;
            }

            if (param->activeParameter == SIZE_PARAMS) {
                ImGui::Checkbox("Use fixed point size###ufps", &param->useFixedPointSize);
                if(param->useFixedPointSize) {
                    ImGui::SliderFloat("Size###Size", &param->fixedPointSize, 0.0f, 20.0f);
                } else {
                    if (ImGui::SliderFloat("Factor###sizeFactor", &param->sizeFactor, param->sizeFactor * 0.99f,
                                           param->sizeFactor * 1.01f, "%.6f")) {
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("sizeFactor", param->sizeFactor);
                    }
                    if (ImGui::SliderFloat("Offset###sizeOffset", &param->sizeOffset, -50.0f, 50.0f, "%.6f")) {
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("sizeOffset", param->sizeOffset);
                    }
                    if (ImGui::SliderFloat("Slope###sizeSlope", &param->sizeSlope, param->sizeSlope * 0.99f,
                                           param->sizeSlope * 1.01f, "%.6f")) {
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("sizeSlope", param->sizeSlope);
                    }
                    if(ImGui::Button("Reset###resetSizeParams")) {
                        param->sizeFactor = 20.0f;
                        param->sizeOffset = 0.0f;
                        param->sizeSlope = 2.5f;
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("sizeFactor", param->sizeFactor);
                        param->getCurrentShader()->setUniform("sizeOffset", param->sizeOffset);
                        param->getCurrentShader()->setUniform("sizeSlope", param->sizeSlope);
                    }
                }
            } else if(param->activeParameter == ALFA_PARAMS) {
                ImGui::Checkbox("Do not attenuate###dnatt", &param->doNotAttenuate);
                if(!param->doNotAttenuate) {
                    if (ImGui::SliderFloat("Factor###alfaFactor", &param->alfaFactor, param->alfaFactor * 0.99f,
                                           param->alfaFactor * 1.01f, "%.6f")) {
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("alfaFactor", param->alfaFactor);
                    }
                    if (ImGui::SliderFloat("Offset##alfaOffset", &param->alfaOffset, -50.0f, 50.0f, "%.6f")) {
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("alfaOffset", param->alfaOffset);
                    }
                    if (ImGui::SliderFloat("Slope###alfaSlope", &param->alfaSlope, param->alfaSlope * 0.99f,
                                           param->alfaSlope * 1.01f, "%.6f")) {
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("alfaSlope", param->alfaSlope);
                    }
                    if(ImGui::Button("Reset###resetAlfaParams")) {
                        param->alfaFactor = 1.0f;
                        param->alfaOffset = 0.0f;
                        param->alfaSlope = 6.0f;
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("alfaFactor", param->alfaFactor);
                        param->getCurrentShader()->setUniform("alfaOffset", param->alfaOffset);
                        param->getCurrentShader()->setUniform("alfaSlope", param->alfaSlope);
                    }
                }
            } else if(param->activeParameter == HALO_PARAMS) {
                ImGui::Checkbox("Do not draw halo###dndhal", &param->doNotDrawHalo);
                if(!param->doNotDrawHalo) {
                    if (ImGui::SliderFloat("Factor###haloFactor", &param->haloFactor, param->haloFactor * 0.99f,
                                           param->haloFactor * 1.01f, "%.6f")) {
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("haloFactor", param->haloFactor);
                    }
                    if (ImGui::SliderFloat("Offset###haloOffset", &param->haloOffset, -1.0f, 1.0f, "%.6f")) {
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("haloOffset", param->haloOffset);
                    }
                    if (ImGui::SliderFloat("Slope###haloSlope", &param->haloSlope, param->haloSlope * 0.99f,
                                           param->haloSlope * 1.01f, "%.6f")) {
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("haloSlope", param->haloSlope);
                    }
                    if(ImGui::Button("Reset###resetHaloParams")) {
                        param->haloFactor = 1.0f;
                        param->haloOffset = 0.0f;
                        param->haloSlope = 0.2f;
                        param->getCurrentShader()->use();
                        param->getCurrentShader()->setUniform("haloFactor", param->haloFactor);
                        param->getCurrentShader()->setUniform("haloOffset", param->haloOffset);
                        param->getCurrentShader()->setUniform("haloSlope", param->haloSlope);
                    }
                }
            }

        }

        if(param->guiVarWidth != ImGui::GetWindowSize().x) {
            param->guiVarWidth = ImGui::GetWindowSize().x;
            glViewport(param->guiVarWidth, 0, param->screenWidth - param->guiVarWidth, param->screenHeight);
            param->camera->setAspectRatio(float(param->screenWidth - param->guiVarWidth) / float(param->screenHeight));
            param->camera->setFovX(glm::radians(param->displayFOV));
        }

        ImGui::End();
    }

    // Show selected star info
    if(param->selectedStar != nullptr && param->isSelectedStarVisible) {
        if (!ImGui::Begin("SelectedStar", NULL, ImVec2(0.0f, 0.0f), 1.0f,
                          ImGuiWindowFlags_NoSavedSettings    |
                          ImGuiWindowFlags_NoMove             |
                          ImGuiWindowFlags_AlwaysAutoResize |
                          ImGuiWindowFlags_NoCollapse |
                          ImGuiWindowFlags_NoTitleBar
        )) {
            ImGui::End();
            std::cout << "Failed to create ImGui window.\n";
            return;
        } else {
            ImGui::SetWindowPos(ImVec2(param->screenWidth - ImGui::GetWindowSize().x, 0.0f), 0);

            ImGui::Text(param->selectedStarInfoText.c_str());
            ImGui::NewLine();
            if (ImGui::Button("Orbit###orbitSelectedStar")) {
                param->orbitStar = param->selectedStar;
                param->switchToOrbitStar();
            }
            ImGui::End();
        }
    }

    if(param->activeRenderMode == SPACE_3D) {
        // Show Ra Dec
        if (!ImGui::Begin("ShowRaDec", NULL, ImVec2(0.0f, 0.0f), 1.0f,
                          ImGuiWindowFlags_NoSavedSettings    |
                          ImGuiWindowFlags_NoMove             |
                          ImGuiWindowFlags_AlwaysAutoResize |
                          ImGuiWindowFlags_NoCollapse |
                          ImGuiWindowFlags_NoTitleBar
        )) {
            ImGui::End();
            std::cout << "Failed to create ImGui window.\n";
            return;
        } else {
            ImGui::SetWindowPos(
                    ImVec2(param->screenWidth - ImGui::GetWindowSize().x, param->screenHeight - ImGui::GetWindowSize().y),
                    0);

            if (ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(0)) {
                showCameraCoordsMode = !showCameraCoordsMode;
            }
            if (showCameraCoordsMode) {
                ImGui::Text(param->getRaDecTex().c_str());
            } else {
                ImGui::Text(getCameraPosText(param->camera->getPosition()).c_str());
            }

            ImGui::End();
        }
    }

    // Show FPS
    if (!ImGui::Begin("ShowFPS", NULL, ImVec2(0.0f, 0.0f), 1.0f,
                      ImGuiWindowFlags_NoSavedSettings    |
                      ImGuiWindowFlags_NoMove             |
                      ImGuiWindowFlags_AlwaysAutoResize |
                      ImGuiWindowFlags_NoCollapse |
                      ImGuiWindowFlags_NoTitleBar
    )) {
        ImGui::End();
        std::cout << "Failed to create ImGui window.\n";
        return;
    } else {
        ImGui::SetWindowPos(ImVec2(param->guiVarWidth, fpsTextYOffset), 0);
        ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(param->fpsColor.x, param->fpsColor.y, param->fpsColor.z, 1.0f);
        ImGui::Text(param->fpsText.c_str());
        ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
        ImGui::End();
    }

}