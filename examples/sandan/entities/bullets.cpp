#include "bullets.hpp"

#include <glm/gtc/matrix_inverse.hpp>

void Bullets::initializeGL(GLuint program, std::string assetPath) {
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

void Bullets::paintGL(glm::mat4 viewMatrix) {
  auto Ka{getKa()};
  auto Kd{getKd()};
  auto Ks{getKs()};

  abcg::glUniform1f(m_shininessLoc, getShininess());
  abcg::glUniform4fv(m_KaLoc, 1, &Ka.x);
  abcg::glUniform4fv(m_KdLoc, 1, &Kd.x);
  abcg::glUniform4fv(m_KsLoc, 1, &Ks.x);

  for (auto bullet : m_bullets) {
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, bullet.m_translation);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(bullet.m_scale));
    abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    const auto modelViewMatrix{glm::mat3(viewMatrix * modelMatrix)};
    const glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE,
                             &normalMatrix[0][0]);

    render();
  }
}

void Bullets::terminateGL() { Model::terminateGL(); }

void Bullets::restart() { m_bullets.clear(); }

void Bullets::update(float deltaTime) {
  for (auto &bullet : m_bullets) {
    bullet.m_translation += bullet.m_velocity * deltaTime;
  }

  for (auto &bullet : m_bullets) {
    if (bullet.m_translation.x < -1.1f || bullet.m_translation.x > 1.1f ||
        bullet.m_translation.y < -1.1f || bullet.m_translation.y > 1.1f ||
        bullet.m_translation.z < -1.1f || bullet.m_translation.z > 1.1f) {
      bullet.m_dead = true;
    }
  }

  m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
                                 [](const Bullet &x) { return x.m_dead; }),
                  m_bullets.end());
}