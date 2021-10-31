#include "shots.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Shots::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_shots.clear();

  // Create shot as a circle
  std::vector<glm::vec2> positions(0);
  std::vector<glm::vec4> colors(0);

  positions.emplace_back(0, 0);
  colors.emplace_back(1, 1, 1, 1);

  const auto step{M_PI * 2 / m_sides};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
    colors.emplace_back(1, 1, 1, 1);
  }
  positions.push_back(positions.at(1));
  colors.push_back(colors.at(1));

  // Generate VBOs
  abcg::glGenBuffers(1, &m_vboVertices);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glGenBuffers(1, &m_vboColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4),
                     colors.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  const GLint colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

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

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Shots::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);
  abcg::glUniform1f(m_rotationLoc, 0);
  abcg::glUniform2f(m_scaleLoc, m_scale, m_scale);

  for (const auto &shot : m_shots) {
    abcg::glUniform2f(m_translationLoc, shot.m_translation.x,
                      shot.m_translation.y);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, m_sides + 2);
  }

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Shots::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vboVertices);
  abcg::glDeleteBuffers(1, &m_vboColors);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Shots::update(Ship &ship, const GameData &gameData, float deltaTime) {
  if (gameData.m_state != State::Playing) return;

  // Create a pair of shots
  if (gameData.m_input[static_cast<size_t>(Input::Fire)]) {
    if (ship.m_shotCoolDownTimer.elapsed() > 100.0 / 1000.0) {
      ship.m_shotCoolDownTimer.restart();

      glm::vec2 forward{glm::vec2{0.0f, 1.0f}};
      glm::vec2 right{glm::vec2{1.0f, 0.0f}};
      const auto cannonOffset{(3.0f / 15.0f) * ship.m_scale};
      const auto shotSpeed{2.0f};

      Shot shot{.m_dead = false,
                .m_damage = 1,
                .m_translation = ship.m_translation + right * cannonOffset,
                .m_velocity = forward * shotSpeed};
      m_shots.push_back(shot);

      shot.m_translation = ship.m_translation - right * cannonOffset;
      m_shots.push_back(shot);
    }
  }

  for (auto &shot : m_shots) {
    shot.m_translation += shot.m_velocity * deltaTime;

    // Kill shot if it goes off screen
    if (shot.m_translation.x < -1.1f || shot.m_translation.x > +1.1f ||
        shot.m_translation.y < -1.1f || shot.m_translation.y > +1.1f)
      shot.m_dead = true;
  }

  // Remove dead shots
  m_shots.remove_if([](const Shot &x) { return x.m_dead; });
}
