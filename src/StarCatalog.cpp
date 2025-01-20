#include "StarCatalog.h"

unsigned int StarCatalog::starID = 0;

StarCatalog::StarCatalog() { }

StarCatalog::~StarCatalog() {
    indexToDistance.clear(); // Index-Distance map
    DistanceToIndex.clear(); // Distance-Index map
    starsIndices.clear();

    glDeleteBuffers(1, &starsIBO);
    glDeleteBuffers(1, &starsVBO);
    glDeleteVertexArrays(1, &starsVAO);
}

int
StarCatalog::size() {
    return starsIndices.size();
}

bool&
StarCatalog::getDrawCatalog() {
    return drawCatalog;
}

bool&
StarCatalog::getShowCiAbsGraph() {
    return showCiAbsGraph;
}

bool&
StarCatalog::getUseCustomColorFlag() {
    return useCustomColorFlag;
}

float*
StarCatalog::getCustomColor() {
    return customColor;
}

void
StarCatalog::setCustomColor(glm::vec4 color) {
    customColor[0] = color.x;
    customColor[1] = color.y;
    customColor[2] = color.z;
    customColor[3] = color.w;
}

void
StarCatalog::setCiAbsGraphShader(ShaderProgram* sh) {
    CiAbsGraphShader = sh;
}

int
StarCatalog::findColumn(std::vector<std::string>& splittedLine, std::string s) {
    int j = -1;
    for(int i = 0; i < splittedLine.size(); ++i) {
        if(splittedLine[i] == s) j = i;
    }
    return j;
}

bool
StarCatalog::checkFirstCSVLine(std::string line) {
    std::vector<std::string> splittedLine = splitLine(line, ',');

    totalNumberOfColumnsInCSVFile = splittedLine.size();

    raColumn = findColumn(splittedLine, "ra");
    if(raColumn == -1) {
        std::cout << "Error: Ra column missing in csv file.\n";
        return false;
    }

    decColumn = findColumn(splittedLine, "dec");
    if(decColumn == -1) {
        std::cout << "Error: Dec column missing in csv file.\n";
        return false;
    }

    parallaxColumn = findColumn(splittedLine, "parallax");
    if(parallaxColumn == -1) {
        std::cout << "Error: Parallax column missing in csv file.\n";
        return false;
    }

    appMagColumn = findColumn(splittedLine, "phot_g_mean_mag");
    if(appMagColumn == -1) {
        std::cout << "Error: phot_g_mean_mag column missing in csv file.\n";
        return false;
    }

    colorIndexColumn = findColumn(splittedLine, "bp_g");
    if(colorIndexColumn == -1) {
        std::cout << "Error: bp_g column missing in csv file.\n";
        return false;
    }

    pmRaColumn = findColumn(splittedLine, "pmra");
    if(pmRaColumn == -1) {
        std::cout << "Error: pmra column missing in csv file.\n";
        return false;
    }

    pmDecColumn = findColumn(splittedLine, "pmdec");
    if(pmDecColumn == -1) {
        std::cout << "Error: pmdec column missing in csv file.\n";
        return false;
    }

    radialVelColumn = findColumn(splittedLine, "radial_velocity");
    if(radialVelColumn == -1) {
        std::cout << "Error: radial_velocity column missing in csv file.\n";
        return false;
    }

    return true;
}

bool
StarCatalog::checkFITSParams(std::vector<std::string> params) {

    totalNumberOfColumnsInFITSFile = params.size();

    raColumn = findColumn(params, "ra");
    if(raColumn == -1) {
        std::cout << "Error: Ra column missing in fits file.\n";
        return false;
    }

    decColumn = findColumn(params, "dec");
    if(decColumn == -1) {
        std::cout << "Error: Dec column missing in fits file.\n";
        return false;
    }

    parallaxColumn = findColumn(params, "parallax");
    if(parallaxColumn == -1) {
        std::cout << "Error: Parallax column missing in fits file.\n";
        return false;
    }

    appMagColumn = findColumn(params, "phot_g_mean_mag");
    if(appMagColumn == -1) {
        std::cout << "Error: phot_g_mean_mag column missing in fits file.\n";
        return false;
    }

    colorIndexColumn = findColumn(params, "bp_g");
    if(colorIndexColumn == -1) {
        std::cout << "Error: bp_g column missing in fits file.\n";
        return false;
    }

    pmRaColumn = findColumn(params, "pmra");
    if(pmRaColumn == -1) {
        std::cout << "Error: pmra column missing in fits file.\n";
        return false;
    }

    pmDecColumn = findColumn(params, "pmdec");
    if(pmDecColumn == -1) {
        std::cout << "Error: pmdec column missing in fits file.\n";
        return false;
    }

    radialVelColumn = findColumn(params, "radial_velocity");
    if(radialVelColumn == -1) {
        std::cout << "Error: radial_velocity column missing in fits file.\n";
        return false;
    }

    return true;
}

bool checkColumnName(std::string name) {
    return name == "ra" ||
           name == "dec" ||
           name == "parallax" ||
           name == "phot_g_mean_mag" ||
           name == "bp_g" ||
           name == "pmra" ||
           name == "pmdec" ||
           name == "radial_velocity";
}

double stringToDouble(const std::string& str) {
    std::stringstream ss(str);
    double value;
    ss >> value;
    return value;
}

bool
StarCatalog::loadGDR3(std::string filePath, LoadOptions guiOptions) {

    loadOptions = guiOptions;

    refEpoch = 2016.0f;
    epoch = 0.0f;
    epochSpeed = 0.0f;
    epochSpeedInt = 0;

    //set filename
    #ifdef _WIN32
        std::vector<std::string> splittedPath = splitLine(filePath, '\\');
    #else
        std::vector<std::string> splittedPath = splitLine(filePath, '/');
    #endif
    sourceFileName = splittedPath[splittedPath.size() - 1];
    std::cout << "Source filename: " << sourceFileName << "\n";

    std::vector<ColorValue> colorIndexTable;
    colorIndexTable.clear();
    colorIndexTable.push_back(ColorValue(-0.4f, hexToRGB("0x9bb2ff")));
    colorIndexTable.push_back(ColorValue(-0.35f, hexToRGB("0x9eb5ff")));
    colorIndexTable.push_back(ColorValue(-0.30f, hexToRGB("0xa3b9ff")));
    colorIndexTable.push_back(ColorValue(-0.25f, hexToRGB("0xaabfff")));
    colorIndexTable.push_back(ColorValue(-0.20f, hexToRGB("0xb2c5ff")));
    colorIndexTable.push_back(ColorValue(-0.15f, hexToRGB("0xbbccff")));
    colorIndexTable.push_back(ColorValue(-0.10f, hexToRGB("0xc4d2ff")));
    colorIndexTable.push_back(ColorValue(-0.05f, hexToRGB("0xccd8ff")));
    colorIndexTable.push_back(ColorValue(0.0f, hexToRGB("0xd3ddff")));
    colorIndexTable.push_back(ColorValue(0.05f, hexToRGB("0xdae2ff")));
    colorIndexTable.push_back(ColorValue(0.10f, hexToRGB("0xdfe5ff")));
    colorIndexTable.push_back(ColorValue(0.15f, hexToRGB("0xe4e9ff")));
    colorIndexTable.push_back(ColorValue(0.20f, hexToRGB("0xe9ecff")));
    colorIndexTable.push_back(ColorValue(0.25f, hexToRGB("0xeeefff")));
    colorIndexTable.push_back(ColorValue(0.30f, hexToRGB("0xf3f2ff")));
    colorIndexTable.push_back(ColorValue(0.35f, hexToRGB("0xf8f6ff")));
    colorIndexTable.push_back(ColorValue(0.40f, hexToRGB("0xfef9ff")));
    colorIndexTable.push_back(ColorValue(0.45f, hexToRGB("0xfff9fb")));
    colorIndexTable.push_back(ColorValue(0.50f, hexToRGB("0xfff7f5")));
    colorIndexTable.push_back(ColorValue(0.55f, hexToRGB("0xfff5ef")));
    colorIndexTable.push_back(ColorValue(0.60f, hexToRGB("0xfff3ea")));
    colorIndexTable.push_back(ColorValue(0.65f, hexToRGB("0xfff3ea")));
    colorIndexTable.push_back(ColorValue(0.70f, hexToRGB("0xffefe0")));
    colorIndexTable.push_back(ColorValue(0.75f, hexToRGB("0xffeddb")));
    colorIndexTable.push_back(ColorValue(0.80f, hexToRGB("0xffebd6")));
    colorIndexTable.push_back(ColorValue(0.85f, hexToRGB("0xffe9d2")));
    colorIndexTable.push_back(ColorValue(0.90f, hexToRGB("0xffe8ce")));
    colorIndexTable.push_back(ColorValue(0.95f, hexToRGB("0xffe6ca")));
    colorIndexTable.push_back(ColorValue(1.0f, hexToRGB("0xffe5c6")));
    colorIndexTable.push_back(ColorValue(1.05f, hexToRGB("0xffe3c3")));
    colorIndexTable.push_back(ColorValue(1.10f, hexToRGB("0xffe2bf")));
    colorIndexTable.push_back(ColorValue(1.15f, hexToRGB("0xffe0bb")));
    colorIndexTable.push_back(ColorValue(1.20f, hexToRGB("0xffdfb8")));
    colorIndexTable.push_back(ColorValue(1.25f, hexToRGB("0xffddb4")));
    colorIndexTable.push_back(ColorValue(1.30f, hexToRGB("0xffdbb0")));
    colorIndexTable.push_back(ColorValue(1.35f, hexToRGB("0xffdaad")));
    colorIndexTable.push_back(ColorValue(1.40f, hexToRGB("0xffd8a9")));
    colorIndexTable.push_back(ColorValue(1.45f, hexToRGB("0xffd6a5")));
    colorIndexTable.push_back(ColorValue(1.50f, hexToRGB("0xffd5a1")));
    colorIndexTable.push_back(ColorValue(1.55f, hexToRGB("0xffd29c")));
    colorIndexTable.push_back(ColorValue(1.60f, hexToRGB("0xffd096")));
    colorIndexTable.push_back(ColorValue(1.65f, hexToRGB("0xffcc8f")));
    colorIndexTable.push_back(ColorValue(1.70f, hexToRGB("0xffc885")));
    colorIndexTable.push_back(ColorValue(1.75f, hexToRGB("0xffc178")));
    colorIndexTable.push_back(ColorValue(1.80f, hexToRGB("0xffb765")));
    colorIndexTable.push_back(ColorValue(1.85f, hexToRGB("0xffa94b")));
    colorIndexTable.push_back(ColorValue(1.90f, hexToRGB("0xff9523")));
    colorIndexTable.push_back(ColorValue(1.95f, hexToRGB("0xff7b00")));
    colorIndexTable.push_back(ColorValue(2.0f, hexToRGB("0xff5200")));

    apMagModifier = 0.0f;
    absMagModifier = 0.0f;

    drawCatalog = false;
    showCiAbsGraph = false;
    useCustomColorFlag = false;
    customColor[0] = 0.0f;
    customColor[1] = 0.0f;
    customColor[2] = 0.0f;
    customColor[3] = 0.0f;

    unknownMagnitudeStarsCount = 0;
    bool unknownDistance;
    bool unknownMagnitude;

        std::string name;
        double ra, dec, dist, ma, absMa;

        bool discartEntry = false;
        int discartedEntries = 0;

        int nullsMagnitudes = 0;

        std::cout << "Loading stars from GDR3 dataset...\n";

    // Check if file is CSV or FITS
    std::vector<std::string> splittedName = splitLine(sourceFileName, '.');
    std::string fileExtension = splittedName[splittedName.size() - 1];
    std::cout << "File format: " << fileExtension << "\n";

    // CSV LOAD
    if(fileExtension == "csv") {

        std::ifstream file;
        file = std::ifstream(filePath);

        if (file.is_open()) {
            std::string line;
            std::getline(file, line); // we must analize this line and check we have the main params

            // Analize first line CSV info
            if(!checkFirstCSVLine(line)) {
                file.close();
                std::cout << "Missing params in CSV dataset.\n";
                return false;
            }

            while (std::getline(file, line)) {
                std::vector<std::string> splittedLine = splitLine(line, ',');

                // check if the last column is null
                if(splittedLine.size() != totalNumberOfColumnsInCSVFile) {
                    splittedLine.push_back("");
                }


                if(splittedLine[raColumn] == "") {
                    discartEntry = true;
                } else {
                    ra = glm::radians(stringToDouble(splittedLine[raColumn]));
                }

                if(splittedLine[decColumn] == "") {
                    discartEntry = true;
                } else {
                    dec = glm::radians(stringToDouble(splittedLine[decColumn]));
                }

                // Distance from parallax
                if(splittedLine[parallaxColumn] == "") {
                    if(loadOptions.discartParallax) {
                        discartEntry = true;
                    } else {
                        dist = loadOptions.defaultDistance;
                    }
                } else {
                    if(stringToDouble(splittedLine[parallaxColumn]) <= 0.0) {
                        if(loadOptions.discartParallax) {
                            discartEntry = true;
                        } else {
                            dist = loadOptions.defaultDistance;
                        }
                    } else {
                        dist = parallaxToDist(stringToDouble(splittedLine[parallaxColumn]));
                    }
                }


                if(splittedLine[appMagColumn] == "") {
                    if(loadOptions.discartAppMag) {
                        discartEntry = true;
                    } else {
                        ma = loadOptions.defaultAppMag;
                    }
                } else {
                    ma = stringToDouble(splittedLine[appMagColumn]);
                }

                glm::vec3 color;
                float colorIndex;
                if(splittedLine[colorIndexColumn] == "") {
                    if(loadOptions.discartColorIndex) {
                        discartEntry = true;
                    } else {
                        colorIndex = loadOptions.defaultColorIndex;
                        color = getColor(colorIndex, colorIndexTable);
                    }
                } else {
                    colorIndex = stringToDouble(splittedLine[colorIndexColumn]);
                    color = getColor(colorIndex, colorIndexTable);
                }

                // Proper motion ra dec radialVel
                float pmra, pmdec, rVel;
                pmra = pmdec = rVel = 0.0f;

                //pmra
                if(splittedLine[pmRaColumn] != "") {
                    pmra = stringToDouble(splittedLine[pmRaColumn]);
                }
                if(splittedLine[pmDecColumn] != "") {
                    pmdec = stringToDouble(splittedLine[pmDecColumn]);
                }
                if(splittedLine[radialVelColumn] != "") {
                    rVel = stringToDouble(splittedLine[radialVelColumn]);
                }

                Star s = Star(ra,
                              dec,
                              dist,
                              absMagFromApMag(ma, dist),
                              color,
                              colorIndex,
                              pmra,
                              pmdec,
                              rVel,
                              loadOptions.onlyWithRadialVel);

                if(!discartEntry) {
                    catalog.push_back(s);
                }
                discartEntry = false;

            }

            file.close();
        }
        else {
            std::cout   << "Error: could not load dataset.\n"
                        << "File path: " << filePath << "\n";
            return false;
        }

    }
    // FITS LOAD
    else if(fileExtension == "fits") {

        CCfits::FITS* input_file(new CCfits::FITS(filePath,CCfits::Read));
        CCfits::ExtHDU& table = input_file->extension("votable");

        std::vector<std::string> params;
        std::vector<std::valarray<float> > values;
        params.clear();
        values.clear();

        for(unsigned int i = 0; i < table.numCols(); ++i) {
            CCfits::Column& column = table.column(i + 1);
            if(checkColumnName(column.name())) {
                params.push_back(column.name());
                values.push_back(std::valarray<float>());
                column.read(values[i], 1, column.rows());
            }
        }

        int rows = table.column(1).rows();
        int cols = table.numCols();
        input_file->destroy();

        // Check params in fits file
        if(!checkFITSParams(params)) {
            std::cout << "Missing params in FITS dataset.\n";
            return false;
        }

            for(unsigned int i = 0; i < rows; ++i) {
                if(values[raColumn][i] != values[raColumn][i]) {
                    discartEntry = true;
                } else {
                    ra = glm::radians(values[raColumn][i]);
                }

                if(values[decColumn][i] != values[decColumn][i]) {
                    discartEntry = true;
                } else {
                    dec = glm::radians(values[decColumn][i]);
                }

                // Distance from parallax
                if(values[parallaxColumn][i] != values[parallaxColumn][i]) {
                    if(loadOptions.discartParallax) {
                        discartEntry = true;
                    } else {
                        dist = loadOptions.defaultDistance;
                    }
                } else {
                    if(values[parallaxColumn][i] <= 0.0f) {
                        if(loadOptions.discartParallax) {
                            discartEntry = true;
                        } else {
                            dist = loadOptions.defaultDistance;
                        }
                    } else {
                        dist = parallaxToDist(values[parallaxColumn][i]);
                    }
                }

                if(values[appMagColumn][i] != values[appMagColumn][i]) {
                    if(loadOptions.discartAppMag) {
                        discartEntry = true;
                    } else {
                        ma = loadOptions.defaultAppMag;
                    }
                } else {
                    ma = values[appMagColumn][i];
                }

                glm::vec3 color;
                float colorIndex;
                if(values[colorIndexColumn][i] != values[colorIndexColumn][i]) {
                    if(loadOptions.discartColorIndex) {
                        discartEntry = true;
                    } else {
                        colorIndex = loadOptions.defaultColorIndex;
                        color = getColor(colorIndex, colorIndexTable);
                    }
                } else {
                    colorIndex = values[colorIndexColumn][i];
                    color = getColor(colorIndex, colorIndexTable);
                }

                // Proper motion ra dec radialVel
                float pmra, pmdec, rVel;
                pmra = pmdec = rVel = 0.0f;

                //pmra
                if(values[pmRaColumn][i] == values[pmRaColumn][i]) {
                    pmra = values[pmRaColumn][i];
                }
                if(values[pmDecColumn][i] == values[pmDecColumn][i]) {
                    pmdec = values[pmDecColumn][i];
                }
                if(values[radialVelColumn][i] == values[radialVelColumn][i]) {
                    rVel = values[radialVelColumn][i];
                }

                Star s = Star(ra,
                              dec,
                              dist,
                              absMagFromApMag(ma, dist),
                              color,
                              colorIndex,
                              pmra,
                              pmdec,
                              rVel,
                              loadOptions.onlyWithRadialVel);

                if(!discartEntry) {
                    catalog.push_back(s);
                }
                discartEntry = false;
            }


        }
        // EXTENSION ERROR
        else {
            std::cout   << "Error: could not detect the file extension.\n"
                        << "Supported file formats are CSV and FITS.\n"
                        << "Check that your dataset file has extension (fileName.csv or fileName.fits)\n";

            return false;
        }

         // Order by distance
        std::sort(catalog.begin(), catalog.end(), StarDistanceComparator());

         catalogSize = catalog.size();

         // Init index-distance map
         indexToDistance.clear();
         DistanceToIndex.clear();
         for(int i = 0; i < catalog.size(); ++i) {
             indexToDistance.push_back(std::pair<int, float>(i, catalog[i].getDistance()));
             DistanceToIndex.insert(std::pair<float, int>(catalog[i].getDistance(), i));
         }

         distance.min = catalog.front().getDistance();
         distance.max = catalog.back().getDistance();

         absMag.min = std::numeric_limits<float>().max();
         absMag.max = std::numeric_limits<float>().min();

         BVColorIndex.min = std::numeric_limits<float>().max();
         BVColorIndex.max = std::numeric_limits<float>().min();

         speedModule.min = std::numeric_limits<float>().max();
         speedModule.max = std::numeric_limits<float>().min();

         for(int i = 0; i < catalog.size(); ++i) {
             if(catalog[i].getAbsMag() < absMag.min) absMag.min = catalog[i].getAbsMag();
             if(catalog[i].getAbsMag() > absMag.max) absMag.max = catalog[i].getAbsMag();

             if(catalog[i].speedModule < speedModule.min) speedModule.min = catalog[i].speedModule;
             if(catalog[i].speedModule > speedModule.max) speedModule.max = catalog[i].speedModule;

             if(catalog[i].getColorIndex() < BVColorIndex.min) BVColorIndex.min = catalog[i].getColorIndex();
             if(catalog[i].getColorIndex() > BVColorIndex.max) BVColorIndex.max = catalog[i].getColorIndex();
         }

         lowerAbsMagFilter = absMag.min;
         upperAbsMagFilter = absMag.max;

         lowerColorIndexFilter = BVColorIndex.min;
         upperColorIndexFilter = BVColorIndex.max;

         lowerSpeedModuleFilter = speedModule.min;
         upperSpeedModuleFilter = speedModule.max;

         knownDistanceStarsCount = catalog.size();
         unknownDistanceStarsCount = unknownDistanceStars.size();

         lowerDistanceFilterIndex = 1;
         upperDistanceFilterIndex = catalog.size();

         // Generate VAO
         glGenVertexArrays(1, &starsVAO);
         if(starsVAO == 0) throw std::runtime_error("Failed to create a VAO.\n");
         glBindVertexArray(starsVAO);
         // Generate VBOs, 0 for Positions, 1 for distance, 2 for radius, 3 for aparent magnitude, 4 color, 5 absolute magnitude, 6 int ID, 7 color index, 8 velocities, 9 3d position
    glGenBuffers(1, &starsVBO);

    if(starsVBO == 0) throw std::runtime_error("Failed to create a VBO.\n");
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);

    // 0 ID
    glEnableVertexAttribArray(0);
    // NOTE THIS FUNCTION IS DIFFERENT THAN THE OTHERS BECAUSE WE PASS UNSIGNED INT VALUES
    glVertexAttribIPointer(0, sizeof(GLuint) / sizeof(GLuint), GL_UNSIGNED_INT, sizeof(Star), (GLuint*) offsetof(Star, id));

    for(unsigned int i = 0; i < catalog.size(); ++i) {
        catalog[i].id = starID;
        starID++;
    }
    firstID = catalog.front().id;
    lastID = catalog.back().id;

    // 1 Position (ra, dec)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, position));

    // 2 Distance
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, sizeof(GLfloat) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, distance));

    // 3 3D Position
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, position3D));

    // 4 Velocity
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, velocity));

    // 5 Absolute Magnitude
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, sizeof(GLfloat) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, absMag));

    // 6 Color Index
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, sizeof(GLfloat) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, colorIndex));

    // 7 Color (RGB)
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, color));

    // 8 Speed module
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, sizeof(GLfloat) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, speedModule));

    // Copy to VRAM
    glBufferData(GL_ARRAY_BUFFER, sizeof(Star) * catalog.size(), catalog.data(), GL_STATIC_DRAW);


    // Generate IBOs
    glGenBuffers(1, &starsIBO);
    if(starsIBO == 0) throw std::runtime_error("Failed to create a IBO.\n");

    starsIndices.clear();
    for(unsigned int i = 0; i < catalog.size(); ++i) {
        starsIndices.push_back(i);
    }

    // Point Cloud
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starsIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * starsIndices.size(), starsIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Clear stars Vectors
    unknownDistanceStars.clear();

    // Set indexFilter to catalog size
    indexFilter = 0;

    distanceFilter.min = 1.0f;
    distanceFilter.max = (float) catalogSize;
    distanceFilter.lower = 1.0f;
    distanceFilter.upper = 2.0f;

    colorFilter.min = BVColorIndex.min;
    colorFilter.max = BVColorIndex.max;
    colorFilter.lower = 1.0f;
    colorFilter.upper = 2.0f;

    absMagFilter.min = absMag.min;
    absMagFilter.max = absMag.max;
    absMagFilter.lower = 1.0f;
    absMagFilter.upper = 2.0f;

    velocityFilter.min = speedModule.min;
    velocityFilter.max = speedModule.max;
    velocityFilter.lower = 1.0f;
    velocityFilter.upper = 2.0f;

    catalog.clear();

    return true;
}

std::vector<starWithProperName>&
StarCatalog::getStarsWithName() {
    return starsWithName;
}

void
StarCatalog::loadHYG(std::string filePath) {

    refEpoch = 2000.0f;
    epoch = 0.0f;
    epochSpeed = 0.0f;
    epochSpeedInt = 0;

    starsWithName.clear();
    bool hasName = false;

    //set filename
    std::vector<std::string> splittedPath = splitLine(filePath, '/');
    sourceFileName = splittedPath[splittedPath.size() - 1];
    std::cout << "Source filename: " << sourceFileName << "\n";

    std::vector<ColorValue> colorIndexTable;
    colorIndexTable.clear();
    colorIndexTable.push_back(ColorValue(-0.4f, hexToRGB("0x9bb2ff")));
    colorIndexTable.push_back(ColorValue(-0.35f, hexToRGB("0x9eb5ff")));
    colorIndexTable.push_back(ColorValue(-0.30f, hexToRGB("0xa3b9ff")));
    colorIndexTable.push_back(ColorValue(-0.25f, hexToRGB("0xaabfff")));
    colorIndexTable.push_back(ColorValue(-0.20f, hexToRGB("0xb2c5ff")));
    colorIndexTable.push_back(ColorValue(-0.15f, hexToRGB("0xbbccff")));
    colorIndexTable.push_back(ColorValue(-0.10f, hexToRGB("0xc4d2ff")));
    colorIndexTable.push_back(ColorValue(-0.05f, hexToRGB("0xccd8ff")));
    colorIndexTable.push_back(ColorValue(0.0f, hexToRGB("0xd3ddff")));
    colorIndexTable.push_back(ColorValue(0.05f, hexToRGB("0xdae2ff")));
    colorIndexTable.push_back(ColorValue(0.10f, hexToRGB("0xdfe5ff")));
    colorIndexTable.push_back(ColorValue(0.15f, hexToRGB("0xe4e9ff")));
    colorIndexTable.push_back(ColorValue(0.20f, hexToRGB("0xe9ecff")));
    colorIndexTable.push_back(ColorValue(0.25f, hexToRGB("0xeeefff")));
    colorIndexTable.push_back(ColorValue(0.30f, hexToRGB("0xf3f2ff")));
    colorIndexTable.push_back(ColorValue(0.35f, hexToRGB("0xf8f6ff")));
    colorIndexTable.push_back(ColorValue(0.40f, hexToRGB("0xfef9ff")));
    colorIndexTable.push_back(ColorValue(0.45f, hexToRGB("0xfff9fb")));
    colorIndexTable.push_back(ColorValue(0.50f, hexToRGB("0xfff7f5")));
    colorIndexTable.push_back(ColorValue(0.55f, hexToRGB("0xfff5ef")));
    colorIndexTable.push_back(ColorValue(0.60f, hexToRGB("0xfff3ea")));
    colorIndexTable.push_back(ColorValue(0.65f, hexToRGB("0xfff3ea")));
    colorIndexTable.push_back(ColorValue(0.70f, hexToRGB("0xffefe0")));
    colorIndexTable.push_back(ColorValue(0.75f, hexToRGB("0xffeddb")));
    colorIndexTable.push_back(ColorValue(0.80f, hexToRGB("0xffebd6")));
    colorIndexTable.push_back(ColorValue(0.85f, hexToRGB("0xffe9d2")));
    colorIndexTable.push_back(ColorValue(0.90f, hexToRGB("0xffe8ce")));
    colorIndexTable.push_back(ColorValue(0.95f, hexToRGB("0xffe6ca")));
    colorIndexTable.push_back(ColorValue(1.0f, hexToRGB("0xffe5c6")));
    colorIndexTable.push_back(ColorValue(1.05f, hexToRGB("0xffe3c3")));
    colorIndexTable.push_back(ColorValue(1.10f, hexToRGB("0xffe2bf")));
    colorIndexTable.push_back(ColorValue(1.15f, hexToRGB("0xffe0bb")));
    colorIndexTable.push_back(ColorValue(1.20f, hexToRGB("0xffdfb8")));
    colorIndexTable.push_back(ColorValue(1.25f, hexToRGB("0xffddb4")));
    colorIndexTable.push_back(ColorValue(1.30f, hexToRGB("0xffdbb0")));
    colorIndexTable.push_back(ColorValue(1.35f, hexToRGB("0xffdaad")));
    colorIndexTable.push_back(ColorValue(1.40f, hexToRGB("0xffd8a9")));
    colorIndexTable.push_back(ColorValue(1.45f, hexToRGB("0xffd6a5")));
    colorIndexTable.push_back(ColorValue(1.50f, hexToRGB("0xffd5a1")));
    colorIndexTable.push_back(ColorValue(1.55f, hexToRGB("0xffd29c")));
    colorIndexTable.push_back(ColorValue(1.60f, hexToRGB("0xffd096")));
    colorIndexTable.push_back(ColorValue(1.65f, hexToRGB("0xffcc8f")));
    colorIndexTable.push_back(ColorValue(1.70f, hexToRGB("0xffc885")));
    colorIndexTable.push_back(ColorValue(1.75f, hexToRGB("0xffc178")));
    colorIndexTable.push_back(ColorValue(1.80f, hexToRGB("0xffb765")));
    colorIndexTable.push_back(ColorValue(1.85f, hexToRGB("0xffa94b")));
    colorIndexTable.push_back(ColorValue(1.90f, hexToRGB("0xff9523")));
    colorIndexTable.push_back(ColorValue(1.95f, hexToRGB("0xff7b00")));
    colorIndexTable.push_back(ColorValue(2.0f, hexToRGB("0xff5200")));

    apMagModifier = 0.0f;
    absMagModifier = 0.0f;

    drawCatalog = false;
    showCiAbsGraph = false;
    useCustomColorFlag = false;
    customColor[0] = 0.0f;
    customColor[1] = 0.0f;
    customColor[2] = 0.0f;
    customColor[3] = 0.0f;

    unknownMagnitudeStarsCount = 0;
    bool unknownDistance;
    bool unknownMagnitude;

        std::string name;
        double ra, dec, dist, ma, absMa;
        glm::vec3 color;

        // Just to count null, not null, min and max values
        int nameNULLS, raNULLS, decNULLS, distNULLS, maNULLS, absMaNULLS, colorNULLS;
        nameNULLS = raNULLS = decNULLS = distNULLS = maNULLS = absMaNULLS = colorNULLS = 0;

        int nameNOTNULLS, raNOTNULLS, decNOTNULLS, distNOTNULLS, maNOTNULLS, absMaNOTNULLS, colorNOTNULLS;
        nameNOTNULLS = raNOTNULLS = decNOTNULLS = distNOTNULLS = maNOTNULLS = absMaNOTNULLS = colorNOTNULLS = 0;

        double raMIN, decMIN, distMIN, maMIN, absMaMIN, colorIndexMIN;
        raMIN = decMIN = distMIN = maMIN = absMaMIN = colorIndexMIN = std::numeric_limits<double>::max();

        double raMAX, decMAX, distMAX, maMAX, absMaMAX, colorIndexMAX;
        raMAX = decMAX = distMAX = maMAX = absMaMAX = colorIndexMAX = std::numeric_limits<double>::min();

        std::cout << "Loading stars from HYG Database...\n";
        std::ifstream file(filePath);
        if (file.is_open()) {
            std::string line;
            std::getline(file, line); // just to ignore the first info line

            while (std::getline(file, line)) {
                std::vector<std::string> splittedLine = splitLine(line, ',');

                if(splittedLine.size() == 8) { // Some lines have no color index value and the split function only return 8 values
                    splittedLine.push_back("");
                }

                // Name variable
                name = splittedLine[0];
                if(name == "") {
                    nameNULLS++;
                } else {
                    hasName = true;
                    nameNOTNULLS++;
                }

                // RA variable
                if(splittedLine[1] == "") raNULLS++; else raNOTNULLS++;
                ra = glm::radians(std::stod(splittedLine[1]) * 15.0);
                if(ra < raMIN) raMIN = ra;
                if(ra > raMAX) raMAX = ra;

                // DEC variable
                if(splittedLine[2] == "") decNULLS++; else decNOTNULLS++;
                dec = glm::radians(std::stod(splittedLine[2]));
                if(dec < decMIN) decMIN = dec;
                if(dec > decMAX) decMAX = dec;

                // Distance variable
                unknownDistance = true;
                if(splittedLine[3] == "" || splittedLine[3] == "100000.0000") distNULLS++; else distNOTNULLS++;
                if(splittedLine[3] == "100000.0000") {
                    //dist = 2000.0f + glm::abs(glm::gaussRand(0.0f, 1000.0f));
                    dist = 65000.0f;
                } else {
                    unknownDistance = false;
                    dist = std::stod(splittedLine[3]);
                    if(dist < distMIN) distMIN = dist;
                    if(dist > distMAX) distMAX = dist;
                }

                // Aparent Magnitude Variable
                //unknownMagnitude = true;
                //if(splittedLine[7] == "") {
                //    maNULLS++;
                //} else {
                //    maNOTNULLS++;
                //    unknownMagnitude = false;
                //}
                //ma = std::stod(splittedLine[7]);
                //if(ma < maMIN) maMIN = ma;
                //if(ma > maMAX) maMAX = ma;

                // Absolute Magnitude Variable
                if(splittedLine[7] == "") absMaNULLS++; else absMaNOTNULLS++;
                absMa = std::stod(splittedLine[7]);
                if(absMa < absMaMIN) absMaMIN = absMa;
                if(absMa > absMaMAX) absMaMAX = absMa;

                // Color variable
                float colorIndex;
                if(splittedLine[8] == "") colorNULLS++; else colorNOTNULLS++;
                if(splittedLine[8] == "") {
                    color = hexToRGB("0xffffff");
                    colorIndex = 0.4f;
                    color = getColor(colorIndex, colorIndexTable);
                } else {
                    colorIndex = std::stof(splittedLine[8]);
                    if(colorIndex < colorIndexMIN) colorIndexMIN = colorIndex;
                    if(colorIndex > colorIndexMAX) colorIndexMAX = colorIndex;
                    color = getColor(colorIndex, colorIndexTable);

                }

                // Proper motion ra dec radialVel 10 11 12
                float pmra, pmdec, rVel;
                pmra = pmdec = rVel = 0.0f;

                pmra = splittedLine[4] == "" ? 0.0 : std::stof(splittedLine[4]);
                pmdec = splittedLine[5] == "" ? 0.0 : std::stof(splittedLine[5]);
                rVel = splittedLine[6] == "" ? 0.0 : std::stof(splittedLine[6]);

                // Add star
                Star s = Star(ra,
                              dec,
                              dist,
                              absMa,
                              color,
                              colorIndex, //hexToRGB("0x00ff00"));
                              pmra,
                              pmdec,
                              rVel,
                              true);

                if(hasName) {
                    hasName = false;
                    starsWithName.push_back(starWithProperName(name, s));
                }

                if(!unknownMagnitude) { // Discart unknown mag entries
                    if (unknownDistance) {
                        unknownDistanceStars.push_back(s);
                    } else {
                        catalog.push_back(s);
                    }
                } else {
                    unknownMagnitudeStarsCount++;
                }

            }

            file.close();
        }
        else {
            std::cout << "Error reading stars file.\n";
        }

    // Order by distance
    std::sort(catalog.begin(), catalog.end(), StarDistanceComparator());

    catalogSize = catalog.size();

    // Init index-distance map
    indexToDistance.clear();
    DistanceToIndex.clear();
    for(int i = 0; i < catalog.size(); ++i) {
        indexToDistance.push_back(std::pair<int, float>(i, catalog[i].getDistance()));
        DistanceToIndex.insert(std::pair<float, int>(catalog[i].getDistance(), i));
    }

    distance.min = catalog.front().getDistance();
    distance.max = catalog.back().getDistance();

    absMag.min = std::numeric_limits<float>().max();
    absMag.max = std::numeric_limits<float>().min();

    BVColorIndex.min = std::numeric_limits<float>().max();
    BVColorIndex.max = std::numeric_limits<float>().min();

    speedModule.min = std::numeric_limits<float>().max();
    speedModule.max = std::numeric_limits<float>().min();

    for(int i = 0; i < catalog.size(); ++i) {
        if(catalog[i].getAbsMag() < absMag.min) absMag.min = catalog[i].getAbsMag();
        if(catalog[i].getAbsMag() > absMag.max) absMag.max = catalog[i].getAbsMag();

        if(catalog[i].speedModule < speedModule.min) speedModule.min = catalog[i].speedModule;
        if(catalog[i].speedModule > speedModule.max) speedModule.max = catalog[i].speedModule;


        if(catalog[i].getColorIndex() < BVColorIndex.min) BVColorIndex.min = catalog[i].getColorIndex();
        if(catalog[i].getColorIndex() > BVColorIndex.max) BVColorIndex.max = catalog[i].getColorIndex();
    }

    lowerAbsMagFilter = absMag.min;
    upperAbsMagFilter = absMag.max;

    lowerColorIndexFilter = BVColorIndex.min;
    upperColorIndexFilter = BVColorIndex.max;

    lowerSpeedModuleFilter = speedModule.min;
    upperSpeedModuleFilter = speedModule.max;

    knownDistanceStarsCount = catalog.size();
    unknownDistanceStarsCount = unknownDistanceStars.size();

    lowerDistanceFilterIndex = 1;
    upperDistanceFilterIndex = catalog.size();

    // Generate VAO
    glGenVertexArrays(1, &starsVAO);
    if(starsVAO == 0) throw std::runtime_error("Failed to create a VAO.\n");
    glBindVertexArray(starsVAO);

    glGenBuffers(1, &starsVBO);

    if(starsVBO == 0) throw std::runtime_error("Failed to create a VBO.\n");
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);

    // 0 ID
    glEnableVertexAttribArray(0);
    // NOTE THIS FUNCTION IS DIFFERENT THAN THE OTHERS BECAUSE WE PASS UNSIGNED INT VALUES
    glVertexAttribIPointer(0, sizeof(GLuint) / sizeof(GLuint), GL_UNSIGNED_INT, sizeof(Star), (GLuint*) offsetof(Star, id));

    for(unsigned int i = 0; i < catalog.size(); ++i) {
        catalog[i].id = starID;
        starID++;
    }
    firstID = catalog.front().id;
    lastID = catalog.back().id;

    // 1 Position (ra, dec)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, position));

    // 2 Distance
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, sizeof(GLfloat) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, distance));

    // 3 3D Position
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, position3D));

    // 4 Velocity
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, velocity));

    // 5 Absolute Magnitude
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, sizeof(GLfloat) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, absMag));

    // 6 Color Index
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, sizeof(GLfloat) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, colorIndex));

    // 7 Color (RGB)
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, color));

    // 8 Speed module
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, sizeof(GLfloat) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Star), (GLuint*) offsetof(Star, speedModule));

    // Copy to VRAM
    glBufferData(GL_ARRAY_BUFFER, sizeof(Star) * catalog.size(), catalog.data(), GL_STATIC_DRAW);

    // Generate IBOs
    glGenBuffers(1, &starsIBO);
    if(starsIBO == 0) throw std::runtime_error("Failed to create a IBO.\n");

    starsIndices.clear();
    for(unsigned int i = 0; i < catalog.size(); ++i) {
        starsIndices.push_back(i);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starsIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * starsIndices.size(), starsIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Clear stars Vectors
    unknownDistanceStars.clear();

    // Set indexFilter to catalog size
    indexFilter = 0;

    distanceFilter.min = 1.0f;
    distanceFilter.max = (float) catalogSize;
    distanceFilter.lower = 1.0f;
    distanceFilter.upper = 2.0f;

    colorFilter.min = BVColorIndex.min;
    colorFilter.max = BVColorIndex.max;
    colorFilter.lower = 1.0f;
    colorFilter.upper = 2.0f;

    absMagFilter.min = absMag.min;
    absMagFilter.max = absMag.max;
    absMagFilter.lower = 1.0f;
    absMagFilter.upper = 2.0f;

    velocityFilter.min = speedModule.min;
    velocityFilter.max = speedModule.max;
    velocityFilter.lower = 1.0f;
    velocityFilter.upper = 2.0f;

    catalog.clear();
}

void
StarCatalog::computeStats(stats& s) {

    float absSum = 0.0f;
    float ciSum = 0.0f;
    float distanceSum = 0.0f;
    float velocitySum = 0.0f;

    s.absMagMax = std::numeric_limits<float>().min();
    s.absMagMin = std::numeric_limits<float>().max();
    s.ciMax = std::numeric_limits<float>().min();
    s.ciMin = std::numeric_limits<float>().max();
    s.distanceMax = std::numeric_limits<float>().min();
    s.distanceMin = std::numeric_limits<float>().max();
    s.velocityMax = std::numeric_limits<float>().min();
    s.velocityMin = std::numeric_limits<float>().max();

    std::vector<std::array<float, 4> > values; // 0 abs, 1 ci, 2 distnce, 3 velocity
    values.clear();

    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
    Star* ptr = nullptr;
    ptr = (Star*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if(ptr != nullptr) {
        Star star;
        for(unsigned int i = 0; i < catalogSize; ++i) {
            star = ptr[i];
            if(checkFilters(star)) {
                std::array<float, 4> f = {star.absMag, star.colorIndex, star.distance, star.speedModule};
                values.push_back(f);

                if(star.absMag < s.absMagMin) s.absMagMin = star.absMag;
                if(star.absMag > s.absMagMax) s.absMagMax = star.absMag;

                if(star.colorIndex < s.ciMin) s.ciMin = star.colorIndex;
                if(star.colorIndex > s.ciMax) s.ciMax = star.colorIndex;

                if(star.distance < s.distanceMin) s.distanceMin = star.distance;
                if(star.distance > s.distanceMax) s.distanceMax = star.distance;

                if(star.speedModule < s.velocityMin) s.velocityMin = star.speedModule;
                if(star.speedModule > s.velocityMax) s.velocityMax = star.speedModule;
            }
        }
        // release pointer to mapped buffer after use
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        std::cout << "Error when trying to read a GL_ARRAY_BUFFER from VRAM.\n";
    }
    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Compute Average
    for(unsigned int i = 0; i < values.size(); ++i) {
        absSum      += values[i][0];
        ciSum       += values[i][1];
        distanceSum += values[i][2];
        velocitySum += values[i][3];
    }

    s.absMagAverage = absSum / values.size();
    s.ciAverage = ciSum / values.size();
    s.distanceAverage = distanceSum / values.size();
    s.velocityAverage = velocitySum / values.size();

    // Compute Variance
    for(unsigned int i = 0; i < values.size(); ++i) {
        values[i][0] = pow(values[i][0] - s.absMagAverage, 2.0f);
        values[i][1] = pow(values[i][1] - s.ciAverage, 2.0f);
        values[i][2] = pow(values[i][2] - s.distanceAverage, 2.0f);
        values[i][3] = pow(values[i][3] - s.velocityAverage, 2.0f);
    }

    absSum = ciSum = distanceSum = velocitySum = 0.0f;
    for(unsigned int i = 0; i < values.size(); ++i) {
        absSum      += values[i][0];
        ciSum       += values[i][1];
        distanceSum += values[i][2];
        velocitySum += values[i][3];
    }

    s.absMagVariance = absSum / values.size();
    s.ciVariance = ciSum / values.size();
    s.distanceVariance = distanceSum / values.size();
    s.velocityVariance = velocitySum / values.size();

    s.entries = values.size();

    values.clear();
}

bool
StarCatalog::checkFilters(Star& s) {
    return  s.absMag >= absMagFilter.lowerValue() &&
            s.absMag <= absMagFilter.upperValue() &&

            s.distance >= getLowerDistanceFloatFilter() &&
            s.distance <= getUpperDistanceFloatFilter() &&

            s.colorIndex >= colorFilter.lowerValue() &&
            s.colorIndex <= colorFilter.upperValue() &&

            s.speedModule >= velocityFilter.lowerValue() &&
            s.speedModule <= velocityFilter.upperValue();
}

bool
StarCatalog::saveToCSVFile(std::string path) {
    std::cout << "Saving CSV stars file...\n";

    std::string line;
    line = "ra,dec,parallax,phot_g_mean_mag,bp_g,pmra,pmdec,radial_velocity\n";

    std::cout << path << "\n";

    std::ofstream file(path);

    if (file.is_open()) {

        file << line;

        glBindVertexArray(starsVAO);
        glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
        Star* ptr = nullptr;
        ptr = (Star*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
        if(ptr != nullptr) {
            Star star;
            for(unsigned int i = 0; i < catalogSize; ++i) {
                star = ptr[i];
                if(checkFilters(star)) {
                    file << star.getCSVLine();
                    //std::cout << star.getCSVLine() << "\n";
                }
            }
            // release pointer to mapped buffer after use
            glUnmapBuffer(GL_ARRAY_BUFFER);
        } else {
            std::cout << "Error when trying to read a GL_ARRAY_BUFFER from VRAM.\n";
        }
        // unbind VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        file.close();
    }
    else {
        std::cout << "Error while saving file.\n";
        return false;
    }

    return true;
}

bool
StarCatalog::saveToFITSFile(std::string path) {

    std::cout << "Saving FITS stars file...\n";

    // Access one time to know how many stars we are going to save
    int starsCount = 0;
    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
    Star* ptr = nullptr;
    ptr = (Star*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if(ptr != nullptr) {
        Star star;
        for(unsigned int i = 0; i < catalogSize; ++i) {
            star = ptr[i];
            if(checkFilters(star)) {
                starsCount++;
            }
        }
        // release pointer to mapped buffer after use
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        std::cout << "Error when trying to read a GL_ARRAY_BUFFER from VRAM.\n";
        return false;
    }
    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create memory for those stars
    std::vector<std::valarray<float> > values(8);
    for(int i = 0; i < 8; ++i) {
        values[i] = std::valarray<float>(starsCount);
    }

    // We access again to read the stars
    starsCount = 0;
    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
    ptr = nullptr;
    ptr = (Star*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if(ptr != nullptr) {
        Star star;
        for(unsigned int i = 0; i < catalogSize; ++i) {
            star = ptr[i];
            if(checkFilters(star)) {
                glm::vec3 polarCoords = star.getPolarFromCartesian(star.position3D);
                polarCoords.x = glm::degrees(polarCoords.x);
                polarCoords.y = glm::degrees(polarCoords.y);

                values[0][starsCount] = polarCoords.x; // ra
                values[1][starsCount] = polarCoords.y; // dec
                values[2][starsCount] = star.getParallax(); // parallax
                values[3][starsCount] = star.getAppMag(); // phot_g_mean_mag
                values[4][starsCount] = star.colorIndex; // B-V
                glm::vec3 velParams = star.getVelocityParams();
                values[5][starsCount] = velParams.x; // pmra*
                values[6][starsCount] = velParams.y; // pmdec
                values[7][starsCount] = velParams.z; // rad vel
                starsCount++;
            }
        }
        // release pointer to mapped buffer after use
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        std::cout << "Error when trying to read a GL_ARRAY_BUFFER from VRAM.\n";
        return false;
    }
    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    CCfits::FITS* output_file(0);

    try {
        // Check if file alredy exits and delete it
        std::ifstream file(path);
        if(!file.is_open()){
        } else {
            std::cout << "Overwriting: " << path << std::endl;
            file.close();
            remove(path.c_str());
        }
        output_file = new CCfits::FITS(path, CCfits::Write);
    }
    catch (CCfits::FITS::CantOpen) {
        std::cout << "Error: Could not save FITS file.\n";
        return false;
    }

    std::vector<string> colName(8,"");
    std::vector<string> colForm(8,"E"); // E to specify IEEE float32 data type
    //std::vector<string> colForm(8,""); // E to specify IEEE float32 data type
    std::vector<string> colUnit(8,"");

    colName[0] = "ra";
    colName[1] = "dec";
    colName[2] = "parallax";
    colName[3] = "phot_g_mean_mag";
    colName[4] = "bp_g";
    colName[5] = "pmra";
    colName[6] = "pmdec";
    colName[7] = "radial_velocity";

    std::string hduName = "votable";
    unsigned long rows = starsCount;
    CCfits::Table* newTable;
    try {
        newTable = output_file->addTable(hduName, rows, colName, colForm, colUnit);
    } catch (CCfits::FitsError e) {
        std::cout << "Error: " << e.message();
        return false;
    }

    newTable->column(colName[0]).write(&values[0][0],starsCount,1);
    values[0].resize(0);
    newTable->column(colName[1]).write(&values[1][0],starsCount,1);
    values[1].resize(0);
    newTable->column(colName[2]).write(&values[2][0],starsCount,1);
    values[2].resize(0);
    newTable->column(colName[3]).write(&values[3][0],starsCount,1);
    values[3].resize(0);
    newTable->column(colName[4]).write(&values[4][0],starsCount,1);
    values[4].resize(0);
    newTable->column(colName[5]).write(&values[5][0],starsCount,1);
    values[5].resize(0);
    newTable->column(colName[6]).write(&values[6][0],starsCount,1);
    values[6].resize(0);
    newTable->column(colName[7]).write(&values[7][0],starsCount,1);
    values[7].resize(0);

    output_file->destroy();
    return true;
}

std::string
StarCatalog::getSourceFileName() {
    return sourceFileName;
}

void
StarCatalog::setLowerIndexForDistance(float dist) {

    filter& f = distanceFilter;

    int index = DistanceToIndex.upper_bound(dist)->second;
    if(index == 0) {
        index = 1;
        if(dist >= distance.max) {
            index = (int) distanceFilter.max;
        }
    }

    if(index <= (int) f.upperValue()) {
        lowerDistanceFilterIndex = index;

        distanceFilter.lower = 1.0f + ((float) index) / distanceFilter.max;
    }

}

bool
StarCatalog::isIdInCatalog(unsigned int id) {
    return id >= firstID && id <= lastID;
}

Star
StarCatalog::getStarWithId(unsigned int id) {

    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
    Star* ptr = nullptr;
    Star s;
    ptr = (Star*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if(ptr != nullptr) {

        s = ptr[id - firstID];

        // release pointer to mapped buffer after use
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        std::cout << "Error when trying to read a GL_ARRAY_BUFFER from VRAM.\n";
    }
    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return s;
}

void
StarCatalog::changeIndexFilter(bool b) {
    if(b) {
        indexFilter++;
    } else {
        indexFilter--;
    }

    if(indexFilter > starsIndices.size()) indexFilter = starsIndices.size();
}

float&
StarCatalog::getRefEpoch() {
    return refEpoch;
}

float&
StarCatalog::getEpoch() {
    return epoch;
}

float&
StarCatalog::getEpochSpeed() {
    return epochSpeed;
}

int&
StarCatalog::getEpochSpeedInt() {
    return epochSpeedInt;
}

std::string
StarCatalog::getDistanceFilterRangeText() {
    return "Distance range:\n(" + std::to_string(indexToDistance.at(lowerDistanceFilterIndex - 1).second) + ", "
    + std::to_string(indexToDistance.at(upperDistanceFilterIndex - 1).second) + ") parsecs";

}

std::string
StarCatalog::getColorIndexFilterRangeText() {
    return "(" + std::to_string(lowerColorIndexFilter) + " - " + std::to_string(upperColorIndexFilter) + ")";
}

float&
StarCatalog::getApMagModifier() {
    return apMagModifier;
}

float&
StarCatalog::getAbsMagModifier() {
    return absMagModifier;
}

filter&
StarCatalog::getDistanceFilter() {
    return distanceFilter;
}

filter&
StarCatalog::getAbsMagFilter() {
    return absMagFilter;
}

filter&
StarCatalog::getColorFilter() {
    return colorFilter;
}

filter&
StarCatalog::getVelocityFilter() {
    return velocityFilter;
}

limit&
StarCatalog::getDistanceLimits() {
    return distance;
}

limit&
StarCatalog::getAbsMagLimits() {
    return absMag;
}

limit&
StarCatalog::getColorIndexLimits() {
    return BVColorIndex;
}

limit&
StarCatalog::getSpeedModuleLimits() {
    return speedModule;
}

int&
StarCatalog::getLowerDistanceFilter() {
    return lowerDistanceFilterIndex;
}

int&
StarCatalog::getUpperDistanceFilter() {
    return upperDistanceFilterIndex;
}

float&
StarCatalog::getLowerDistanceFloatFilter() {
    return indexToDistance.at(lowerDistanceFilterIndex - 1).second;
}

float&
StarCatalog::getUpperDistanceFloatFilter() {
    return indexToDistance.at(upperDistanceFilterIndex - 1).second;
}

float&
StarCatalog::getLowerAbsMagFilter() {
    return lowerAbsMagFilter;
}

float&
StarCatalog::getUpperAbsMagFilter() {
    return upperAbsMagFilter;
}

float&
StarCatalog::getLowerColorIndexFilter() {
    return lowerColorIndexFilter;
}

float&
StarCatalog::getUpperColorIndexFilter() {
    return upperColorIndexFilter;
}

float&
StarCatalog::getLowerSpeedModuleFilter() {
    return lowerSpeedModuleFilter;
}

float&
StarCatalog::getUpperSpeedModuleFilter() {
    return upperSpeedModuleFilter;
}

void
StarCatalog::draw(renderMode r, ShaderProgram* sp, glm::mat4 vpm) {

    if(r == SPACE_3D) {
        sp->setUniform("epoch", epoch);

        this->draw();
    } else if(r == PLOT_2D) {
        this->draw();
    } else if(r == HR_DIAGRAM) {
        this->draw();
    }
}

void
StarCatalog::draw() {
    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starsIBO);

    if(upperDistanceFilterIndex >= lowerDistanceFilterIndex) {
        glDrawElements(GL_POINTS, upperDistanceFilterIndex - lowerDistanceFilterIndex + 1, GL_UNSIGNED_INT,
                       (void*)(sizeof(GLuint) * (lowerDistanceFilterIndex - 1)));
    }

    glBindVertexArray(0);
}

void
StarCatalog::drawVelocities() {

    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starsIBO);
    if(upperDistanceFilterIndex >= lowerDistanceFilterIndex) {
        glDrawElements(GL_POINTS, upperDistanceFilterIndex - lowerDistanceFilterIndex + 1, GL_UNSIGNED_INT,
                       (void*)(sizeof(GLuint) * (lowerDistanceFilterIndex - 1)));
    }
    glBindVertexArray(0);
}

void
StarCatalog::update2DCoords(glm::mat4 m) {
    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
    Star* ptr = nullptr;
    ptr = (Star*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if(ptr != nullptr) {

        Star star;
        glm::vec3 polarCoord;

        for(unsigned int i = 0; i < catalogSize; ++i) {
            star = ptr[i];
            polarCoord = polarFromCartesian(m * glm::vec4(star.position3D, 1.0f));
            ptr[i].position.x = polarCoord.x;
            ptr[i].position.y = polarCoord.y;
        }

        // release pointer to mapped buffer after use
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        std::cout << "Error when trying to read a GL_ARRAY_BUFFER from VRAM.\n";
    }

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void
StarCatalog::applyColorization(glm::vec3 firstColor, glm::vec3 secondColor, glm::vec3 thirdColor, float mixFactor, float min, float max, unsigned int offset) {
    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
    Star* ptr = nullptr;
    ptr = (Star*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if(ptr != nullptr) {

        Star star;
        glm::vec3 color;
        float param;
        float middlePoint = (max - min) / 2.0f;

        for(unsigned int i = 0; i < catalogSize; ++i) {
            star = ptr[i];

            float* f;

            param = *((float*)(((unsigned char*) &star) + offset));

            if(param < min) {
                color = firstColor;
            } else if(param > max) {
                color = thirdColor;
            } else {
                if(param < middlePoint) {
                    float percentage = glm::pow((param - min) / (middlePoint - min), mixFactor);
                    color = glm::mix(firstColor, secondColor, percentage);
                } else {
                    float percentage = glm::pow(1.0f - (param - middlePoint) / (max - middlePoint), mixFactor);
                    color = glm::mix(thirdColor, secondColor, percentage);

                }
            }
            ptr[i].color = color;
        }
        // release pointer to mapped buffer after use
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        std::cout << "Error when trying to read a GL_ARRAY_BUFFER from VRAM.\n";
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void
StarCatalog::resetBVColor() {

    std::vector<ColorValue> colorIndexTable;
    colorIndexTable.clear();
    colorIndexTable.push_back(ColorValue(-0.4f, hexToRGB("0x9bb2ff")));
    colorIndexTable.push_back(ColorValue(-0.35f, hexToRGB("0x9eb5ff")));
    colorIndexTable.push_back(ColorValue(-0.30f, hexToRGB("0xa3b9ff")));
    colorIndexTable.push_back(ColorValue(-0.25f, hexToRGB("0xaabfff")));
    colorIndexTable.push_back(ColorValue(-0.20f, hexToRGB("0xb2c5ff")));
    colorIndexTable.push_back(ColorValue(-0.15f, hexToRGB("0xbbccff")));
    colorIndexTable.push_back(ColorValue(-0.10f, hexToRGB("0xc4d2ff")));
    colorIndexTable.push_back(ColorValue(-0.05f, hexToRGB("0xccd8ff")));
    colorIndexTable.push_back(ColorValue(0.0f, hexToRGB("0xd3ddff")));
    colorIndexTable.push_back(ColorValue(0.05f, hexToRGB("0xdae2ff")));
    colorIndexTable.push_back(ColorValue(0.10f, hexToRGB("0xdfe5ff")));
    colorIndexTable.push_back(ColorValue(0.15f, hexToRGB("0xe4e9ff")));
    colorIndexTable.push_back(ColorValue(0.20f, hexToRGB("0xe9ecff")));
    colorIndexTable.push_back(ColorValue(0.25f, hexToRGB("0xeeefff")));
    colorIndexTable.push_back(ColorValue(0.30f, hexToRGB("0xf3f2ff")));
    colorIndexTable.push_back(ColorValue(0.35f, hexToRGB("0xf8f6ff")));
    colorIndexTable.push_back(ColorValue(0.40f, hexToRGB("0xfef9ff")));
    colorIndexTable.push_back(ColorValue(0.45f, hexToRGB("0xfff9fb")));
    colorIndexTable.push_back(ColorValue(0.50f, hexToRGB("0xfff7f5")));
    colorIndexTable.push_back(ColorValue(0.55f, hexToRGB("0xfff5ef")));
    colorIndexTable.push_back(ColorValue(0.60f, hexToRGB("0xfff3ea")));
    colorIndexTable.push_back(ColorValue(0.65f, hexToRGB("0xfff3ea")));
    colorIndexTable.push_back(ColorValue(0.70f, hexToRGB("0xffefe0")));
    colorIndexTable.push_back(ColorValue(0.75f, hexToRGB("0xffeddb")));
    colorIndexTable.push_back(ColorValue(0.80f, hexToRGB("0xffebd6")));
    colorIndexTable.push_back(ColorValue(0.85f, hexToRGB("0xffe9d2")));
    colorIndexTable.push_back(ColorValue(0.90f, hexToRGB("0xffe8ce")));
    colorIndexTable.push_back(ColorValue(0.95f, hexToRGB("0xffe6ca")));
    colorIndexTable.push_back(ColorValue(1.0f, hexToRGB("0xffe5c6")));
    colorIndexTable.push_back(ColorValue(1.05f, hexToRGB("0xffe3c3")));
    colorIndexTable.push_back(ColorValue(1.10f, hexToRGB("0xffe2bf")));
    colorIndexTable.push_back(ColorValue(1.15f, hexToRGB("0xffe0bb")));
    colorIndexTable.push_back(ColorValue(1.20f, hexToRGB("0xffdfb8")));
    colorIndexTable.push_back(ColorValue(1.25f, hexToRGB("0xffddb4")));
    colorIndexTable.push_back(ColorValue(1.30f, hexToRGB("0xffdbb0")));
    colorIndexTable.push_back(ColorValue(1.35f, hexToRGB("0xffdaad")));
    colorIndexTable.push_back(ColorValue(1.40f, hexToRGB("0xffd8a9")));
    colorIndexTable.push_back(ColorValue(1.45f, hexToRGB("0xffd6a5")));
    colorIndexTable.push_back(ColorValue(1.50f, hexToRGB("0xffd5a1")));
    colorIndexTable.push_back(ColorValue(1.55f, hexToRGB("0xffd29c")));
    colorIndexTable.push_back(ColorValue(1.60f, hexToRGB("0xffd096")));
    colorIndexTable.push_back(ColorValue(1.65f, hexToRGB("0xffcc8f")));
    colorIndexTable.push_back(ColorValue(1.70f, hexToRGB("0xffc885")));
    colorIndexTable.push_back(ColorValue(1.75f, hexToRGB("0xffc178")));
    colorIndexTable.push_back(ColorValue(1.80f, hexToRGB("0xffb765")));
    colorIndexTable.push_back(ColorValue(1.85f, hexToRGB("0xffa94b")));
    colorIndexTable.push_back(ColorValue(1.90f, hexToRGB("0xff9523")));
    colorIndexTable.push_back(ColorValue(1.95f, hexToRGB("0xff7b00")));
    colorIndexTable.push_back(ColorValue(2.0f, hexToRGB("0xff5200")));

    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
    Star* ptr = nullptr;
    ptr = (Star*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if(ptr != nullptr) {

        Star star;

        for(unsigned int i = 0; i < catalogSize; ++i) {
            star = ptr[i];
            ptr[i].color = getColor(star.colorIndex, colorIndexTable);
        }
        // release pointer to mapped buffer after use
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        std::cout << "Error when trying to read a GL_ARRAY_BUFFER from VRAM.\n";
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void
StarCatalog::nullDraw() {
    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starsIBO);

    glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}
