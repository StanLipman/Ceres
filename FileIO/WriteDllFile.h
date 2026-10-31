#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

inline bool WriteAllBytesToNewDll(const std::string& inputFile, const std::vector<uint8_t>& bytes)
{
    size_t lastSlash = inputFile.find_last_of("\\/");

    std::string inputDirectory = lastSlash == std::string::npos ? "" : inputFile.substr(0, lastSlash + 1);

    std::string fileName = inputFile.substr(lastSlash == std::string::npos ? 0 : lastSlash + 1);

    size_t lastDot = fileName.find_last_of('.');
    if (lastDot != std::string::npos) {
        fileName = fileName.substr(0, lastDot);
    }

    // TODO determine correct output directory https://github.com/StanLipman/Ceres/issues/5
    std::string outputDirectory = inputDirectory + "Output\\";
    std::string outputFile = outputDirectory + fileName + ".dll";

    std::ofstream file(outputFile, std::ios::binary | std::ios::trunc);

    if (!file.is_open())
        return false;

    file.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));

    return file.good();
}