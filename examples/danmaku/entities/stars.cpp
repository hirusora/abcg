#include "stars.hpp"

#include <cppitertools/itertools.hpp>

void Stars::initializeGL(GLuint program) {
  terminateGL();

  m_re.seed(std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;

  std::vector<glm::vec2> positions(0);
  std::vector<glm::vec3> colors(0);
  std::vector<float> sizes(0);

  std::uniform_real_distribution rp(-1.0f, 1.0f);
  std::uniform_real_distribution ri(0.5f, 0.8f);
  std::uniform_real_distribution rs(10.0f, 1.0f);

  for ([[maybe_unused]] auto i : iter::range(0, m_quantity)) {
    positions.emplace_back(rp(m_re), rp(m_re));
    colors.push_back(m_color * ri(m_re));
    sizes.push_back(rs(m_re));
  }

  // Generate VBOs
  abcg::glGenBuffers(1, &m_vboVertices);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenBuffers(1, &m_vboColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3),
                     colors.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenBuffers(1, &m_vboSizes);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboSizes);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizes.size() * sizeof(float),
                     sizes.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};
  GLint colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};
  GLint sizeAttribute{abcg::glGetAttribLocation(m_program, "inSize")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  abcg::glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glEnableVertexAttribArray(sizeAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboSizes);
  abcg::glVertexAttribPointer(sizeAttribute, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Stars::paintGL() {
  abcg::glUseProgram(m_program);

  abcg::glEnable(GL_BLEND);
  abcg::glBlendFunc(GL_ONE, GL_ONE);

  abcg::glBindVertexArray(m_vao);

  abcg::glDrawArrays(GL_POINTS, 0, m_quantity);

  abcg::glBindVertexArray(0);

  abcg::glDisable(GL_BLEND);

  abcg::glUseProgram(0);
}

void Stars::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vboVertices);
  abcg::glDeleteBuffers(1, &m_vboColors);
  abcg::glDeleteBuffers(1, &m_vboSizes);
  abcg::glDeleteVertexArrays(1, &m_vao);
}
