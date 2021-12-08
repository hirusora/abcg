#ifndef SHOTS_HPP_
#define SHOTS_HPP_

#include <vector>

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "model.hpp"

class Shots : Model {
 public:
  void initializeGL(GLuint program, std::string path);
  void paintGL(GameData gameData, glm::mat4 viewMatrix);
  void terminateGL();

  void restart();
  void update(float deltaTime);
  void createBullet(glm::vec3 location);

  struct Shot {
    bool m_dead{false};
    float m_scale{0.01f};
    int m_damage{1};

    glm::vec3 m_translation;
    glm::vec3 m_velocity;
  };

  std::vector<Shot> m_shots;

 private:
  GLuint m_texture;
  abcg::ElapsedTimer m_timer;
};

#endif