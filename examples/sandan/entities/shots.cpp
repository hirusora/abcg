#include "shots.hpp"

#include <filesystem>
#include <glm/gtc/matrix_inverse.hpp>

void Shots::initializeGL(GLuint program, std::string path) {
  Model::initializeGL(program, path);

  auto directory{std::filesystem::path(path).parent_path().string()};
  auto filePath{directory + "/textures/white.png"};
  if (std::filesystem::exists(filePath)) {
    abcg::glDeleteTextures(1, &m_texture);
    m_texture = abcg::opengl::loadTexture(filePath);
    setDiffuseTexture(m_texture);
  }
  restart();
}

void Shots::paintGL(GameData gameData, glm::mat4 viewMatrix) {
  if (gameData.m_state != State::Playing) return;

  for (auto shot : m_shots) {
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, shot.m_translation);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(shot.m_scale));

    render(modelMatrix, viewMatrix);
  }
}

void Shots::terminateGL() { Model::terminateGL(); }

void Shots::restart() {
  m_shots.clear();
  m_timer.restart();
}

void Shots::update(float deltaTime) {
  for (auto &shot : m_shots) {
    shot.m_translation += shot.m_velocity * deltaTime;
  }

  for (auto &shot : m_shots) {
    if (shot.m_translation.x < -1.1f || shot.m_translation.x > 1.1f ||
        shot.m_translation.y < -1.1f || shot.m_translation.y > 1.1f ||
        shot.m_translation.z < -1.1f || shot.m_translation.z > 1.1f) {
      shot.m_dead = true;
    }
  }

  m_shots.erase(std::remove_if(m_shots.begin(), m_shots.end(),
                               [](const Shot &x) { return x.m_dead; }),
                m_shots.end());
}

void Shots::createBullet(glm::vec3 location) {
  if (m_timer.elapsed() < 0.2) return;

  auto forward{glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - location)};

  Shot shot{.m_translation = location + forward * 0.1f,
            .m_velocity = forward * 1.0f};
  m_shots.push_back(shot);

  m_timer.restart();
}