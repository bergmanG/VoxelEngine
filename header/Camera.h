#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
class Camera {
public:
    glm::vec2 axes;//yaw and pitch
    glm::vec3 eye_pos;
    glm::vec3 view;
    glm::vec3 up;
    glm::mat4 proj_mat;

    Camera();
    void walk(glm::vec3 dir, float dist);
    void look(float yaw,float pitch);
    void setPersperctive(float fovy,float aspect,float near,float far);
    glm::mat4 getMvp();
	glm::mat4 getModel();
private:
};

#endif // CAMERA_H
