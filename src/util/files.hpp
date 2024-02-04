#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

inline std::string readFile(std::string path) {
    std::string text;

    std::fstream stream(path, std::ios::in);
    if(stream.is_open()) {
        std::stringstream stringStream;
        stringStream << stream.rdbuf();
        text = stringStream.str();
        stream.close();
    } else {
        std::cerr << "Failed to open file " << path << std::endl;
        std::exit(-1);
    }

    return text;
}