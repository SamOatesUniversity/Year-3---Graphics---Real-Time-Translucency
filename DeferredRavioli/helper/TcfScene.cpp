#include "TcfScene.hpp"
#include <tcf/SimpleScene.hpp>
#include <memory>

TcfScene::
TcfScene()
{
}

TcfScene::
~TcfScene()
{
}

bool TcfScene::
readFile(std::string filepath)
{
    std::shared_ptr<tcf::SimpleScene> tcf_scene(new tcf::SimpleScene());

    if (!tcf::readFile(filepath.c_str(), tcf_scene.get())) {
        return false;
    }

    meshes_.clear();
    models_.clear();

    meshes_.reserve(tcf_scene->meshArray.size());
    for (const auto& mesh : tcf_scene->meshArray) {
        models_.reserve(models_.size() + mesh.instanceArray.size());
        for (const auto& model : mesh.instanceArray) {
            Model new_model;
            new_model.mesh_index = meshes_.size();
            new_model.xform = glm::mat4(  model.m00, model.m01, model.m02, 0.0f,
                                          model.m10, model.m11, model.m12, 0.0f,
                                          model.m20, model.m21, model.m22, 0.0f,
                                          model.m30, model.m31, model.m32, 1.0f);
            models_.push_back(new_model);
        }
        Mesh new_mesh;
        new_mesh.element_array.assign((unsigned int*)&mesh.indexArray.front(),
                                      (unsigned int*)&mesh.indexArray.back()+1);
        new_mesh.normal_array.assign((glm::vec3*)&mesh.normalArray.front(),
                                     (glm::vec3*)&mesh.normalArray.back()+1);
        new_mesh.position_array.assign((glm::vec3*)&mesh.vertexArray.front(),
                                       (glm::vec3*)&mesh.vertexArray.back()+1);
        new_mesh.tangent_array.assign((glm::vec3*)&mesh.tangentArray.front(),
                                      (glm::vec3*)&mesh.tangentArray.back()+1);
        new_mesh.texcoord_array.assign((glm::vec2*)&mesh.texcoordArray.front(),
                                      (glm::vec2*)&mesh.texcoordArray.back()+1);
        meshes_.push_back(new_mesh);
    }

    return true;
}

int TcfScene::
meshCount() const
{
    return meshes_.size();
}

const TcfScene::Mesh& TcfScene::
mesh(int index)
{
    return meshes_[index];
}

int TcfScene::
modelCount() const
{
    return models_.size();
}

TcfScene::Model TcfScene::
model(int index) const
{
    return models_[index];
}
