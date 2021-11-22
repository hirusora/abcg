#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <random>

#include "abcg.hpp"
#include "camera.hpp"
#include "entities/bullets.hpp"
#include "entities/patterns/nuclear.hpp"
#include "entities/patterns/waveparticle.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_program{};
  GLint m_viewMatrixLoc{};
  GLint m_projMatrixLoc{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  Bullets m_bullets;
  WaveParticlePattern m_waveParticlePattern{};
  NuclearPattern m_nuclearPattern{};

  unsigned long m_patternIndex{0};
  bool m_restarting{false};

  void restart();
  void update();
};

#endif