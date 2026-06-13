#include <GL/glew.h>

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

static std::string ReadFromFile(const char *path) {
  std::ifstream file(path, std::ios::binary);

  file.seekg(0, std::ios::end);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::string content(static_cast<size_t>(size), '\0');
  file.read(content.data(), size);
  return content;
}

struct VertexData {
  GLfloat pos[3];
  GLfloat clr[4];
};

using ObjBuffer = std::vector<VertexData>;

class Object {
public:
  Object(ObjBuffer &buffer, const char *path) : buffer(buffer) {
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(path)) {
      std::cerr << "Failed to load OBJ: " << path << std::endl;
      abort();
    }

    auto &shapes = reader.GetShapes();
    auto &attrib = reader.GetAttrib();

    glm::vec3 min = {std::numeric_limits<float>::infinity(),
                     std::numeric_limits<float>::infinity(),
                     std::numeric_limits<float>::infinity()};
    glm::vec3 max = {-std::numeric_limits<float>::infinity(),
                     -std::numeric_limits<float>::infinity(),
                     -std::numeric_limits<float>::infinity()};
    for (auto &shape : shapes) {
      for (auto &idx : shape.mesh.indices) {
        const auto vi = static_cast<size_t>(idx.vertex_index);
        assert(3 * vi + 2 <= attrib.vertices.size());
        GLfloat x = attrib.vertices[3 * vi] * 0.05;
        GLfloat y = attrib.vertices[3 * vi + 1] * 0.05;
        GLfloat z = attrib.vertices[3 * vi + 2] * 0.05;
        GLfloat r = attrib.colors[3 * vi];
        GLfloat g = attrib.colors[3 * vi + 1];
        GLfloat b = attrib.colors[3 * vi + 2];

        buffer.push_back({x, y, z, r, g, b, 1.0f});

        min.x = std::min(min.x, x);
        min.y = std::min(min.y, y);
        min.z = std::min(min.z, z);
        max.x = std::max(max.x, x);
        max.y = std::max(max.y, y);
        max.z = std::max(max.z, z);
      }
    }

    center = (min + max) * 0.5f;
  }

  glm::vec3 center;
  ObjBuffer &buffer;
};

class SimplePipeLine {
public:
  SimplePipeLine() : program(0), mvpLocation(-1), vao(0) {}

  void CompileShader() {
    if (program != 0) {
      glDeleteProgram(program);
      program = 0;
    }

    program = glCreateProgram();

    std::string vsSource = ReadFromFile("shader/mvp.vert");
    const char *vsSourcePtr = vsSource.c_str();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSourcePtr, nullptr);
    glCompileShader(vs);
    CheckShaderCompile(vs, "vertex");
    glAttachShader(program, vs);
    glDeleteShader(vs);

    std::string fsSource = ReadFromFile("shader/one_color.frag");
    const char *fsSourcePtr = fsSource.c_str();
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSourcePtr, nullptr);
    glCompileShader(fs);
    CheckShaderCompile(fs, "fragment");
    glAttachShader(program, fs);
    glDeleteShader(fs);

    glLinkProgram(program);
    CheckProgramLink(program);

    glUseProgram(program);
    mvpLocation = glGetUniformLocation(program, "mvp");
    assert(mvpLocation != -1);

    model = glm::rotate(glm::identity<glm::mat4>(), glm::radians(-60.0f),
                        glm::vec3(1, 0, 0));
    if (!objs.empty()) {
      model = glm::translate(glm::identity<glm::mat4>(), objs[0].center) *
              glm::rotate(glm::identity<glm::mat4>(), glm::radians(-60.0f),
                          glm::vec3(1, 0, 0)) *
              glm::translate(glm::identity<glm::mat4>(), -objs[0].center);
    }

    view =
        glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    glm::f32 fovy = glm::radians(40.0f);
    glm::f32 zNear = 0.1f;
    glm::f32 zFar = 1000.0f;
    glm::f32 aspect =
        glutGet(GLUT_WINDOW_WIDTH) * 1.0f / glutGet(GLUT_WINDOW_HEIGHT);
    projection = glm::perspective(fovy, aspect, zNear, zFar);

    glUniform1f(glGetUniformLocation(program, "alpha"), 1.0f);
  }

  void CreateVertexBuffer(const char *path) {
    objs.emplace_back(buffer, path);
    assert(buffer.size());

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(VertexData),
                 buffer.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
  }

  void Render() {
    assert(program != 0 && vao != 0 && buffer.size() != 0);
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);
    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, buffer.size());
    glBindVertexArray(0);
  }

  void ChangeView(glm::mat4 transform) { view = transform * view; }

private:
  void CheckShaderCompile(GLuint shader, const char *stage) {
    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
      GLint length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
      assert(length > 0);
      std::string log(length + 1, '\0');
      glGetShaderInfoLog(shader, length, nullptr, log.data());
      std::cerr << stage << " shader compile error:\n"
                << log.data() << std::endl;
    }
  }

  void CheckProgramLink(GLuint programHandle) {
    GLint status = GL_FALSE;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
      GLint length = 0;
      glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &length);
      assert(length > 0);
      std::string log(length + 1, '\0');
      glGetProgramInfoLog(programHandle, length, nullptr, log.data());
      std::cerr << "Program link error:\n" << log.data() << std::endl;
    }
  }

  GLuint program;
  GLint mvpLocation;
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
  GLuint vao;

  ObjBuffer buffer;
  std::vector<Object> objs;
} g_pipeline;

void myDisplayFunc() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  g_pipeline.Render();
  glutSwapBuffers();
}

void myIdleFunc() { glutPostRedisplay(); }

void myKeyboardFunc(unsigned char key, int x, int y) {
  switch (key) {
  case 27: // ESC
    glutLeaveMainLoop();
    break;
  }
  glutPostRedisplay();
}

void mySpecialFunc(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_F6:
    g_pipeline.CompileShader();
    break;
  }
}

float mouseLeftSpeed = 0.05f;   // rotate angle per mouse motion pixcel
float mouseRightSpeed = 0.005f; // xy position per mouse motion pixcel
int mouseKey = 0;
int lastU = 0;
int lastV = 0;
void myMouseFunc(int key, int up_or_down, int u, int v) {
  mouseKey = key;
  lastU = u;
  lastV = v;
}

void myMotionFunc(int u, int v) {
  float du = u - lastU;
  float dv = lastV - v;
  switch (mouseKey) {
  case GLUT_LEFT_BUTTON: {
    float len = std::sqrt(du * du + dv * dv);
    glm::mat4 transform =
        glm::rotate(glm::identity<glm::mat4>(),
                    glm::radians(len * mouseLeftSpeed), glm::vec3(dv, -du, 0));
    g_pipeline.ChangeView(transform);
    break;
  }
  case GLUT_RIGHT_BUTTON: {
    glm::mat4 transform = glm::translate(
        glm::identity<glm::mat4>(), glm::vec3(du, dv, 0) * mouseRightSpeed);
    g_pipeline.ChangeView(transform);
    break;
  }

  default:
    break;
  }

  lastU = u;
  lastV = v;

  glutPostRedisplay();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitContextVersion(4, 6);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
#ifdef DEBUG
  glutInitContextFlags(GLUT_DEBUG);
#endif

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow("CS6610 homework2");

  glutDisplayFunc(myDisplayFunc);
  glutIdleFunc(myIdleFunc);
  glutKeyboardFunc(myKeyboardFunc);
  glutSpecialFunc(mySpecialFunc);
  glutMouseFunc(myMouseFunc);
  glutMotionFunc(myMotionFunc);

  glewExperimental = GL_TRUE;
  assert(glewInit() == GLEW_OK);

  g_pipeline.CompileShader();
  g_pipeline.CreateVertexBuffer("resources/teapot.obj");

  glutMainLoop();

  return 0;
}