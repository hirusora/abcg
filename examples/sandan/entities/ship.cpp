#include "ship.hpp"

#include <fmt/core.h>

#include <glm/gtc/matrix_inverse.hpp>

void Ship::initializeGL(GLuint program, std::string path) {
  Model::initializeGL(program, path);
  restart();
}

void Ship::paintGL(GameData gameData, glm::mat4 viewMatrix) {
  if (gameData.m_state != State::Playing) return;

  auto transform{glm::mat4(1.0f)};
  transform = glm::rotate(transform, m_rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
  transform = glm::rotate(transform, m_rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, m_translation);
  modelMatrix = transform * modelMatrix;
  modelMatrix = glm::scale(modelMatrix, glm::vec3(m_scale));

  if (!m_focused) {
    render(modelMatrix, viewMatrix);
  } else {
    abcg::glEnable(GL_BLEND);
    abcg::glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    abcg::glBlendColor(0.0f, 0.0f, 0.0f, 0.2f);
    render(modelMatrix, viewMatrix);
    abcg::glDisable(GL_BLEND);
  }
}

void Ship::terminateGL() { Model::terminateGL(); }

void Ship::restart() {
  m_deaths = 0;
  m_focused = false;
  m_translation = glm::vec3(0.0f, 0.0f, -0.8f);
  m_rotation = glm::vec3(0.0f);
  m_invulnerableTimer.restart();
}

void Ship::update(GameData gameData, float deltaTime) {
  m_focused = gameData.m_input[static_cast<size_t>(Input::Focus)];

  // Camera movement
  const auto limit{static_cast<float>(M_PI_2 - M_PI_2 / 10)};
  auto angularVelocity{static_cast<float>(M_PI_4)};
  auto velocity{0.5f};

  if (m_focused) {
    angularVelocity /= 2.0f;
    velocity /= 2.0f;
  }

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

  // Forward and Backward movement
  if (gameData.m_input[static_cast<size_t>(Input::Forward)]) {
    m_translation.z += velocity * deltaTime;
  }
  if (gameData.m_input[static_cast<size_t>(Input::Backward)]) {
    m_translation.z -= velocity * deltaTime;
  }
  m_translation.z = glm::clamp(m_translation.z, -0.8f, -0.3f);
}

glm::vec3 Ship::getCoreLocation() {
  auto transform{glm::mat4(1.0f)};
  transform = glm::rotate(transform, m_rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
  transform = glm::rotate(transform, m_rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
  return transform * glm::vec4(m_translation, 1.0f);
}

void Ship::takeHit() {
  m_deaths++;
  m_invulnerableTimer.restart();
}