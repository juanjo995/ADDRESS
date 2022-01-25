#include "OpenFileDialogWINDOWS.h"

// Returns an empty string if dialog is canceled
std::string openFileDialog() {
  OPENFILENAME ofn;
  char fileName[MAX_PATH] = "";
  ZeroMemory(&ofn, sizeof(ofn));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
  ofn.lpstrFile = fileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = "";

  if(GetOpenFileName(&ofn)) {
      return fileName;
  }
  else {
      return "";
  }

}

// Returns an empty string if dialog is canceled
std::string saveFileDialog() {

    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "newStarCatalog.csv";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "";

    if(GetSaveFileName(&ofn)) {
        return fileName;
    }
    else {
        return "";
    }

}