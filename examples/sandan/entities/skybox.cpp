
#include "skybox.hpp"

#include <filesystem>

void Skybox::initializeGL(GLuint program, std::string path) {
  loadTexture(path);

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_positions), m_positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};

  // Get location of uniform variables
  m_viewMatrixLoc = abcg::glGetUniformLocation(program, "viewMatrix");
  m_projMatrixLoc = abcg::glGetUniformLocation(program, "projMatrix");
  m_skyTexLoc = abcg::glGetUniformLocation(program, "skyTex");

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Skybox::terminateGL() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Skybox::loadTexture(std::string path) {
  if (!std::filesystem::exists(path)) return;

  abcg::glDeleteTextures(1, &m_texture);
  m_texture = abcg::opengl::loadCubemap({path + "posx.png", path + "negx.png",
                                         path + "posy.png", path + "negy.png",
                                         path + "posz.png", path + "negz.png"});
}

void Skybox::paintGL(glm::mat4 projMatrix, glm::mat4 viewMatrix) const {
  // Set uniform variables
  abcg::glUniformMatrix4fv(m_viewMatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLoc, 1, GL_FALSE, &projMatrix[0][0]);
  abcg::glUniform1i(m_skyTexLoc, 0);

  abcg::glBindVertexArray(m_VAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

  abcg::glEnable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CW);
  abcg::glDepthFunc(GL_LEQUAL);
  abcg::glDrawArrays(GL_TRIANGLES, 0, m_positions.size());
  abcg::glDepthFunc(GL_LESS);

  abcg::glBindVertexArray(0);
}
