#include "bullets.hpp"

void Bullets::initializeGL(GLuint program, std::string assetsPath) {
  m_program = program;

  loadObj(assetsPath + "starship.obj");
  setupVAO(m_program);

  m_modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");

  create(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3(0.0f, 1.0f, 0.0f));
}

void Bullets::paintGL() {
  for (auto bullet : m_bullets) {
    abcg::glUniform4fv(m_colorLoc, 1, &bullet.m_color[0]);

    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, bullet.m_translation);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(bullet.m_scale));
    abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    render();
  }
}

void Bullets::terminateGL() { terminateGL(); }

void Bullets::create(glm::vec3 start, glm::vec3 velocity, glm::vec3 forward) {
  Bullet bullet{
      .m_scale = 0.5f,
      .m_translation = start,
      .m_velocity = velocity,
      .m_forward = forward,
  };

  m_bullets.push_back(bullet);
}

void Bullets::update(float deltaTime) {
  for (auto &bullet : m_bullets) {
    bullet.m_translation += bullet.m_velocity * deltaTime;
  }
}