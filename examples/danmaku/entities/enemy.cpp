#include "enemy.hpp"

#include <glm/gtx/fast_trigonometry.hpp>

void Enemy::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_translation = glm::vec2(0.0f, 0.7f);
  m_rotation = M_PI;
  m_hp = m_maxHP;

  m_re.seed(std::chrono::steady_clock::now().time_since_epoch().count());

  m_targetTranslation = m_translation;
  m_targetReached = false;

  std::array<glm::vec2, 27> positions{
      // Ship head
      glm::vec2{+00.0f, +10.0f}, glm::vec2{-02.0f, +07.0f},
      glm::vec2{+02.0f, +07.0f},
      // Ship body
      glm::vec2{-02.0f, +07.0f}, glm::vec2{+02.0f, +07.0f},
      glm::vec2{-02.0f, -10.0f}, glm::vec2{+02.0f, -10.0f},
      // Ship left wing connection
      glm::vec2{-02.0f, +01.0f}, glm::vec2{-05.0f, +01.0f},
      glm::vec2{-02.0f, -01.0f}, glm::vec2{-05.0f, -01.0f},
      // Ship right wing connection
      glm::vec2{+02.0f, +01.0f}, glm::vec2{+05.0f, +01.0f},
      glm::vec2{+02.0f, -01.0f}, glm::vec2{+05.0f, -01.0f},
      // Ship left wing
      glm::vec2{-05.0f, +10.0f}, glm::vec2{-10.0f, +05.0f},
      glm::vec2{-05.0f, +05.0f}, glm::vec2{-05.0f, -05.0f},
      glm::vec2{-10.0f, -05.0f}, glm::vec2{-05.0f, -10.0f},
      // Ship right wing
      glm::vec2{+05.0f, +10.0f}, glm::vec2{+10.0f, +05.0f},
      glm::vec2{+05.0f, +05.0f}, glm::vec2{+05.0f, -05.0f},
      glm::vec2{+10.0f, -05.0f}, glm::vec2{+05.0f, -10.0f}};

  // Normalize
  for (auto &position : positions) {
    position /= 10.0f;
  }

  std::array<glm::vec4, 27> colors{
      // Ship head
      m_mainColor, m_mainColor, m_mainColor,
      // Ship body
      m_bodyColor, m_bodyColor, m_bodyColor, m_bodyColor,
      // Ship left wing connection
      m_engineColor, m_engineColor, m_engineColor, m_engineColor,
      // Ship right wing connection
      m_engineColor, m_engineColor, m_engineColor, m_engineColor,
      // Ship left wing
      m_mainColor, m_mainColor, m_mainColor, m_mainColor, m_mainColor,
      m_mainColor,
      // Ship right wing
      m_mainColor, m_mainColor, m_mainColor, m_mainColor, m_mainColor,
      m_mainColor};

  const std::array indices{// Ship head
                           0, 1, 2,
                           // Ship body
                           3, 4, 5, 5, 4, 6,
                           // Ship left wing connection
                           7, 8, 9, 8, 9, 10,
                           // Ship right wing connection
                           11, 12, 13, 12, 13, 14,
                           // Ship left wing
                           15, 16, 17, 16, 17, 19, 17, 18, 19, 18, 19, 20,
                           // Ship right wing
                           21, 22, 23, 22, 23, 25, 23, 24, 25, 24, 25, 26};

  // Generate VBOs
  abcg::glGenBuffers(1, &m_vboVertices);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenBuffers(1, &m_vboColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};
  GLint colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Enemy::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform2f(m_scaleLoc, m_scale, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glDrawElements(GL_TRIANGLES, 15 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Enemy::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vboVertices);
  abcg::glDeleteBuffers(1, &m_vboColors);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Enemy::update(const GameData &gameData, float deltaTime) {
  if (gameData.m_state != State::Playing) return;

  // Updates translation
  if (glm::distance(m_translation, m_targetTranslation) > 0.01) {
    m_translation += m_velocity * deltaTime;
  } else if (!m_targetReached) {
    m_targetReached = true;

    // Randomizes how much time the enemy will stay in its target position
    std::uniform_real_distribution rd(3.0f, 5.0f);
    m_targetDuration = rd(m_re);
    m_targetTimer.restart();
  }

  // Randomize enemy movement by calculating new target and velocity.
  if (m_targetReached && m_targetTimer.elapsed() > m_targetDuration) {
    std::uniform_real_distribution rx(-0.70, 0.70);
    std::uniform_real_distribution ry(0.55, 0.70);
    std::uniform_real_distribution rt(1.0f, 3.0f);

    auto time{rt(m_re)};

    m_targetTranslation = glm::vec2(rx(m_re), ry(m_re));
    m_velocity = (m_targetTranslation - m_translation) / time;

    m_targetReached = false;
  }
}
