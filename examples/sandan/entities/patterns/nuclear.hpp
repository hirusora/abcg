#ifndef NUCLEARPATTERN_HPP_
#define NUCLEARPATTERN_HPP_

#include <random>
#include <unordered_map>

#include "../bullets.hpp"
#include "abcg.hpp"

class NuclearPattern {
 public:
  void restart(Bullets* bullets);
  void update(float deltaTime);

 private:
  Bullets* m_bullets;
  int m_id{0};

  std::default_random_engine m_re;
  abcg::ElapsedTimer m_bulletTimer{};

  struct BulletExtension {
    abcg::ElapsedTimer m_timer{};
    bool m_isNuclear{true};
    bool m_shrinking{false};
    bool m_exploding{false};
  };

  std::unordered_map<int, BulletExtension> m_bulletsExtension;
  float m_shrinkScaling{-0.1f};
  float m_explosionScaling{+0.20f};

  glm::vec4 m_crimsonColor{glm::vec4(220.0f, 20.0f, 60.0f, 255.0f) / 255.0f};
  glm::vec4 m_yellowColor{glm::vec4(244.0f, 208.0f, 63.0f, 255.0f) / 255.0f};

  void createBullet(glm::vec3 velocity, glm::vec3 start, glm::vec4 color,
                    float scale = 0.5f, bool isNuclear = true);
};

#endif