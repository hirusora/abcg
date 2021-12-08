#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "gamedata.hpp"

class OpenGLWindow;

class Camera {
 public:
  void computeViewMatrix();
  void computeProjectionMatrix();
  void setViewport(float width, float height);

  void mouseWheel(float intensity);
  void mouseMove(glm::vec2 position);

  void restart();
  void update(GameData gameData, float deltaTime);

 private:
  friend OpenGLWindow;

  glm::vec3 m_baseEye{0.0f, 0.1f, -1.0f};

  glm::vec3 m_eye;
  glm::vec3 m_at;
  glm::vec3 m_up;

  glm::mat4 m_viewMatrix;
  glm::mat4 m_projMatrix;

  float m_viewportWidth;
  float m_viewportHeight;

  float m_FOV;

  glm::vec2 m_rotation;
};

#endif