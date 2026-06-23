#pragma once

#include "pipeline.h"
#include "resource_engine.h"
#include "tiny_obj_loader.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

namespace wzq {

extern ResourceEngine *g_engine;

template <typename Pipeline> class Teapot {
public:
  Teapot(Pipeline &pipeline)
      : reader(g_engine->LoadObjs("teapot.obj")), pipeline(pipeline) {
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
        const auto ni = static_cast<size_t>(idx.normal_index);
        assert(3 * vi + 2 <= attrib.vertices.size());
        GLfloat x = attrib.vertices[3 * vi];
        GLfloat y = attrib.vertices[3 * vi + 1];
        GLfloat z = attrib.vertices[3 * vi + 2];
        GLfloat nx = attrib.normals[3 * ni];
        GLfloat ny = attrib.normals[3 * ni + 1];
        GLfloat nz = attrib.normals[3 * ni + 2];

        buffer.push_back({x, y, z, nx, ny, nz});

        min.x = std::min(min.x, x);
        min.y = std::min(min.y, y);
        min.z = std::min(min.z, z);
        max.x = std::max(max.x, x);
        max.y = std::max(max.y, y);
        max.z = std::max(max.z, z);
      }
    }

    center = (min + max) * 0.5f;
    vbo = pipeline.GetVBOId();
    pipeline.SetVBOData(vbo, buffer);
  }

  void Draw(glm::mat4 view, glm::mat4 proj, glm::vec3 light) {
    pipeline.Draw(model, view, proj, light);
  }

  glm::vec3 GetCenter() { return model * glm::vec4(center, 1.0); }

  glm::mat4 model;

private:
  tinyobj::ObjReader &reader;
  glm::vec3 center;

  Pipeline &pipeline;
  GLuint vbo;
  std::vector<typename Pipeline::VertexData> buffer;
};

}; // namespace wzq