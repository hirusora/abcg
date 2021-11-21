#ifndef BULLETS_HPP_
#define BULLETS_HPP_

#include <list>

#include "abcg.hpp"
#include "model.hpp"

class Bullets : Model {
 public:
  void initializeGL(GLuint program, std::string assetsPath);
  void paintGL();
  void terminateGL();

  void create(glm::vec3 start, glm::vec3 velocity, glm::vec3 forward);
  void update(float deltaTime);

 private:
  struct Bullet {
    float m_scale{0.1f};
    glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};

    glm::vec3 m_translation{0.0f, 0.0f, 0.0f};
    glm::vec3 m_velocity{0.0f, 0.0f, 0.0f};
    glm::vec3 m_forward{1.0f, 0.0f, 0.0f};
  };

  GLuint m_program;
  GLint m_modelMatrixLoc;
  GLint m_colorLoc;

  std::list<Bullet> m_bullets;
};

#endif