#ifndef STARS_HPP_
#define STARS_HPP_

#include <array>
#include <random>

#include "abcg.hpp"

class OpenGLWindow;

class Stars {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

 private:
  friend OpenGLWindow;

  GLuint m_program{};

  GLuint m_vao{};
  GLuint m_vboVertices{};
  GLuint m_vboColors{};
  GLuint m_vboSizes{};

  int m_quantity{70};
  glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};

  std::default_random_engine m_re;
};

#endif