#pragma once

#include <GL/glew.h>
#include <fstream>
#include <iostream>

static std::string ReadFromFile(std::string path) {
  std::ifstream file(path, std::ios::binary);

  file.seekg(0, std::ios::end);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::string content(static_cast<size_t>(size), '\0');
  file.read(content.data(), size);
  return content;
}

inline void GLEW_DCHECK(GLenum error) {
#ifndef NDEBUG
  if (error != GLEW_OK) {
    std::cerr << "GLEW ERROR: " << glewGetErrorString(error);
    abort();
  }
#endif
}

inline void GL_DCHECK(GLint value) {
#ifndef NDEBUG
  GLenum error;
  if (value == -1 && (error = glGetError()) != 0) {
    std::cerr << "GL ERROR: " << gluErrorString(error);
    abort();
  }
#endif
}

inline void GL_DCheckShaderCompile(GLuint shader) {
#ifndef NDEBUG
  GLint status = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    std::string log(length + 1, '\0');
    glGetShaderInfoLog(shader, length, nullptr, log.data());
    std::cerr << "Shader compile error:\n" << log.data() << std::endl;
  }
#endif
}

inline void GL_DCheckProgramLink(GLuint program) {
#ifndef NDEBUG
  GLint status = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    std::string log(length + 1, '\0');
    glGetProgramInfoLog(program, length, nullptr, log.data());
    std::cerr << "Program link error:\n" << log.data() << std::endl;
  }
#endif
}