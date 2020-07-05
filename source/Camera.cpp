#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
Camera::Camera() {
    eye_pos = glm::vec3(0.0,0.0,0.0);
    up = glm::vec3(0.0,1.0,0.0);
    axes = glm::vec2(0.0,0.0);
}
void Camera::setPersperctive(float fovy,float aspect,float near,float far) {
    proj_mat = glm::perspective(glm::radians(fovy), aspect, near, far);
}
void Camera::walk(glm::vec3 dir, float dist) {
    eye_pos += dir*dist;

}
void Camera::look(float yaw, float pitch) {
	if (axes.x + pitch < 90 && axes.x + pitch > -90) {
		axes.x += pitch;
	}
	axes.y -= yaw;
}
glm::mat4 Camera::getMvp(){

    float radYaw = glm::radians(axes.y);
    float radPitch = glm::radians(axes.x);
    view= glm::vec3(cos(radYaw)*cos(radPitch),sin(radPitch),cos(radPitch)*sin(radYaw));
    glm::mat4 model_mat = glm::lookAt(eye_pos, eye_pos + view, up);

    return proj_mat * model_mat;
}
glm::mat4 Camera::getModel() {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), -eye_pos);
	return model;
}