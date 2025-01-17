#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "camera.hpp"
#include "entities/bullets.hpp"
#include "entities/healthbar.hpp"
#include "entities/patterns/waveparticle.hpp"
#include "entities/ship.hpp"
#include "entities/shipcore.hpp"
#include "entities/shots.hpp"
#include "entities/skybox.hpp"
#include "entities/ufo.hpp"
#include "gamedata.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_program{};
  GLuint m_2dProgram{};
  GLuint m_skyboxProgram{};

  GLint m_viewMatrixLoc{};
  GLint m_projMatrixLoc{};
  GLint m_diffuseTexLoc{};
  GLint m_normalTexLoc{};
  GLint m_cubeTexLoc{};
  GLint m_lightDirLoc{};
  GLint m_IaLoc{};
  GLint m_IdLoc{};
  GLint m_IsLoc{};

  ImFont* m_font;
  ImFont* m_font_desc;

  int m_viewportWidth{};
  int m_viewportHeight{};

  glm::vec4 m_lightDir{0.0f, 0.0f, 1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};

  GameData m_gameData;
  Camera m_camera;
  Ship m_ship;
  Ufo m_ufo;
  Bullets m_bullets;
  Shots m_shots;
  WaveParticlePattern m_waveParticlePattern;
  Skybox m_skybox;
  HealthBar m_healthBar;

  bool m_restarting{false};

  void restart();
  void update();
  void checkCollision();
  void checkFinish();
};

#endif