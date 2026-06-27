#include "pipeline.h"

#include "utils.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/matrix.hpp>
namespace wzq {

BlinnPipeline::BlinnPipeline() : program(glCreateProgram()), size(0) {
  CompileShader();
  BindVertexAttrib();
  BindUniform();
}

void BlinnPipeline::CompileShader() {
  vs = g_engine->CompileShader("blinn.vert", GL_VERTEX_SHADER);
  fs = g_engine->CompileShader("blinn.frag", GL_FRAGMENT_SHADER);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  GL_DCheckProgramLink(program);

  // glDeleteShader(vs);
  // glDeleteShader(fs);
}

void BlinnPipeline::RecompileShader() {
  vs = g_engine->RecompileShader("blinn.vert", GL_VERTEX_SHADER);
  fs = g_engine->RecompileShader("blinn.frag", GL_FRAGMENT_SHADER);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  GL_DCheckProgramLink(program);
}

void BlinnPipeline::BindVertexAttrib() { glGenVertexArrays(1, &vao); }

void BlinnPipeline::BindUniform() {
  GL_DCHECK(modelLoc = glGetUniformLocation(program, "model"));
  GL_DCHECK(viewLoc = glGetUniformLocation(program, "view"));
  GL_DCHECK(projLoc = glGetUniformLocation(program, "projection"));
  GL_DCHECK(normalLoc = glGetUniformLocation(program, "normalMatrix"));
  GL_DCHECK(lightLoc = glGetUniformLocation(program, "lightPosition"));
}

GLuint BlinnPipeline::GetVBOId() const {
  glBindVertexArray(vao);
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindVertexArray(0);
  return vbo;
}

void BlinnPipeline::SetVBOData(GLuint vbo,
                               const std::vector<VertexData> &buffer) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(VertexData),
               buffer.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        (void *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  size += buffer.size();
}

void BlinnPipeline::Draw(glm::mat4 model, glm::mat4 view, glm::mat4 proj,
                         glm::vec3 light) const {
  glUseProgram(program);

  glm::mat3 normalMatrix = glm::transpose(glm::inverse(model));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
  glUniformMatrix3fv(normalLoc, 1, GL_FALSE, &normalMatrix[0][0]);
  glUniform3fv(lightLoc, GL_FALSE, &light[0]);

  glEnable(GL_DEPTH_TEST);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, size);
  glBindVertexArray(0);
}

} // namespace wzq