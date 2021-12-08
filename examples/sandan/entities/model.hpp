#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <vector>

#include "abcg.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};
  glm::vec4 tangent{};

  bool operator==(const Vertex& other) const noexcept {
    static const auto epsilon{std::numeric_limits<float>::epsilon()};
    return glm::all(glm::epsilonEqual(position, other.position, epsilon)) &&
           glm::all(glm::epsilonEqual(normal, other.normal, epsilon)) &&
           glm::all(glm::epsilonEqual(texCoord, other.texCoord, epsilon));
  }
};

class Model {
 public:
  void initializeGL(GLuint program, std::string_view path);
  void terminateGL();

  void loadCubeTexture(const std::string& path);
  void loadDiffuseTexture(std::string_view path);
  void loadNormalTexture(std::string_view path);
  void loadObj(std::string_view path, bool standardize = true);
  void render(glm::mat4 modelMatrix, glm::mat4 viewMatrix) const;
  void setupVAO(GLuint program);
  void setupLocators(GLuint program);

  void setDiffuseTexture(GLuint texture) { m_diffuseTexture = texture; };
  void setMappingMode(int mode) { m_mappingMode = mode; };

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  GLint m_modelMatrixLoc;
  GLint m_normalMatrixLoc;
  GLint m_KaLoc;
  GLint m_KdLoc;
  GLint m_KsLoc;
  GLint m_shininessLoc;
  GLint m_mappingModeLoc;

  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};
  int m_mappingMode{3};
  GLuint m_diffuseTexture{};
  GLuint m_normalTexture{};
  GLuint m_cubeTexture{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  void computeNormals();
  void computeTangents();
  void createBuffers();
  void standardize();
};

#endif