#include "waveparticle.hpp"

#include <chrono>
#include <cppitertools/range.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

void WaveParticlePattern::restart(Bullets* bullets) {
  m_bullets = bullets;
  m_bulletTimer.restart();
  m_re.seed(std::chrono::steady_clock::now().time_since_epoch().count());
  m_rotation = glm::mat4{1.0f};
  randomizeTargets();
}

void WaveParticlePattern::update(float deltaTime) {
  if (m_bulletTimer.elapsed() > 0.1) {
    m_bulletTimer.restart();

    auto velocity{glm::vec3(0.5f, 0.0f, 0.0f)};
    for (auto i : iter::range(m_numberRotations)) {
      auto axis{glm::vec3(1.0f * (i == 0), 1.0f * (i == 1), 1.0f * (i == 2))};
      auto angle{static_cast<float>(m_angularVelocities[i] * deltaTime)};
      m_rotation = glm::rotate(glm::mat4{1.0f}, angle, axis) * m_rotation;
    }

    for (auto i : iter::range(8)) {
      auto axis{glm::vec3(0.0f, 1.0f, 0.0f)};
      auto angle{static_cast<float>(M_PI_4 * i)};
      auto rotatedVelocity{glm::rotate(velocity, angle, axis)};
      createBullet(m_rotation * glm::vec4(rotatedVelocity, 1.0f));
    }

    velocity = glm::rotate(velocity, static_cast<float>(M_PI_4),
                           glm::vec3(0.0f, 0.0f, 1.0f));
    for (auto i : iter::range(8)) {
      auto axis{glm::vec3(0.0f, 1.0f, 0.0f)};
      auto angle{static_cast<float>(M_PI_4 * i)};
      auto rotatedVelocity{glm::rotate(velocity, angle, axis)};
      createBullet(m_rotation * glm::vec4(rotatedVelocity, 1.0f));
    }

    velocity = glm::rotate(velocity, static_cast<float>(-M_PI_2),
                           glm::vec3(0.0f, 0.0f, 1.0f));
    for (auto i : iter::range(8)) {
      auto axis{glm::vec3(0.0f, 1.0f, 0.0f)};
      auto angle{static_cast<float>(M_PI_4 * i)};
      auto rotatedVelocity{glm::rotate(velocity, angle, axis)};
      createBullet(m_rotation * glm::vec4(rotatedVelocity, 1.0f));
    }

    velocity = glm::rotate(velocity, static_cast<float>(-M_PI_4),
                           glm::vec3(0.0f, 0.0f, 1.0f));
    for (auto i : iter::range(2)) {
      auto axis{glm::vec3(1.0f, 0.0f, 0.0f)};
      auto angle{static_cast<float>(M_PI * i)};
      auto rotatedVelocity{glm::rotate(velocity, angle, axis)};
      createBullet(m_rotation * glm::vec4(rotatedVelocity, 1.0f));
    }
  }

  // Updates angular velocities
  std::vector<bool> targetStatuses;
  for (auto i : iter::range(m_numberRotations)) {
    targetStatuses.push_back(false);
    if (glm::abs(m_angularTargets[i] - m_angularVelocities[i]) >= M_PI_2 / 20) {
      auto signal{m_angularTargets[i] < m_angularVelocities[i] ? -1 : +1};
      m_angularVelocities[i] += m_angularAcceleration * deltaTime * signal;
    } else {
      targetStatuses[i] = true;
    }
  }

  auto targetReached{std::all_of(targetStatuses.begin(), targetStatuses.end(),
                                 [](bool v) { return v; })};
  if (targetReached) {
    randomizeTargets();
  }
}

void WaveParticlePattern::createBullet(glm::vec3 velocity) {
  std::uniform_int_distribution ri(0, static_cast<int>(m_colors.size() - 1));

  Bullets::Bullet bullet{
      .m_scale = 0.05f,
      .m_color = m_colors.at(ri(m_re)),
      .m_translation = glm::vec3(0.0f, 0.0f, 0.0f),
      .m_velocity = velocity,
      .m_forward = velocity,
  };
  m_bullets->m_bullets.push_back(bullet);
}

void WaveParticlePattern::randomizeTargets() {
  std::uniform_int_distribution ri(1, 3);
  std::uniform_real_distribution rd(-2 * M_PI, 2 * M_PI);
  m_numberRotations = ri(m_re);
  for (auto i : iter::range(m_numberRotations)) {
    m_angularTargets[i] = rd(m_re);
  }
}