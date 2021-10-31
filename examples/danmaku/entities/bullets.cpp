#include "bullets.hpp"

#include <chrono>
#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Bullets::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_bullets.clear();

  m_re.seed(std::chrono::steady_clock::now().time_since_epoch().count());

  // Randomizes initial bullet wave rotation
  std::uniform_real_distribution rd(0.0f, 1.0f);
  m_rotation = rd(m_re) * M_PI / 2;

  m_angularVelocity = 0.0f;
  m_angularAcceleration = m_angularAccelerationStep;
  m_targetAngularVelocity = 0.0f;
  m_targetReached = false;

  m_bulletTimer.restart();
}

void Bullets::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glEnable(GL_BLEND);
  abcg::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Renders each bullet.
  for (const auto &bullet : m_bullets) {
    abcg::glBindVertexArray(bullet.m_vao);

    abcg::glUniform2f(m_scaleLoc, m_scale, m_scale);
    abcg::glUniform1f(m_rotationLoc, bullet.m_rotation);

    abcg::glUniform2f(m_translationLoc, bullet.m_translation.x,
                      bullet.m_translation.y);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, m_sides + 2);
  }

  abcg::glBindVertexArray(0);

  abcg::glDisable(GL_BLEND);

  abcg::glUseProgram(0);
}

void Bullets::terminateGL() {
  for (auto bullet : m_bullets) {
    abcg::glDeleteBuffers(1, &bullet.m_vboVertices);
    abcg::glDeleteBuffers(1, &bullet.m_vboColors);
    abcg::glDeleteVertexArrays(1, &bullet.m_vao);
  }
}

void Bullets::update(Enemy &enemy, const GameData &gameData, float deltaTime) {
  if (gameData.m_state != State::Playing) return;

  // Fires a wave of bullets (4 bullets, each 90 degrees apart)
  if (m_bulletTimer.elapsed() > 0.1) {
    m_bulletTimer.restart();

    // When <= 50%, increases difficulty
    auto lanes{enemy.m_hp <= enemy.m_maxHP / 2 ? 8 : 4};
    auto angle{2 * M_PI / lanes};

    for (auto i : iter::range(0, lanes))
      m_bullets.push_back(createBullet(enemy, angle * i + m_rotation));
  }

  // Updates rotation
  if (abs(m_angularVelocity - m_targetAngularVelocity) >
      m_angularAccelerationStep) {
    m_angularVelocity += m_angularAcceleration * deltaTime;
  } else if (!m_targetReached) {
    m_targetReached = true;

    // Randomizes how much time angular velocity will be maintained
    std::uniform_real_distribution rd(0.5f, 2.0f);
    m_targetDuration = rd(m_re);
    m_targetTimer.restart();
  }
  m_rotation += m_angularVelocity * deltaTime;

  // Adds dynamism to the bullets rotation by calculating new target angular
  // velocity.
  if (m_targetReached && m_targetTimer.elapsed() > m_targetDuration) {
    std::uniform_int_distribution ri(-9, 9);
    m_targetAngularVelocity = M_PI / 2 * ri(m_re);
    m_angularAcceleration =
        m_angularAccelerationStep *
        (m_angularVelocity < m_targetAngularVelocity ? 1 : -1);
    m_targetReached = false;
  }

  for (auto &bullet : m_bullets) {
    bullet.m_translation += bullet.m_velocity * deltaTime;

    // Kill bullet if it goes off screen
    if (bullet.m_translation.x < -1.1f || bullet.m_translation.x > +1.1f ||
        bullet.m_translation.y < -1.1f || bullet.m_translation.y > +1.1f)
      bullet.m_dead = true;
  }

  // Remove dead bullets
  m_bullets.remove_if([](const Bullet &x) { return x.m_dead; });
}

Bullets::Bullet Bullets::createBullet(Enemy &enemy, float rotation) {
  auto velocity{glm::rotate(glm::vec2(1.0f, 0.0f), rotation)};
  auto translation{enemy.m_translation + velocity * enemy.m_scale};

  Bullet bullet{
      .m_rotation = rotation,
      .m_translation = translation,
      .m_velocity = velocity,
  };

  std::uniform_int_distribution ri(0, 6);
  auto colorIndex{ri(m_re)};

  // Create bullet visual
  std::vector<glm::vec2> positions(0);
  std::vector<glm::vec4> colors(0);

  positions.emplace_back(0, 0);
  colors.emplace_back(0, 0, 0, 0);

  const auto step{M_PI * 2 / m_sides};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
    colors.push_back(m_colors.at(colorIndex));
  }
  positions.push_back(positions.at(1));
  colors.push_back(colors.at(1));

  // Generate VBOs
  abcg::glGenBuffers(1, &bullet.m_vboVertices);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, bullet.m_vboVertices);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenBuffers(1, &bullet.m_vboColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, bullet.m_vboColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4),
                     colors.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  const GLint colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  // Create VAO
  abcg::glGenVertexArrays(1, &bullet.m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(bullet.m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, bullet.m_vboVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, bullet.m_vboColors);
  abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return bullet;
}