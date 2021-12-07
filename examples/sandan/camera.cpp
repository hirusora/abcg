#include "camera.hpp"

#include <fmt/core.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Camera::computeProjectionMatrix() {
  const auto aspect{m_viewportWidth / m_viewportHeight};
  m_projMatrix = glm::perspective(glm::radians(m_FOV), aspect, 0.1f, 100.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::setViewport(float width, float height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
  computeProjectionMatrix();
}

void Camera::restart() {
  m_eye = m_eyePosition;
  m_at = glm::vec3(0.0f, 0.0f, 0.0f);
  m_up = glm::vec3(0.0f, 1.0f, 0.0f);

  m_FOV = 20.0f;
}

void Camera::update(GameData gameData, float deltaTime) {
  // Camera movement
  const auto angularVelocity{static_cast<float>(M_PI_4)};
  const auto limit{static_cast<float>(M_PI_2 - M_PI_2 / 10)};

  if (gameData.m_input[static_cast<size_t>(Input::Left)]) {
    m_rotation.x -= angularVelocity * deltaTime;
  }
  if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
    m_rotation.x += angularVelocity * deltaTime;
  }
  if (gameData.m_input[static_cast<size_t>(Input::Up)]) {
    m_rotation.y += angularVelocity * deltaTime;
  }
  if (gameData.m_input[static_cast<size_t>(Input::Down)]) {
    m_rotation.y -= angularVelocity * deltaTime;
  }
  m_rotation.y = glm::clamp(m_rotation.y, -limit, limit);

  auto transform{glm::mat4(1.0f)};
  transform = glm::rotate(transform, m_rotation.x, glm::vec3(0, 1, 0));
  transform = glm::rotate(transform, m_rotation.y, glm::vec3(1, 0, 0));
  m_eye = transform * glm::vec4(m_eyePosition, 1.0f);
  computeViewMatrix();

  auto stepChanged{false};
  auto step{1.0f * deltaTime};
  if (gameData.m_input[static_cast<size_t>(Input::Forward)]) {
    stepChanged = true;
  }
  if (gameData.m_input[static_cast<size_t>(Input::Backward)]) {
    stepChanged = true;
    step *= -1;
  }

  auto zoomChanged{false};
  auto zoom{20.0f * deltaTime};
  if (gameData.m_input[static_cast<size_t>(Input::ZoomIn)]) {
    zoomChanged = true;
    zoom *= -1;
  }
  if (gameData.m_input[static_cast<size_t>(Input::ZoomOut)]) {
    zoomChanged = true;
  }

  if (stepChanged) {
    auto z = glm::clamp(m_eye.z + step, -2.4f, -1.6f);
    m_eye.z = z;
    computeViewMatrix();
  }

  if (zoomChanged) {
    m_FOV = glm::clamp(m_FOV + zoom, 10.0f, 45.0f);
    computeProjectionMatrix();
  }
}