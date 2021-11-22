#ifndef WAVEPARTICLEPATTERN_HPP_
#define WAVEPARTICLEPATTERN_HPP_

#include <random>

#include "../bullets.hpp"
#include "abcg.hpp"

class WaveParticlePattern {
 public:
  void restart(Bullets* bullets);
  void update(float deltaTime);

 private:
  Bullets* m_bullets;

  std::default_random_engine m_re;
  abcg::ElapsedTimer m_bulletTimer;

  glm::mat4 m_rotation{1.0f};
  int m_numberRotations{0};
  std::vector<float> m_angularTargets{0.0f, 0.0f, 0.0f};

  std::vector<float> m_angularVelocities{0.0f, 0.0f, 0.0f};
  float m_angularAcceleration{M_PI_4};

  std::vector<glm::vec4> m_colors{
      glm::vec4(217.0f, 30.0f, 24.0f, 255.0f) / 255.0f,   // Red
      glm::vec4(0.0f, 230.0f, 64.0f, 255.0f) / 255.0f,    // Green
      glm::vec4(0.0f, 181.0f, 204.0f, 255.0f) / 255.0f,   // Blue
      glm::vec4(248.0f, 148.0f, 6.0f, 255.0f) / 255.0f,   // Orange
      glm::vec4(238.0f, 238.0f, 0.0f, 255.0f) / 255.0f,   // Yellow
      glm::vec4(140.0f, 20.0f, 252.0f, 255.0f) / 255.0f,  // Indigo
      glm::vec4(150.0f, 54.0f, 148.0f, 255.0f) / 255.0f,  // Violet
  };

  void createBullet(glm::vec3 velocity);
  void randomizeTargets();
};

#endif