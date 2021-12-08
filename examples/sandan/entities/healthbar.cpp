#include "healthbar.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

void HealthBar::initializeGL(GLuint program) {
  // Create health bar rectangle
  std::array<glm::vec2, 4> positions{
      glm::vec2{-1.0f, m_height}, glm::vec2{1.0f, m_height},
      glm::vec2{-1.0f, -m_height}, glm::vec2{1.0f, -m_height}};
  std::array<glm::vec4, 4> colors{m_color, m_color, m_color, m_color};

  // Generate VBOs
  abcg::glGenBuffers(1, &m_VBOVertices);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenBuffers(1, &m_VBOColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  const GLint colorAttribute{abcg::glGetAttribLocation(program, "inColor")};

  // Get location of uniform attributes
  m_rotationLoc = abcg::glGetUniformLocation(program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(program, "translation");

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  restart();
}

void HealthBar::paintGL(GameData gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glBindVertexArray(m_VAO);
  abcg::glUniform1f(m_rotationLoc, 0.0f);
  abcg::glUniform2f(m_scaleLoc, m_scale.x, m_scale.y);
  abcg::glUniform2f(m_translationLoc, m_translation.x, m_translation.y);

  abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  abcg::glBindVertexArray(0);
}

void HealthBar::terminateGL() {
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void HealthBar::update(GameData gameData, Ufo enemy) {
  if (gameData.m_state != State::Playing) return;

  m_scale.x =
      static_cast<float>(enemy.getHP()) / static_cast<float>(enemy.getBaseHP());
}

void HealthBar::restart() {
  m_scale = glm::vec2(1.0f, 1.0f);
  m_translation = glm::vec2{0.0f, 1.0f - m_height};
}