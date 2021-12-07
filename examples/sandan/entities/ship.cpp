#include "ship.hpp"

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <limits>

const auto epsilon{std::numeric_limits<float>::epsilon()};

void Ship::initializeGL(GLuint program, std::string assetPath) {
  m_program = program;

  loadObj(assetPath, true);
  setupVAO(m_program);

  m_modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_normalMatrixLoc = abcg::glGetUniformLocation(m_program, "normalMatrix");
  m_shininessLoc = abcg::glGetUniformLocation(m_program, "shininess");
  m_KaLoc = abcg::glGetUniformLocation(m_program, "Ka");
  m_KdLoc = abcg::glGetUniformLocation(m_program, "Kd");
  m_KsLoc = abcg::glGetUniformLocation(m_program, "Ks");

  restart();
}

void Ship::paintGL(glm::mat4 viewMatrix) {
  auto transform{glm::mat4(1.0f)};
  transform = glm::rotate(transform, m_rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
  transform = glm::rotate(transform, m_rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::scale(modelMatrix, glm::vec3(m_scale));
  modelMatrix = glm::translate(modelMatrix, m_translation);
  modelMatrix = transform * modelMatrix;
  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

  const auto modelViewMatrix{glm::mat3(viewMatrix * modelMatrix)};
  const glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  auto Ka{getKa()};
  auto Kd{getKd()};
  auto Ks{getKs()};

  abcg::glUniform1f(m_shininessLoc, getShininess());
  abcg::glUniform4fv(m_KaLoc, 1, &Ka.x);
  abcg::glUniform4fv(m_KdLoc, 1, &Kd.x);
  abcg::glUniform4fv(m_KsLoc, 1, &Ks.x);

  render();
}

void Ship::terminateGL() { Model::terminateGL(); }

void Ship::restart() {
  m_translation = glm::vec3(0.0f, 0.0f, -1.0f);
  m_rotation = glm::vec3(0.0f);
}

void Ship::update(GameData gameData, float deltaTime) {
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

  // Forward and Backward movement
  auto stepChanged{false};
  auto step{1.0f * deltaTime};
  if (gameData.m_input[static_cast<size_t>(Input::Forward)]) {
    stepChanged = true;
  }
  if (gameData.m_input[static_cast<size_t>(Input::Backward)]) {
    stepChanged = true;
    step *= -1;
  }

  if (stepChanged) {
    auto z = glm::clamp(m_translation.z + step, -1.0f, -0.2f);
    m_translation.z = z;
  }
}