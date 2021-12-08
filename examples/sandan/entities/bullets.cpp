#include "bullets.hpp"

#include <glm/gtc/matrix_inverse.hpp>

void Bullets::initializeGL(GLuint program, std::string path) {
  Model::initializeGL(program, path);
  setMappingMode(2);
  restart();
}

void Bullets::paintGL(GameData gameData, glm::mat4 viewMatrix) {
  if (gameData.m_state != State::Playing) return;

  for (auto bullet : m_bullets) {
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, bullet.m_translation);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(bullet.m_scale));

    setDiffuseTexture(bullet.m_texture);
    render(modelMatrix, viewMatrix);
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