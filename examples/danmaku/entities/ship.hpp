#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "../gamedata.hpp"
#include "abcg.hpp"

enum class ShipState { Normal, Invulnerable };

class OpenGLWindow;
class Shots;

class Ship {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);

  void takeHit();

 private:
  friend OpenGLWindow;
  friend Shots;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vboVertices{};
  GLuint m_vboColors{};
  GLuint m_ebo{};

  struct Core {
    GLuint m_vao{};
    GLuint m_vboVertices{};
    GLuint m_vboColors{};

    glm::vec4 m_color{1, 0, 0, 1};
    float m_scale{0.02f};
    int m_sides{30};
  };

  Core m_core{};

  float m_scale{0.125f};
  glm::vec2 m_translation{};
  float m_rotation{0.0f};
  float m_velocity_speed{1.3f};
  int m_deaths{};
  ShipState m_state{};

  glm::vec4 m_mainColor{glm::vec4(44.0f, 130.0f, 201.0f, 255.0f) / 255.0f};
  glm::vec4 m_bodyColor{glm::vec4(255.0f, 255.0f, 255.0f, 255.0f) / 255.0f};
  glm::vec4 m_engineColor{glm::vec4(191.0f, 191.0f, 191.0f, 255.0f) / 255.0f};
  glm::vec4 m_innerThrusterColor{glm::vec4(245.0f, 229.0f, 27.0f, 255.0f) /
                                 255.0f};
  glm::vec4 m_outerThrusterColor{glm::vec4(242.0f, 38.0f, 19.0f, 255.0f) /
                                 255.0f};
  glm::vec4 m_coreColor{glm::vec4(207.0f, 0.0f, 15.0f, 255.0f) / 255.0f};

  int m_invulnerabilitySeconds{1};
  abcg::ElapsedTimer m_invulnerableTimer;
  abcg::ElapsedTimer m_invulnerableBlinkTimer;

  abcg::ElapsedTimer m_trailBlinkTimer;
  abcg::ElapsedTimer m_shotCoolDownTimer;
};
#endif