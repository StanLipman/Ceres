#pragma once
#include <iostream>
#include "FileIO/WriteDllFile.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;

    // Test data
    std::vector<uint8_t> bytes = { 0x4D, 0x5A, 0x90, 0x00 };

    if (!WriteAllBytesToNewDll(argv[1], bytes))
        return 1;

    std::cout << "Succesfully created DLL" << std::endl;

    return 0;
}