#include "ufo.hpp"

#include <glm/gtc/matrix_inverse.hpp>

void Ufo::initializeGL(GLuint program, std::string assetPath) {
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

void Ufo::paintGL(glm::mat4 viewMatrix) {
  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, m_translation);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(m_scale));
  modelMatrix = glm::rotate(modelMatrix, static_cast<float>(M_PI_2),
                            glm::vec3(1.0f, 0.0f, 0.0f));

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

void Ufo::terminateGL() { Model::terminateGL(); }

void Ufo::restart() { m_translation = glm::vec3(0.0f, 0.0f, 0.0f); }
