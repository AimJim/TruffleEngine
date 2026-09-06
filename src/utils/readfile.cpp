#include "readfile.h"

#include <fstream>

std::vector<char> readFile(const std::string &filename){
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if(!file.is_open()){
        throw std::runtime_error("failed to open file: " + filename);

    }

    size_t filesize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(filesize);
    file.seekg(0);
    file.read(buffer.data(), filesize);
    file.close();
    return buffer;
}