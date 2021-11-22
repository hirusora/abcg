#ifndef BULLETS_HPP_
#define BULLETS_HPP_

#include <vector>

#include "abcg.hpp"
#include "model.hpp"

class Bullets : Model {
 public:
  void initializeGL(GLuint program, std::string assetPath);
  void paintGL();
  void terminateGL();

  void restart();
  void update(float deltaTime);

  struct Bullet {
    int m_id{};

    bool m_dead{false};
    float m_scale{0.1f};
    glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};

    glm::vec3 m_translation{0.0f, 0.0f, 0.0f};
    glm::vec3 m_velocity{0.0f, 0.0f, 0.0f};
    glm::vec3 m_forward{1.0f, 0.0f, 0.0f};
  };

  std::vector<Bullet> m_bullets;

 private:
  GLuint m_program;
  GLint m_modelMatrixLoc;
  GLint m_colorLoc;
};

#endif