#include "scane.h"

namespace wzq {

TeapotScane::TeapotScane() : pipeline(), teapot(pipeline) {
  glm::mat4 &model = teapot.model;
  model = glm::identity<glm::mat4>();
  model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
  model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(1, 0, 0));

  view =
      glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

  glm::f32 fovy = glm::radians(40.0f);
  glm::f32 zNear = 0.1f;
  glm::f32 zFar = 1000.0f;
  int fbw, fbh;
  glfwGetFramebufferSize(g_window, &fbw, &fbh);
  glm::f32 aspect = fbw * 1.0f / fbh;
  proj = glm::perspective(fovy, aspect, zNear, zFar);

  light = teapot.GetCenter();
  light.z += 10.0;
}

void TeapotScane::Draw() { teapot.Draw(view, proj, light); }

void TeapotScane::ChangeView(glm::mat4 transform) { view = transform * view; }

void TeapotScane::Reset() {
  pipeline.RecompileShader();
  glm::vec3 centor = teapot.GetCenter();
  glm::vec3 eye = centor + glm::vec3(0, 0, 3);
  view = glm::lookAt(eye, centor, glm::vec3(0, 1, 0));
}

void TeapotScane::RotateLight(float angle, glm::vec3 axis) {
  glm::mat4 translate = glm::identity<glm::mat4>();
  translate = glm::translate(translate, -teapot.GetCenter());
  translate = glm::rotate(translate, angle, axis);
  translate = glm::translate(translate, teapot.GetCenter());
  light = translate * glm::vec4(light, 1.0);
}

}; // namespace wzq
