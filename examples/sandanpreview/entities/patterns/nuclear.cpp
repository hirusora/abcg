#include "nuclear.hpp"

#include <chrono>
#include <cppitertools/range.hpp>
#include <glm/gtx/rotate_vector.hpp>

void NuclearPattern::restart(Bullets* bullets) {
  m_bullets = bullets;
  m_id = 0;
  m_bulletsExtension.clear();
  m_re.seed(std::chrono::steady_clock::now().time_since_epoch().count());
  m_bulletTimer.restart();
}

void NuclearPattern::update(float deltaTime) {
  // Every 1 second creates nuclear bullets at the ceiling.
  if (m_bulletTimer.elapsed() > 1.0f) {
    std::uniform_real_distribution rd(-1.0f, 1.0f);

    auto position{glm::vec3(rd(m_re), 1.0f, rd(m_re))};
    auto velocity{glm::vec3(0.0f, -0.2f, 0.0f)};

    createBullet(velocity, position, m_crimsonColor);
    m_bulletTimer.restart();
  }

  // Controls shrinking and explosion behavior.
  for (auto& bullet : m_bullets->m_bullets) {
    int id = bullet.m_id;
    if (!m_bulletsExtension[id].m_isNuclear) {
      continue;
    }

    if (!m_bulletsExtension[id].m_shrinking &&
        !m_bulletsExtension[id].m_exploding) {
      m_bulletsExtension[id].m_shrinking = true;
    }

    if (m_bulletsExtension[id].m_shrinking) {
      bullet.m_scale += m_shrinkScaling * deltaTime;
      if (bullet.m_scale <= 0.2f) {
        bullet.m_scale = 0.2f;
        m_bulletsExtension[id].m_shrinking = false;
        m_bulletsExtension[id].m_exploding = true;
      }
    }
    if (m_bulletsExtension[id].m_exploding) {
      bullet.m_scale += m_explosionScaling * deltaTime;
      if (bullet.m_scale >= 0.5f) {
        bullet.m_scale = 0.5f;
        m_bulletsExtension[id].m_shrinking = true;
        m_bulletsExtension[id].m_exploding = false;

        // When explosion finishes, shoots bullets around the explosion.
        auto position{bullet.m_translation};
        auto velocity{glm::vec3(0.5f, 0.0f, 0.0f)};
        auto offset{glm::vec3(0.3f, 0.0f, 0.0f)};

        for (auto i : iter::range(0, 8)) {
          auto axis{glm::vec3(0.0f, 1.0f, 0.0f)};
          auto angle{static_cast<float>(M_PI_4 * i)};
          auto rotatedVelocity{glm::rotate(velocity, angle, axis)};
          auto rotatedOffset{glm::rotate(offset, angle, axis)};
          createBullet(rotatedVelocity, position + rotatedOffset, m_yellowColor,
                       0.05f, false);
        }

        velocity = glm::rotate(velocity, static_cast<float>(M_PI_4),
                               glm::vec3(0.0f, 0.0f, 1.0f));
        offset = glm::rotate(offset, static_cast<float>(M_PI_4),
                             glm::vec3(0.0f, 0.0f, 1.0f));
        for (auto i : iter::range(0, 8)) {
          auto axis{glm::vec3(0.0f, 1.0f, 0.0f)};
          auto angle{static_cast<float>(M_PI_4 * i)};
          auto rotatedVelocity{glm::rotate(velocity, angle, axis)};
          auto rotatedOffset{glm::rotate(offset, angle, axis)};
          createBullet(rotatedVelocity, position + rotatedOffset, m_yellowColor,
                       0.05f, false);
        }

        velocity = glm::rotate(velocity, static_cast<float>(-M_PI_2),
                               glm::vec3(0.0f, 0.0f, 1.0f));
        offset = glm::rotate(offset, static_cast<float>(-M_PI_2),
                             glm::vec3(0.0f, 0.0f, 1.0f));
        for (auto i : iter::range(0, 8)) {
          auto axis{glm::vec3(0.0f, 1.0f, 0.0f)};
          auto angle{static_cast<float>(M_PI_4 * i)};
          auto rotatedVelocity{glm::rotate(velocity, angle, axis)};
          auto rotatedOffset{glm::rotate(offset, angle, axis)};
          createBullet(rotatedVelocity, position + rotatedOffset, m_yellowColor,
                       0.05f, false);
        }

        velocity = glm::rotate(velocity, static_cast<float>(-M_PI_4),
                               glm::vec3(0.0f, 0.0f, 1.0f));
        offset = glm::rotate(offset, static_cast<float>(-M_PI_4),
                             glm::vec3(0.0f, 0.0f, 1.0f));
        for (auto i : iter::range(0, 2)) {
          auto axis{glm::vec3(1.0f, 0.0f, 0.0f)};
          auto angle{static_cast<float>(M_PI * i)};
          auto rotatedVelocity{glm::rotate(velocity, angle, axis)};
          auto rotatedOffset{glm::rotate(offset, angle, axis)};
          createBullet(rotatedVelocity, position + rotatedOffset, m_yellowColor,
                       0.05f, false);
        }
      }
    }
  }
}

void NuclearPattern::createBullet(glm::vec3 velocity, glm::vec3 start,
                                  glm::vec4 color, float scale,
                                  bool isNuclear) {
  Bullets::Bullet bullet{
      .m_id = m_id++,
      .m_scale = scale,
      .m_color = color,
      .m_translation = start,
      .m_velocity = velocity,
      .m_forward = velocity,
  };
  m_bullets->m_bullets.push_back(bullet);

  // Updates bullet extension
  m_bulletsExtension[bullet.m_id] = BulletExtension{.m_isNuclear = isNuclear};
  m_bulletsExtension[bullet.m_id].m_timer.restart();
}