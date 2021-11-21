#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "model.vert",
                                    getAssetsPath() + "model.frag");

  // Get location of uniform variables
  m_viewMatrixLoc = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLoc = abcg::glGetUniformLocation(m_program, "projMatrix");

  // Load models
  m_bullets.initializeGL(m_program, getAssetsPath() + "bullet.obj");

  restart();

  resizeGL(getWindowSettings().width, getWindowSettings().height);
  m_camera.computeViewMatrix();
}

void OpenGLWindow::paintGL() {
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

  m_bullets.paintGL();

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  {
    const auto widgetSize{ImVec2(218, 62)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr, ImGuiWindowFlags_NoDecoration);

    {
      ImGui::PushItemWidth(120);
      const std::vector<std::string> comboItems{"Nuclear", "Wave / Particle"};

      if (ImGui::BeginCombo("Pattern", comboItems.at(m_patternIndex).c_str())) {
        for (const auto index : iter::range(comboItems.size())) {
          const bool isSelected{m_patternIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected)) {
            m_patternIndex = index;
            restart();
          }
          if (isSelected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      ImGui::PushItemWidth(170);
      if (ImGui::SliderFloat("FOV", &m_camera.m_FOV, 5.0f, 179.0f,
                             "%.0f degrees")) {
        m_camera.computeProjectionMatrix(m_viewportWidth, m_viewportHeight);
      }
      ImGui::PopItemWidth();
    }

    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_bullets.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::restart() {
  m_camera.restart();
  m_bullets.restart();
  m_nuclearPattern.restart(&m_bullets);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};
  m_camera.update(deltaTime);
  m_bullets.update(deltaTime);

  if (m_patternIndex == 0) {
    m_nuclearPattern.update(deltaTime);
  }
}