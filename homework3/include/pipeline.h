#pragma once

#include <GL/glew.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>

#include "resource_engine.h"

namespace wzq {

extern ResourceEngine *g_engine;

class BlinnPipeline {
public:
  BlinnPipeline();
  //   ~BlinnPipeline() { glDeletePorgram(program); }

  struct VertexData {
    GLfloat pos[3];
    GLfloat normal[3];
  };

  void CompileShader();
  void BindVertexAttrib();
  void BindUniform();

  void RecompileShader();

  GLuint GetVBOId() const;
  void SetVBOData(GLuint vbo, const std::vector<VertexData> &buffer);

  void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 proj,
            glm::vec3 light) const;

private:
  GLuint program;
  GLuint vao;
  GLuint size;
  GLuint vs;
  GLuint fs;

  GLint modelLoc;
  GLint viewLoc;
  GLint projLoc;
  GLint normalLoc;
  GLint lightLoc;
};

} // namespace wzq
