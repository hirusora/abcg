#include "waveparticle.hpp"

#include <cppitertools/range.hpp>
#include <glm/gtx/rotate_vector.hpp>

void WaveParticlePattern::restart(Bullets* bullets) {
  m_bullets = bullets;
  m_bulletTimer.restart();
}

void WaveParticlePattern::update(float deltaTime) {
  if (m_bulletTimer.elapsed() > 0.1) {
    m_bulletTimer.restart();

    auto forward{glm::vec3(1.0f, 0.0f, 0.0f)};

    for (auto angle : iter::range(0.0, 2 * M_PI, M_PI_2)) {
      auto velocity{glm::rotate(forward, static_cast<float>(angle),
                                glm::vec3(0.0f, 1.0f, 0.0f))};
      createBullet(velocity);
    }
    for (auto angle : iter::range(0.0, M_PI, M_PI)) {
      auto velocity{glm::rotate(forward, static_cast<float>(angle + M_PI_2),
                                glm::vec3(1.0f, 0.0f, 0.0f))};
      createBullet(velocity);
    }
  }
}

void WaveParticlePattern::createBullet(glm::vec3 velocity) {
  Bullets::Bullet bullet{
      .m_scale = 0.05f,
      .m_translation = glm::vec3(0.0f, 0.0f, 0.0f),
      .m_velocity = velocity,
      .m_forward = velocity,
  };
  m_bullets->m_bullets.push_back(bullet);
}