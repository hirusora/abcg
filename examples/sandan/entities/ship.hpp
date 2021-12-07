#ifndef SHIP_HPP_
#define SHIP_HPP_

#include <vector>

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "model.hpp"

class Ship : Model {
 public:
  void initializeGL(GLuint program, std::string assetPath);
  void paintGL(glm::mat4 viewMatrix);
  void terminateGL();

  void restart();
  void update(GameData gameData, float deltaTime);

 private:
  GLuint m_program;
  GLint m_modelMatrixLoc;
  GLint m_normalMatrixLoc;
  GLint m_shininessLoc;
  GLint m_KaLoc;
  GLint m_KdLoc;
  GLint m_KsLoc;

  float m_scale{0.1f};
  glm::vec3 m_translation;
  glm::vec3 m_rotation;
};

#endif