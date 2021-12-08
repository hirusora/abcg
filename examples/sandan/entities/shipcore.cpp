#include "shipcore.hpp"

#include <filesystem>

void ShipCore::initializeGL(GLuint program, std::string path) {
  Model::initializeGL(program, path);

  auto directory{std::filesystem::path(path).parent_path().string()};
  auto filePath{directory + "/textures/red.png"};
  if (std::filesystem::exists(filePath)) {
    abcg::glDeleteTextures(1, &m_texture);
    m_texture = abcg::opengl::loadTexture(filePath);
    setDiffuseTexture(m_texture);
  }
}

void ShipCore::paintGL(glm::mat4 viewMatrix, glm::vec3 translation) {
  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, translation);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(m_scale));

  render(modelMatrix, viewMatrix);
}

void ShipCore::terminateGL() {
  abcg::glDeleteTextures(1, &m_texture);
  Model::terminateGL();
}
