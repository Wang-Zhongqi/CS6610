#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "resource_engine.h"
#include "scane.h"
#include "utils.h"

namespace wzq {

GLFWwindow *g_window = nullptr;
wzq::ResourceEngine *g_engine = nullptr;
wzq::TeapotScane *g_scane = nullptr;

float mouseLeftSpeed = 0.0005f; // rotate angle per mouse motion pixel
float mouseRightSpeed = 0.005f; // xy position per mouse motion pixel
float lightRotateSpeed = 1.0f;  // xy position per mouse motion pixel
int mouseKey = -1;
int lastU = 0;
int lastV = 0;
int mods;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (action == GLFW_PRESS) {
    switch (key) {
    case GLFW_KEY_ESCAPE: {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
    }
    case GLFW_KEY_F6: {
      g_scane->Reset();
      break;
    }
    default:
      break;
    }
  }
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  lastU = static_cast<int>(x);
  lastV = static_cast<int>(y);
  wzq::mods = mods;
  if (action == GLFW_PRESS) {
    mouseKey = button;
  } else if (action == GLFW_RELEASE) {
    mouseKey = -1;
  }
}

void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
  int u = static_cast<int>(xpos);
  int v = static_cast<int>(ypos);
  float du = u - lastU;
  float dv = lastV - v;
  if (mods == 0) {
    switch (mouseKey) {
    case GLFW_MOUSE_BUTTON_LEFT: {
      float len = std::sqrt(du * du + dv * dv);
      glm::mat4 transform = glm::rotate(glm::identity<glm::mat4>(),
                                        glm::radians(len * mouseLeftSpeed),
                                        glm::normalize(glm::vec3(dv, -du, 0)));
      g_scane->ChangeView(transform);
      break;
    }
    case GLFW_MOUSE_BUTTON_RIGHT: {
      glm::mat4 transform = glm::translate(
          glm::identity<glm::mat4>(), glm::vec3(du, dv, 0) * mouseRightSpeed);
      g_scane->ChangeView(transform);
      break;
    }
    default:
      break;
    }
  } else if (mods == GLFW_MOD_CONTROL) {
    switch (mouseKey) {
    case GLFW_MOUSE_BUTTON_LEFT: {
      float len = std::sqrt(du * du + dv * dv);
      g_scane->RotateLight(glm::radians(len * lightRotateSpeed),
                           glm::normalize(glm::vec3(dv, -du, 0)));
      break;
    }
    }
  }

  lastU = u;
  lastV = v;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
} // namespace wzq

using namespace wzq;

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

  g_window = glfwCreateWindow(1920, 1080, "CS6610 homework3", nullptr, nullptr);
  if (!g_window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwSetWindowAttrib(g_window, GLFW_FLOATING, GLFW_TRUE);
  glfwMakeContextCurrent(g_window);
  glfwSetKeyCallback(g_window, key_callback);
  glfwSetMouseButtonCallback(g_window, mouse_button_callback);
  glfwSetCursorPosCallback(g_window, cursor_pos_callback);
  glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);

  int framebufferWidth, framebufferHeight;
  glfwGetFramebufferSize(g_window, &framebufferWidth, &framebufferHeight);
  glViewport(0, 0, framebufferWidth, framebufferHeight);

  glewExperimental = GL_TRUE;
  GLEW_DCHECK(glewInit());

  g_engine = new wzq::ResourceEngine();
  g_scane = new wzq::TeapotScane();
  while (!glfwWindowShouldClose(g_window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    g_scane->Draw();
    glfwSwapBuffers(g_window);
    glfwPollEvents();
  }

  glfwDestroyWindow(g_window);
  glfwTerminate();

  return 0;
}