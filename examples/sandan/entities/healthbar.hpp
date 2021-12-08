#ifndef HEALTHBAR_HPP_
#define HEALTHBAR_HPP_

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "ufo.hpp"

class HealthBar {
 public:
  void initializeGL(GLuint program);
  void terminateGL();
  void paintGL(GameData gameData);

  void update(GameData gameData, Ufo enemy);
  void restart();

 private:
  const float m_height{0.02f};

  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_VAO{};
  GLuint m_VBOVertices{};
  GLuint m_VBOColors{};

  glm::vec4 m_color{glm::vec4(64.0f, 252.0f, 30.0f, 1.0f) / 255.0f};
  glm::vec2 m_scale{};
  glm::vec2 m_translation{};
};
#endif