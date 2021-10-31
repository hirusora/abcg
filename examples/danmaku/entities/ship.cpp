#include "ship.hpp"

#include <cppitertools/itertools.hpp>
#include <vector>

void Ship::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");

  m_translation = glm::vec2(0.0f, -0.8f);
  m_deaths = 0;
  m_state = ShipState::Normal;

  // Define ship renderization
  {
    std::array<glm::vec2, 35> positions{
        // Ship head
        glm::vec2{+00.0f, +15.0f}, glm::vec2{-02.0f, +13.0f},
        glm::vec2{+02.0f, +13.0f},
        // Ship body
        glm::vec2{-02.0f, +13.0f}, glm::vec2{+02.0f, +13.0f},
        glm::vec2{-02.0f, -03.0f}, glm::vec2{+02.0f, -03.0f},
        // Ship thruster
        glm::vec2{+02.0f, -03.0f}, glm::vec2{-02.0f, -03.0f},
        glm::vec2{+02.0f, -05.0f}, glm::vec2{-02.0f, -05.0f},
        // Left cannon
        glm::vec2{-02.0f, +06.5f}, glm::vec2{-04.0f, +06.5f},
        glm::vec2{-02.0f, +02.0f}, glm::vec2{-04.0f, +02.0f},
        // Right cannon
        glm::vec2{+02.0f, +06.5f}, glm::vec2{+04.0f, +06.5f},
        glm::vec2{+02.0f, +02.0f}, glm::vec2{+04.0f, +02.0f},
        // Ship left wing
        glm::vec2{-02.0f, +05.0f}, glm::vec2{-07.0f, +02.0f},
        glm::vec2{-02.0f, -03.0f}, glm::vec2{-07.0f, -05.0f},
        glm::vec2{-15.0f, +10.0f},
        // Ship right wing
        glm::vec2{+02.0f, +05.0f}, glm::vec2{+07.0f, +02.0f},
        glm::vec2{+02.0f, -03.0f}, glm::vec2{+07.0f, -05.0f},
        glm::vec2{+15.0f, +10.0f},
        // Thrusters
        glm::vec2{+02.0f, -05.0f}, glm::vec2{-02.0f, -05.0f},
        glm::vec2{+00.0f, -09.0f}, glm::vec2{+02.0f, -05.0f},
        glm::vec2{-02.0f, -05.0f}, glm::vec2{+00.0f, -15.0f}};

    // Normalize
    for (auto &position : positions) {
      position /= 15.0f;
    }

    std::array<glm::vec4, 35> colors{
        // Ship head
        m_mainColor, m_mainColor, m_mainColor,
        // Ship body
        m_bodyColor, m_bodyColor, m_bodyColor, m_bodyColor,
        // Ship thruster
        m_engineColor, m_engineColor, m_engineColor, m_engineColor,
        // Left cannon
        m_engineColor, m_engineColor, m_engineColor, m_engineColor,
        // Right cannon
        m_engineColor, m_engineColor, m_engineColor, m_engineColor,
        // Ship left wing
        m_mainColor, m_mainColor, m_mainColor, m_mainColor, m_mainColor,
        // Ship right wing
        m_mainColor, m_mainColor, m_mainColor, m_mainColor, m_mainColor,
        // Thrusters
        m_innerThrusterColor, m_innerThrusterColor, m_innerThrusterColor,
        m_outerThrusterColor, m_outerThrusterColor, m_outerThrusterColor};

    const std::array indices{// Ship head
                             0, 1, 2,
                             // Ship body
                             3, 4, 5, 4, 5, 6,
                             // Ship thruster
                             7, 8, 9, 8, 9, 10,
                             // Left cannon
                             11, 12, 13, 12, 13, 14,
                             // Right cannon
                             15, 16, 17, 16, 17, 18,
                             // Ship left wing
                             19, 20, 21, 20, 21, 22, 20, 22, 23,
                             // Ship right wing
                             24, 25, 26, 25, 26, 27, 25, 27, 28,
                             // Thrusters
                             29, 30, 31, 32, 33, 34};

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

  // Define ship's core renderization
  {
    m_core = Ship::Core{};

    std::vector<glm::vec2> positions(0);
    std::vector<glm::vec4> colors(0);

    positions.emplace_back(0, 0);
    colors.emplace_back(1, 1, 1, 1);

    const auto step{M_PI * 2 / m_core.m_sides};
    for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
      positions.emplace_back(std::cos(angle), std::sin(angle));
      colors.push_back(m_coreColor);
    }
    positions.push_back(positions.at(1));
    colors.emplace_back(colors.at(1));

    // Generate VBOs
    abcg::glGenBuffers(1, &m_core.m_vboVertices);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_core.m_vboVertices);
    abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                       positions.data(), GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glGenBuffers(1, &m_core.m_vboColors);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_core.m_vboColors);
    abcg::glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4),
                       colors.data(), GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Get location of attributes in the program
    const GLint positionAttribute{
        abcg::glGetAttribLocation(m_program, "inPosition")};
    const GLint colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

    // Create VAO
    abcg::glGenVertexArrays(1, &m_core.m_vao);

    // Bind vertex attributes to current VAO
    abcg::glBindVertexArray(m_core.m_vao);

    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_core.m_vboVertices);
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                                nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_core.m_vboColors);
    abcg::glEnableVertexAttribArray(colorAttribute);
    abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0,
                                nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // End of binding to current VAO
    abcg::glBindVertexArray(0);
  }
}

void Ship::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  // Render ship
  {
    abcg::glBindVertexArray(m_vao);

    abcg::glUniform2f(m_scaleLoc, m_scale, m_scale);
    abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    abcg::glUniform1f(m_rotationLoc, m_rotation);

    abcg::glEnable(GL_BLEND);
    abcg::glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Restart thruster blink timer every 100 ms
    if (m_trailBlinkTimer.elapsed() > 0.1) m_trailBlinkTimer.restart();
    if (m_trailBlinkTimer.elapsed() < 0.05) {
      abcg::glDrawElements(GL_TRIANGLES, 17 * 3, GL_UNSIGNED_INT, nullptr);
    }

    // Blink when invulnerable
    if (m_state == ShipState::Invulnerable) {
      if (m_invulnerableBlinkTimer.elapsed() > 0.2)
        m_invulnerableBlinkTimer.restart();
      auto alpha{(m_invulnerableBlinkTimer.elapsed() < 0.1) ? 0.2f : 0.7f};
      abcg::glBlendColor(0.0f, 0.0f, 0.0f, alpha);
    } else {
      abcg::glBlendColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    abcg::glDrawElements(GL_TRIANGLES, 15 * 3, GL_UNSIGNED_INT, nullptr);

    abcg::glDisable(GL_BLEND);

    abcg::glBindVertexArray(0);
  }

  // Render ship core
  if (gameData.m_input[static_cast<size_t>(Input::Focus)]) {
    abcg::glBindVertexArray(m_core.m_vao);

    abcg::glUniform2f(m_scaleLoc, m_core.m_scale, m_core.m_scale);
    abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, m_core.m_sides + 2);

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Ship::terminateGL() {
  abcg::glDeleteBuffers(1, &m_core.m_vboVertices);
  abcg::glDeleteBuffers(1, &m_core.m_vboColors);
  abcg::glDeleteVertexArrays(1, &m_core.m_vao);

  abcg::glDeleteBuffers(1, &m_vboVertices);
  abcg::glDeleteBuffers(1, &m_vboColors);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Ship::update(const GameData &gameData, float deltaTime) {
  if (gameData.m_state != State::Playing) return;

  if (m_invulnerableTimer.elapsed() > m_invulnerabilitySeconds)
    m_state = ShipState::Normal;

  glm::vec2 velocity{0.0f, 0.0f};
  auto velocity_speed{m_velocity_speed};

  if (gameData.m_input[static_cast<size_t>(Input::Focus)]) {
    velocity_speed /= 2.0f;
  }

  if (gameData.m_input[static_cast<size_t>(Input::Up)])
    velocity.y += velocity_speed;
  if (gameData.m_input[static_cast<size_t>(Input::Down)])
    velocity.y -= velocity_speed;
  if (gameData.m_input[static_cast<size_t>(Input::Right)])
    velocity.x += velocity_speed;
  if (gameData.m_input[static_cast<size_t>(Input::Left)])
    velocity.x -= velocity_speed;

  m_translation += velocity * deltaTime;

  // Checking border collision
  if (m_translation.x - m_scale < -1.0f)
    m_translation += glm::vec2(velocity_speed, 0.0f) * deltaTime;
  if (m_translation.x + m_scale > +1.0f)
    m_translation -= glm::vec2(velocity_speed, 0.0f) * deltaTime;
  if (m_translation.y - m_scale < -1.0f)
    m_translation += glm::vec2(0.0f, velocity_speed) * deltaTime;
  if (m_translation.y + m_scale > +1.0f)
    m_translation -= glm::vec2(0.0f, velocity_speed) * deltaTime;
}

void Ship::takeHit() {
  m_deaths++;
  m_state = ShipState::Invulnerable;
  m_invulnerableTimer.restart();
  m_invulnerableBlinkTimer.restart();
}