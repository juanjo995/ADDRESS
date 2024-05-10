#include "FileDialog.h"

std::string openFileDialog() {
    NFD_Init();

    std::string filePath = "";

    nfdchar_t *outPath;
    nfdfilteritem_t filterItem[0];
    nfdresult_t result = NFD_OpenDialog(&outPath, NULL, 0, NULL);
    if (result == NFD_OKAY)
    {
        filePath = outPath;
        NFD_FreePath(outPath);
    }
    else if (result == NFD_CANCEL)
    {
        std::cout << "User pressed cancel.\n";
    }
    else 
    {
        std::cout << "Error: " << NFD_GetError() << "\n";
    }

    NFD_Quit();
    return filePath;
}

// Returns an empty string if dialog is canceled
std::string saveFileDialog(std::string extension) {
    NFD_Init();

    std::string filePath = "";

    std::string fileName = "newStarCatalog." + extension;

    nfdchar_t *outPath;
    nfdfilteritem_t filterItem[0];
    nfdresult_t result = NFD_SaveDialog(&outPath, NULL, 0, NULL, fileName.c_str());
    if (result == NFD_OKAY)
    {
        filePath = outPath;
        NFD_FreePath(outPath);
    }
    else if (result == NFD_CANCEL)
    {
        std::cout << "User pressed cancel.\n";
    }
    else 
    {
        std::cout << "Error: " << NFD_GetError() << "\n";
    }

    NFD_Quit();
    return filePath;
}
