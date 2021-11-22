#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Camera::computeProjectionMatrix(int width, int height) {
  const auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix = glm::perspective(glm::radians(m_FOV), aspect, +1.0f, -1.0f);
  // m_projMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::restart() { m_eye = glm::vec3(0.0f, 0.2f, -2.0f); }

void Camera::update(float deltaTime) {
  m_eye = glm::rotate(m_eye, m_angle * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
  computeViewMatrix();
}
