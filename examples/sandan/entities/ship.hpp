#ifndef SHIP_HPP_
#define SHIP_HPP_

#include <vector>

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "model.hpp"
#include "shipcore.hpp"

class Ship : Model {
 public:
  void initializeGL(GLuint program, std::string path);
  void paintGL(GameData gameData, glm::mat4 viewMatrix);
  void terminateGL();

  void restart();
  void update(GameData gameData, float deltaTime);

  glm::vec3 getCoreLocation();
  float getScale() const { return m_scale; };
  bool isInvulnerable() { return m_state == ShipState::Invulnerable; };
  void takeHit();
  int getDeaths() { return m_deaths; };

  ShipCore m_core;

 private:
  enum class ShipState { Normal, Invulnerable };

  float m_scale{0.1f};
  int m_deaths;
  bool m_focused{false};
  ShipState m_state;

  abcg::ElapsedTimer m_invulnerableTimer;
  abcg::ElapsedTimer m_invulnerableBlinkTimer;

  glm::vec3 m_translation;
  glm::vec3 m_rotation;
};

#endif