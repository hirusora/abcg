#ifndef SHOTS_HPP_
#define SHOTS_HPP_

#include <list>

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "ship.hpp"

class OpenGLWindow;

class Shots {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(Ship &ship, const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vboVertices{};
  GLuint m_vboColors{};

  struct Shot {
    bool m_dead{};
    int m_damage{};

    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
  };

  float m_scale{0.010f};
  int m_sides{10};

  std::list<Shot> m_shots;
};

#endif