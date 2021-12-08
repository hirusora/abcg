#ifndef UFO_HPP_
#define UFO_HPP_

#include <vector>

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "model.hpp"

class Ufo : Model {
 public:
  void initializeGL(GLuint program, std::string path);
  void paintGL(glm::mat4 viewMatrix);
  void terminateGL();

  void restart();

 private:
  glm::vec3 m_translation;
};

#endif