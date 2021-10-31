#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "entities/bullets.hpp"
#include "entities/enemy.hpp"
#include "entities/healthbar.hpp"
#include "entities/ship.hpp"
#include "entities/shots.hpp"
#include "entities/stars.hpp"
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
  GLuint m_program;
  GLuint m_starsProgram;

  ImFont* m_font;
  ImFont* m_font_desc;

  int m_viewportHeight;
  int m_viewportWidth;

  GameData m_gameData;
  abcg::ElapsedTimer m_restartTimer;

  Stars m_stars;
  Ship m_ship;
  Shots m_shots;
  Enemy m_enemy;
  Bullets m_bullets;
  HealthBar m_healthBar;

  void restart();
  void update();
  void checkCollision();
  void checkFinished();
};

#endif