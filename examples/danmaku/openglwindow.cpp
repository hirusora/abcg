#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <map>
#include <vector>

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_LSHIFT ||
        event.key.keysym.sym == SDLK_RSHIFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Focus));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_LSHIFT ||
        event.key.keysym.sym == SDLK_RSHIFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Focus));
  }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "KawashiroGothic-Lla5.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  m_font_desc = io.Fonts->AddFontFromFileTTF(filename.c_str(), 30.0f);
  if (m_font == nullptr || m_font_desc == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Create program to render the other objects
  m_program = createProgramFromFile(getAssetsPath() + "objects.vert",
                                    getAssetsPath() + "objects.frag");
  m_starsProgram = createProgramFromFile(getAssetsPath() + "stars.vert",
                                         getAssetsPath() + "stars.frag");

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_stars.paintGL();
  m_shots.paintGL(m_gameData);
  m_ship.paintGL(m_gameData);
  m_bullets.paintGL(m_gameData);
  m_enemy.paintGL(m_gameData);
  m_healthBar.paintGL(m_gameData);
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
    const auto deathsMessage{fmt::format("Deaths: {}", m_ship.m_deaths)};
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

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_program);

  m_stars.terminateGL();
  m_shots.terminateGL();
  m_ship.terminateGL();
  m_bullets.terminateGL();
  m_enemy.terminateGL();
  m_healthBar.terminateGL();
}

// Own methods.

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;
  m_gameData.m_finishTime = 0;
  m_gameData.m_gameTimer.restart();

  m_stars.initializeGL(m_starsProgram);
  m_shots.initializeGL(m_program);
  m_ship.initializeGL(m_program);
  m_bullets.initializeGL(m_program);
  m_enemy.initializeGL(m_program);
  m_healthBar.initializeGL(m_program);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing && m_restartTimer.elapsed() > 5) {
    restart();
    return;
  }

  m_ship.update(m_gameData, deltaTime);
  m_enemy.update(m_gameData, deltaTime);
  m_bullets.update(m_enemy, m_gameData, deltaTime);
  m_shots.update(m_ship, m_gameData, deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkCollision();
    checkFinished();
  }

  // Updates after collision to properly represent enemy's health.
  m_healthBar.update(m_enemy, m_gameData);
}

void OpenGLWindow::checkCollision() {
  // Check enemy collision
  for (auto &shot : m_shots.m_shots) {
    auto distance{glm::distance(m_enemy.m_translation, shot.m_translation)};
    if (distance < m_enemy.m_scale + m_shots.m_scale) {
      m_enemy.m_hp -= shot.m_damage;
      shot.m_dead = true;
    }
  }

  // Check ship's core collision
  if (m_ship.m_state != ShipState::Invulnerable) {
    for (auto bullet : m_bullets.m_bullets) {
      auto distance{glm::distance(m_ship.m_translation, bullet.m_translation)};
      if (distance < m_ship.m_core.m_scale + m_bullets.m_scale * 0.9) {
        m_ship.takeHit();
      }
    }

    auto distance{glm::distance(m_ship.m_translation, m_enemy.m_translation)};
    if (distance < m_ship.m_core.m_scale + m_enemy.m_scale * 0.9) {
      m_ship.takeHit();
    }
  }
}

void OpenGLWindow::checkFinished() {
  if (m_enemy.m_hp <= 0) {
    m_gameData.m_state = State::Finished;
    m_gameData.m_finishTime = m_gameData.m_gameTimer.elapsed();
    m_restartTimer.restart();
  }
}
