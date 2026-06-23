#pragma once

#include <glm/matrix.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

#include "pipeline.h"
#include "scane_objects.h"

namespace wzq {

extern GLFWwindow *g_window;

class TeapotScane {
public:
  TeapotScane();
  void Draw();

  void ChangeView(glm::mat4 transform);
  void Reset();
  void RotateLight(float angle, glm::vec3 axis);

private:
  BlinnPipeline pipeline;
  Teapot<BlinnPipeline> teapot;
  glm::mat4 view;
  glm::mat4 proj;
  glm::vec3 light;
};

} // namespace wzq
