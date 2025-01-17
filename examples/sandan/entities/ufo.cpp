#include "ufo.hpp"

#include <glm/gtc/matrix_inverse.hpp>

void Ufo::initializeGL(GLuint program, std::string path) {
  Model::initializeGL(program, path);
  restart();
}

void Ufo::paintGL(GameData gameData, glm::mat4 viewMatrix) {
  if (gameData.m_state != State::Playing) return;

  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, m_translation);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(m_scale));
  modelMatrix = glm::rotate(modelMatrix, static_cast<float>(M_PI_2),
                            glm::vec3(1.0f, 0.0f, 0.0f));

  render(modelMatrix, viewMatrix);
}

void Ufo::terminateGL() { Model::terminateGL(); }

void Ufo::restart() {
  m_translation = glm::vec3(0.0f, 0.0f, 0.0f);
  m_hp = m_baseHP;
}
