#ifndef OPENFILEDIALOGWINDOWS_H
#define OPENFILEDIALOGWINDOWS_H

#include <windows.h>
#include <string.h>
#include <iostream>

// Returns an empty string if dialog is canceled
std::string openFileDialog();

// Returns an empty string if dialog is canceled
std::string saveFileDialog(std::string extension);

#endif //OPENFILEDIALOGWINDOWS_H
