#pragma once
#include "../utils/glm_imports.h"

struct Transform
{
    glm::vec4 postition;
    glm::vec4 rotation;
    glm::vec4 scale;
};

class Object{

    Transform transform;

    //Constructor
    protected:
    Object();
    public: 
    

    //Getters and setters for the transform
    void set_position(glm::vec3 position);
    void set_rotation(glm::vec4 rotation);
    void set_scale(glm::vec3 scale);
    
    glm::vec3 get_position();
    glm::vec4 get_rotation();
    glm::vec3 get_scale();

    //Virtual functions
    virtual void update() = 0; //Only implementations of this class will have exclusive behaviour
};