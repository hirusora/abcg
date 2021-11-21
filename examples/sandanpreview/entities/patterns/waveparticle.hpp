#ifndef SIMPLEPATTERN_HPP_
#define SIMPLEPATTERN_HPP_

#include "../bullets.hpp"
#include "abcg.hpp"

class WaveParticlePattern {
 public:
  void restart(Bullets* bullets);
  void update(float deltaTime);

 private:
  Bullets* m_bullets;

  abcg::ElapsedTimer m_bulletTimer;

  void createBullet(glm::vec3 velocity);
};

#endif