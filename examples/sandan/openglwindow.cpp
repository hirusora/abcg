#include "openglwindow.hpp"

#include <fmt/core.h>
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
      m_gameData.m_input.set(static_cast<size_t>(Input::Shoot));
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
      m_gameData.m_input.reset(static_cast<size_t>(Input::Shoot));
    if (event.key.keysym.sym == SDLK_LSHIFT ||
        event.key.keysym.sym == SDLK_RSHIFT ||
        event.key.keysym.sym == SDLK_LCTRL ||
        event.key.keysym.sym == SDLK_RCTRL)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Focus));
  }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0, 0, 1);

  // Load a new font
  ImGuiIO& io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "fonts/KawashiroGothic-Lla5.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  m_font_desc = io.Fonts->AddFontFromFileTTF(filename.c_str(), 30.0f);
  if (m_font == nullptr || m_font_desc == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "shaders/model.vert",
                                    getAssetsPath() + "shaders/model.frag");
  m_2dProgram = createProgramFromFile(getAssetsPath() + "shaders/model2d.vert",
                                      getAssetsPath() + "shaders/model2d.frag");
  m_skyboxProgram =
      createProgramFromFile(getAssetsPath() + "shaders/skybox.vert",
                            getAssetsPath() + "shaders/skybox.frag");

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
  m_shots.initializeGL(m_program, getAssetsPath() + "models/shot/shot.obj");
  m_ufo.initializeGL(m_program, getAssetsPath() + "models/ufo/ufo.obj");
  m_waveParticlePattern.initializeGL(getAssetsPath() +
                                     "models/bullet/textures/waveparticle/");
  m_skybox.initializeGL(m_skyboxProgram,
                        getAssetsPath() + "models/skybox/textures/");
  m_healthBar.initializeGL(m_2dProgram);

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

  m_ufo.paintGL(m_gameData, m_camera.m_viewMatrix);
  m_bullets.paintGL(m_gameData, m_camera.m_viewMatrix);
  m_shots.paintGL(m_gameData, m_camera.m_viewMatrix);

  if (m_gameData.m_input[static_cast<size_t>(Input::Focus)]) {
    m_ship.m_core.paintGL(m_gameData, m_camera.m_viewMatrix,
                          m_ship.getCoreLocation());
  }
  m_ship.paintGL(m_gameData, m_camera.m_viewMatrix);

  abcg::glUseProgram(0);

  // Render skybox
  abcg::glUseProgram(m_skyboxProgram);
  m_skybox.paintGL(m_camera.m_projMatrix, m_camera.m_viewMatrix);
  abcg::glUseProgram(0);

  // Render 2D models
  abcg::glUseProgram(m_2dProgram);
  m_healthBar.paintGL(m_gameData);
  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  if (m_gameData.m_state == State::Finished) {
    const auto size{ImVec2(m_viewportWidth, m_viewportHeight)};
    const auto position{ImVec2(0, 0)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);

    // Calculate each text size for centering.
    ImGui::PushFont(m_font);
    const auto message = "Enemy defeated!";
    const auto messageSize = ImGui::CalcTextSize(message);
    ImGui::PopFont();

    ImGui::PushFont(m_font_desc);
    const auto deathsMessage{fmt::format("Deaths: {}", m_ship.getDeaths())};
    const auto deathsMessageSize = ImGui::CalcTextSize(deathsMessage.c_str());

    const auto hours{(int)m_gameData.m_finishTime / 3600};
    const auto minutes{((int)m_gameData.m_finishTime % 3600) / 60};
    const auto seconds{(int)m_gameData.m_finishTime % 60};
    const auto timeMessage{
        fmt::format("Total time: {:02}:{:02}:{:02}", hours, minutes, seconds)};
    const auto timeMessageSize = ImGui::CalcTextSize(timeMessage.c_str());
    ImGui::PopFont();

    const auto totalTextHeight{messageSize.y + deathsMessageSize.y +
                               timeMessageSize.y};

    // Print game finished message with centering
    ImGui::PushFont(m_font);
    ImGui::SetCursorPos(ImVec2((m_viewportWidth - messageSize.x) / 2,
                               (m_viewportHeight - totalTextHeight) / 2));
    ImGui::Text(message);
    ImGui::PopFont();

    ImGui::PushFont(m_font_desc);
    ImGui::SetCursorPos(
        ImVec2((m_viewportWidth - deathsMessageSize.x) / 2,
               (m_viewportHeight - totalTextHeight) / 2 + messageSize.y));
    ImGui::Text("%s", deathsMessage.c_str());
    ImGui::PopFont();

    ImGui::PushFont(m_font_desc);
    ImGui::SetCursorPos(ImVec2((m_viewportWidth - timeMessageSize.x) / 2,
                               (m_viewportHeight - totalTextHeight) / 2 +
                                   messageSize.y + deathsMessageSize.y));
    ImGui::Text("%s", timeMessage.c_str());
    ImGui::PopFont();

    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
  m_camera.setViewport(width, height);
}

void OpenGLWindow::terminateGL() {
  m_bullets.terminateGL();
  m_shots.terminateGL();
  m_ship.terminateGL();
  m_ship.m_core.terminateGL();
  m_ufo.terminateGL();
  m_waveParticlePattern.terminateGL();
  m_skybox.terminateGL();
  m_healthBar.terminateGL();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteProgram(m_2dProgram);
  abcg::glDeleteProgram(m_skyboxProgram);
}

void OpenGLWindow::restart() {
  m_restarting = true;

  m_gameData.m_state = State::Playing;
  m_gameData.m_finishTime = 0;
  m_gameData.m_gameTimer.restart();

  m_camera.restart();
  m_ship.restart();
  m_shots.restart();
  m_ufo.restart();
  m_bullets.restart();
  m_waveParticlePattern.restart(&m_bullets);
  m_healthBar.restart();

  m_restarting = false;
}

void OpenGLWindow::update() {
  if (m_restarting) return;

  if (m_gameData.m_state == State::Finished &&
      m_gameData.m_restartTimer.elapsed() >= 5) {
    restart();
  }

  float deltaTime{static_cast<float>(getDeltaTime())};

  if (m_gameData.m_input[static_cast<size_t>(Input::Shoot)]) {
    m_shots.createBullet(m_ship.getCoreLocation());
  }

  m_ship.update(m_gameData, deltaTime);
  m_camera.update(m_gameData, deltaTime);
  m_bullets.update(deltaTime);
  m_shots.update(deltaTime);
  m_waveParticlePattern.update(deltaTime);
  m_healthBar.update(m_gameData, m_ufo);

  if (m_gameData.m_state == State::Playing) {
    checkCollision();
    checkFinish();
  }
}

void OpenGLWindow::checkCollision() {
  // Check enemy collision
  for (auto& shot : m_shots.m_shots) {
    auto distance{glm::distance(m_ufo.getTranslation(), shot.m_translation)};
    if (distance < m_ufo.getScale() + shot.m_scale) {
      m_ufo.takeDamage(shot.m_damage);
      shot.m_dead = true;
    }
  }

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

void OpenGLWindow::checkFinish() {
  if (m_ufo.getHP() <= 0) {
    m_gameData.m_state = State::Finished;
    m_gameData.m_finishTime = m_gameData.m_gameTimer.elapsed();
    m_gameData.m_restartTimer.restart();
  }
}
