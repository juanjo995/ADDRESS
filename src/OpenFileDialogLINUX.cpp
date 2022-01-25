#include "OpenFileDialogLINUX.h"

// Returns an empty string if dialog is canceled
std::string openFileDialog() {
    std::string fileName = "";
    char filename[1024];
    //FILE *f = popen("zenity --file-selection", "r");
    FILE *f = popen("zenity --file-selection --filename=./datasets/default", "r");
    fgets(filename, 1024, f);
    int ret=pclose(f);
    if(ret != 0) {
        perror("file_name_dialog()");
        std::cout << "Something when wrong in load file dialog. Zenity ret value: " << ret << "\n";
    } else {
        fileName = std::string(filename);
        fileName.pop_back();
    }
    return fileName;
}

// Returns an empty string if dialog is canceled
std::string saveFileDialog(std::string extension) {
    std::string fileName = "";
    char filename[1024];
    //FILE *f = popen("zenity --file-selection", "r");
    // command -> "zenity --file-selection --filename=./datasets/newStarCatalog.csv --save --confirm-overwrite"
    std::string command = "zenity --file-selection --filename=./datasets/newStarCatalog.";
    command += extension;
    command += " --save --confirm-overwrite";
    FILE *f = popen(command.c_str(), "r");
    fgets(filename, 1024, f);
    int ret=pclose(f);
    if(ret != 0) {
        perror("file_name_dialog()");
        std::cout << "Something when wrong in load file dialog. Zenity ret value: " << ret << "\n";
    } else {
        fileName = std::string(filename);
        fileName.pop_back();
    }
    return fileName;
}