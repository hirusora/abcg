#ifndef BULLETS_HPP_
#define BULLETS_HPP_

#include <list>
#include <random>
#include <vector>

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "enemy.hpp"

class OpenGLWindow;

class Bullets {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(Enemy &enemy, const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  struct Bullet {
    GLuint m_vao{};
    GLuint m_vboVertices{};
    GLuint m_vboColors{};

    bool m_dead{};
    float m_rotation{};
    glm::vec2 m_translation{};
    glm::vec2 m_velocity{};
  };

  int m_sides{30};
  float m_scale{0.05f};
  std::vector<glm::vec4> m_colors{
      glm::vec4(217.0f, 30.0f, 24.0f, 255.0f) / 255.0f,   // Red
      glm::vec4(0.0f, 230.0f, 64.0f, 255.0f) / 255.0f,    // Green
      glm::vec4(0.0f, 181.0f, 204.0f, 255.0f) / 255.0f,   // Blue
      glm::vec4(248.0f, 148.0f, 6.0f, 255.0f) / 255.0f,   // Orange
      glm::vec4(238.0f, 238.0f, 0.0f, 255.0f) / 255.0f,   // Yellow
      glm::vec4(140.0f, 20.0f, 252.0f, 255.0f) / 255.0f,  // Indigo
      glm::vec4(150.0f, 54.0f, 148.0f, 255.0f) / 255.0f,  // Violet
  };

  float m_rotation;
  float m_angularVelocity;
  float m_angularAcceleration;
  float m_angularAccelerationStep{M_PI / 2 * 0.1f};
  float m_targetAngularVelocity;
  int m_targetDuration;
  bool m_targetReached;

  std::list<Bullet> m_bullets;

  std::default_random_engine m_re;

  abcg::ElapsedTimer m_bulletTimer;
  abcg::ElapsedTimer m_targetTimer;

  Bullet createBullet(Enemy &enemy, float rotation);
};

#endif