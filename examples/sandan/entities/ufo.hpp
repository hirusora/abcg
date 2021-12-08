#ifndef UFO_HPP_
#define UFO_HPP_

#include <vector>

#include "../gamedata.hpp"
#include "abcg.hpp"
#include "model.hpp"

class Ufo : Model {
 public:
  void initializeGL(GLuint program, std::string path);
  void paintGL(GameData gameData, glm::mat4 viewMatrix);
  void terminateGL();

  void restart();

  void takeDamage(int damage) { m_hp -= damage; };

  float getScale() const { return m_scale; };
  int getHP() { return m_hp; };
  int getBaseHP() { return m_baseHP; };
  glm::vec3 getTranslation() { return m_translation; };

 private:
  float m_scale{0.1f};
  int m_baseHP{120};
  int m_hp;
  glm::vec3 m_translation;
};

#endif