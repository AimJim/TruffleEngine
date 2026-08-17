#pragma once

#include <string>

#include "../utils/glm_imports.h"
#include <vector>

struct Vertex{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    bool operator==(const Vertex &other) const{
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

template <>
struct std::hash<Vertex>
{
	size_t operator()(Vertex const &vertex) const noexcept
	{
		return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
	}
};

class Mesh{

    std::vector<Vertex>* vertices;
    std::vector<uint32_t>* indices;

    public:
    Mesh(std::string model_path);
    Mesh();
    ~Mesh();

};