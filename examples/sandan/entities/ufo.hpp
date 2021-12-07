#ifndef UFO_HPP_
#define UFO_HPP_

#include <vector>

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "model.hpp"

class Ufo : Model {
 public:
  void initializeGL(GLuint program, std::string assetPath);
  void paintGL(glm::mat4 viewMatrix);
  void terminateGL();

  void restart();

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
};

#endif