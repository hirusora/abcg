#ifndef SHIPCORE_HPP_
#define SHIPCORE_HPP_

#include <vector>

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "model.hpp"

class ShipCore : Model {
 public:
  void initializeGL(GLuint program, std::string path);
  void paintGL(glm::mat4 viewMatrix, glm::vec3 translation);
  void terminateGL();

  float getScale() const { return m_scale; };

 private:
  float m_scale{0.01f};
  GLuint m_texture;
};

#endif