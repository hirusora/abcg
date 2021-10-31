#ifndef ENEMY_HPP_
#define ENEMY_HPP_

#include <random>

#include "../gamedata.hpp"
#include "abcg.hpp"

class OpenGLWindow;
class Bullets;
class HealthBar;

class Enemy {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;
  friend Bullets;
  friend HealthBar;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vboVertices{};
  GLuint m_vboColors{};
  GLuint m_ebo{};

  float m_scale{0.15f};
  float m_rotation{0.0f};
  glm::vec2 m_translation{};

  glm::vec4 m_mainColor{glm::vec4(217.0f, 30.0f, 24.0f, 255.0f) / 255.0f};
  glm::vec4 m_bodyColor{glm::vec4(255.0f, 255.0f, 255.0f, 255.0f) / 255.0f};
  glm::vec4 m_engineColor{glm::vec4(191.0f, 191.0f, 191.0f, 255.0f) / 255.0f};

  float m_maxHP{240.0f};
  float m_hp{};
  glm::vec2 m_velocity{};

  glm::vec2 m_targetTranslation{};
  int m_targetDuration{};
  bool m_targetReached{};

  std::default_random_engine m_re;

  abcg::ElapsedTimer m_targetTimer;
};
#endif