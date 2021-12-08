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
  void paintGL(glm::mat4 viewMatrix);
  void terminateGL();

  void restart();
  void update(GameData gameData, float deltaTime);

  glm::vec3 getCoreLocation();
  float getScale() const { return m_scale; };
  bool isInvulnerable() { return m_invulnerableTimer.elapsed() < 1; };
  void takeHit();

  ShipCore m_core;

 private:
  float m_scale{0.1f};
  int m_deaths;
  bool m_focused{false};
  abcg::ElapsedTimer m_invulnerableTimer;

  glm::vec3 m_translation;
  glm::vec3 m_rotation;
};

#endif