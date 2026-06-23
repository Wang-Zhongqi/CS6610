
#define TINYOBJLOADER_IMPLEMENTATION
#include "resource_engine.h"

#include "utils.h"

namespace wzq {

GLuint ResourceEngine::CompileShader(std::string name, uint32_t type) {
  auto iter = shaders.find(name);
  if (iter != shaders.end()) {
    return iter->second;
  }

  return RecompileShader(name, type);
}

GLuint ResourceEngine::RecompileShader(std::string name, uint32_t type) {
  std::string source = ReadFromFile("shader/" + name);
  const char *ptr = source.c_str();
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &ptr, nullptr);
  glCompileShader(shader);
  GL_DCheckShaderCompile(shader);

  shaders[name] = shader;
  return shader;
}

tinyobj::ObjReader &ResourceEngine::LoadObjs(std::string name) {
  auto iter = objs.find(name);
  if (iter != objs.end()) {
    return iter->second;
  }

  tinyobj::ObjReader &reader = objs[name];
  if (!reader.ParseFromFile("objs/" + name)) {
    std::cerr << "Failed to load OBJ: " << name << std::endl;
    abort();
  }
  return reader;
}

} // namespace wzq