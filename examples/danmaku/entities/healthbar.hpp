#ifndef HEALTHBAR_HPP_
#define HEALTHBAR_HPP_

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "enemy.hpp"

class HealthBar {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(Enemy &enemy, const GameData &gameData);

 private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vboVertices{};
  GLuint m_vboColors{};

  glm::vec4 m_color{glm::vec4(64.0f, 252.0f, 30.0f, 1.0f) / 255.0f};
  glm::vec2 m_scale{};
  glm::vec2 m_translation{};
  float m_rotation{0.0f};
};
#endif