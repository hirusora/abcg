#ifndef BULLETS_HPP_
#define BULLETS_HPP_

#include <vector>

#include "abcg.hpp"
#include "model.hpp"

class Bullets : Model {
 public:
  void initializeGL(GLuint program, std::string path);
  void paintGL(glm::mat4 viewMatrix);
  void terminateGL();

  void restart();
  void update(float deltaTime);

  struct Bullet {
    int m_id{};

    bool m_dead{false};
    float m_scale{0.1f};
    GLuint m_texture{};

    glm::vec3 m_translation{0.0f, 0.0f, 0.0f};
    glm::vec3 m_velocity{0.0f, 0.0f, 0.0f};
    glm::vec3 m_forward{1.0f, 0.0f, 0.0f};
  };

  std::vector<Bullet> m_bullets;
};

#endif