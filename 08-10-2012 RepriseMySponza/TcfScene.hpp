#pragma once
#ifndef __TCF_SCENE__
#define __TCF_SCENE__

#include <glm/glm.hpp>
#include <string>
#include <vector>

class TcfScene
{
public:
    
    TcfScene();

    ~TcfScene();

    bool
    readFile(std::string filepath);

    struct Mesh
    {
        std::vector<glm::vec3> position_array;
        std::vector<glm::vec3> normal_array;
        std::vector<glm::vec3> tangent_array;
        std::vector<glm::vec2> texcoord_array;
        std::vector<unsigned int> element_array;
    };

    int
    meshCount() const;

    const Mesh&
    mesh(int index);

    struct Model
    {
        unsigned int mesh_index;
        glm::mat4x3 xform;
    };

    int
    modelCount() const;

    Model
    model(int index) const;

private:

    std::vector<Mesh> meshes_;

    std::vector<Model> models_;

};

#endif
