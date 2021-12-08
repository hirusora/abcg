#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void OpenGLWindow::handleEvent(SDL_Event& event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.set(static_cast<size_t>(Input::Forward));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.set(static_cast<size_t>(Input::Backward));
    if (event.key.keysym.sym == SDLK_LEFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::ZoomOut));
    if (event.key.keysym.sym == SDLK_RIGHT)
      m_gameData.m_input.set(static_cast<size_t>(Input::ZoomIn));
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Shot));
    if (event.key.keysym.sym == SDLK_LSHIFT ||
        event.key.keysym.sym == SDLK_RSHIFT ||
        event.key.keysym.sym == SDLK_LCTRL ||
        event.key.keysym.sym == SDLK_RCTRL)
      m_gameData.m_input.set(static_cast<size_t>(Input::Focus));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Forward));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Backward));
    if (event.key.keysym.sym == SDLK_LEFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::ZoomOut));
    if (event.key.keysym.sym == SDLK_RIGHT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::ZoomIn));
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Shot));
    if (event.key.keysym.sym == SDLK_LSHIFT ||
        event.key.keysym.sym == SDLK_RSHIFT ||
        event.key.keysym.sym == SDLK_LCTRL ||
        event.key.keysym.sym == SDLK_RCTRL)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Focus));
  }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "shaders/model.vert",
                                    getAssetsPath() + "shaders/model.frag");

  // Get location of uniform variables
  m_viewMatrixLoc = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLoc = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_diffuseTexLoc = abcg::glGetUniformLocation(m_program, "diffuseTex");
  m_normalTexLoc = abcg::glGetUniformLocation(m_program, "normalTex");
  m_cubeTexLoc = abcg::glGetUniformLocation(m_program, "cubeTex");
  m_lightDirLoc = abcg::glGetUniformLocation(m_program, "lightDirWorldSpace");
  m_IaLoc = abcg::glGetUniformLocation(m_program, "Ia");
  m_IdLoc = abcg::glGetUniformLocation(m_program, "Id");
  m_IsLoc = abcg::glGetUniformLocation(m_program, "Is");

  // Load models
  m_bullets.initializeGL(m_program,
                         getAssetsPath() + "models/bullet/bullet.obj");
  m_ship.initializeGL(m_program,
                      getAssetsPath() + "models/spaceship/spaceship.obj");
  m_ship.m_core.initializeGL(m_program,
                             getAssetsPath() + "models/shipcore/shipcore.obj");
  m_ufo.initializeGL(m_program, getAssetsPath() + "models/ufo/ufo.obj");
  m_waveParticlePattern.initializeGL(getAssetsPath() +
                                     "models/bullet/textures/waveparticle");

  restart();

  resizeGL(getWindowSettings().width, getWindowSettings().height);
  m_camera.computeViewMatrix();
}

void OpenGLWindow::paintGL() {
  if (m_restarting) return;

  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);
  abcg::glUniform1i(m_diffuseTexLoc, 0);
  abcg::glUniform1i(m_normalTexLoc, 1);
  abcg::glUniform1i(m_cubeTexLoc, 2);
  abcg::glUniform4fv(m_lightDirLoc, 1, &m_lightDir.x);
  abcg::glUniform4fv(m_IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(m_IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(m_IsLoc, 1, &m_Is.x);

  m_ufo.paintGL(m_camera.m_viewMatrix);
  m_bullets.paintGL(m_camera.m_viewMatrix);

  if (m_gameData.m_input[static_cast<size_t>(Input::Focus)]) {
    m_ship.m_core.paintGL(m_camera.m_viewMatrix, m_ship.getCoreLocation());
  }
  m_ship.paintGL(m_camera.m_viewMatrix);

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() { abcg::OpenGLWindow::paintUI(); }

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
  m_camera.setViewport(width, height);
}

void OpenGLWindow::terminateGL() {
  m_bullets.terminateGL();
  m_ship.terminateGL();
  m_ship.m_core.terminateGL();
  m_ufo.terminateGL();
  m_waveParticlePattern.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::restart() {
  m_restarting = true;

  m_camera.restart();
  m_ship.restart();
  m_ufo.restart();
  m_bullets.restart();
  m_waveParticlePattern.restart(&m_bullets);

  m_restarting = false;
}

void OpenGLWindow::update() {
  if (m_restarting) return;

  float deltaTime{static_cast<float>(getDeltaTime())};
  m_ship.update(m_gameData, deltaTime);
  m_camera.update(m_gameData, deltaTime);

  m_bullets.update(deltaTime);
  m_waveParticlePattern.update(deltaTime);

  checkCollision();
}

void OpenGLWindow::checkCollision() {
  // Check enemy collision
  // for (auto& shot : m_shots.m_shots) {
  //   auto distance{glm::distance(m_enemy.m_translation, shot.m_translation)};
  //   if (distance < m_enemy.m_scale + m_shots.m_scale) {
  //     m_enemy.m_hp -= shot.m_damage;
  //     shot.m_dead = true;
  //   }
  // }

  // Check ship's core collision
  if (!m_ship.isInvulnerable()) {
    auto core{m_ship.getCoreLocation()};
    for (auto bullet : m_bullets.m_bullets) {
      auto distance{glm::distance(core, bullet.m_translation)};
      if (distance < m_ship.m_core.getScale() * 0.9f + bullet.m_scale * 0.9f) {
        m_ship.takeHit();
      }
    }
  }
}