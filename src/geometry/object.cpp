#include "object.h"

Object::Object(){

    //Position
    transform.postition.x = 0;
    transform.postition.y = 0;
    transform.postition.z = 0;
    transform.postition.w = 1;
    //Rotation
    transform.rotation.x = 0;
    transform.rotation.y = 0;
    transform.rotation.z = 0;
    transform.rotation.w = 1;
    //Scale
    transform.scale.x = 1;
    transform.scale.y = 1;
    transform.scale.z = 1;
    transform.scale.w = 0;
}

void Object::set_position(glm::vec3 position){
    transform.postition.x = position.x;
    transform.postition.y = position.y;
    transform.postition.z = position.z;
}
void Object::set_rotation(glm::vec4 rotation){
    transform.rotation.x = rotation.x;
    transform.rotation.y = rotation.y;
    transform.rotation.z = rotation.z;
    transform.rotation.w = rotation.w;
}
void Object::set_scale(glm::vec3 scale){
    transform.scale.x = scale.x;
    transform.scale.y = scale.y;
    transform.scale.z = scale.z;
}
    
glm::vec3 Object::get_position(){
    return transform.postition;
}

glm::vec4 Object::get_rotation(){
    return transform.rotation;
}
glm::vec3 Object::get_scale(){
    return transform.scale;
}