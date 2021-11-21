#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class OpenGLWindow;

class Camera {
 public:
  void computeViewMatrix();
  void computeProjectionMatrix(int width, int height);
  void update(float deltaTime);

 private:
  friend OpenGLWindow;

  glm::vec3 m_eye{glm::vec3(0.0f, 1.5f, 5.0f)};
  glm::vec3 m_at{glm::vec3(0.0f, 0.0f, 0.0f)};
  glm::vec3 m_up{glm::vec3(0.0f, 1.0f, 0.0f)};

  glm::mat4 m_viewMatrix;
  glm::mat4 m_projMatrix;

  float m_FOV{70.0f};
  float m_angle{M_PI / 20};
};

#endif