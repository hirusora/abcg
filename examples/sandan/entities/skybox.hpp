#ifndef SKYBOX_HPP_
#define SKYBOX_HPP_

#include <vector>

#include "abcg.hpp"

class Skybox {
 public:
  void initializeGL(GLuint program, std::string path);
  void terminateGL();
  void paintGL(glm::mat4 modelMatrix, glm::mat4 viewMatrix) const;

  void loadTexture(std::string path);

 private:
  const std::array<glm::vec3, 36> m_positions{
      // Front
      glm::vec3{-1, -1, +1}, glm::vec3{+1, -1, +1}, glm::vec3{+1, +1, +1},
      glm::vec3{-1, -1, +1}, glm::vec3{+1, +1, +1}, glm::vec3{-1, +1, +1},
      // Back
      glm::vec3{+1, -1, -1}, glm::vec3{-1, -1, -1}, glm::vec3{-1, +1, -1},
      glm::vec3{+1, -1, -1}, glm::vec3{-1, +1, -1}, glm::vec3{+1, +1, -1},
      // Right
      glm::vec3{+1, -1, -1}, glm::vec3{+1, +1, -1}, glm::vec3{+1, +1, +1},
      glm::vec3{+1, -1, -1}, glm::vec3{+1, +1, +1}, glm::vec3{+1, -1, +1},
      // Left
      glm::vec3{-1, -1, +1}, glm::vec3{-1, +1, +1}, glm::vec3{-1, +1, -1},
      glm::vec3{-1, -1, +1}, glm::vec3{-1, +1, -1}, glm::vec3{-1, -1, -1},
      // Top
      glm::vec3{-1, +1, +1}, glm::vec3{+1, +1, +1}, glm::vec3{+1, +1, -1},
      glm::vec3{-1, +1, +1}, glm::vec3{+1, +1, -1}, glm::vec3{-1, +1, -1},
      // Bottom
      glm::vec3{-1, -1, -1}, glm::vec3{+1, -1, -1}, glm::vec3{+1, -1, +1},
      glm::vec3{-1, -1, -1}, glm::vec3{+1, -1, +1}, glm::vec3{-1, -1, +1}};

  GLuint m_VAO{};
  GLuint m_VBO{};

  GLint m_viewMatrixLoc{};
  GLint m_projMatrixLoc{};
  GLint m_skyTexLoc{};

  GLuint m_texture{};
};

#endif