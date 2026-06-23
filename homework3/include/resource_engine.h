#pragma once

#include <GL/glew.h>
#include <unordered_map>

#include "tiny_obj_loader.h"

namespace wzq {

class ResourceEngine {
public:
  GLuint CompileShader(std::string name, uint32_t type);
  GLuint RecompileShader(std::string name, uint32_t type);
  tinyobj::ObjReader &LoadObjs(std::string name);
  std::unordered_map<std::string, GLuint> shaders;
  std::unordered_map<std::string, tinyobj::ObjReader> objs;
};

} // namespace wzq