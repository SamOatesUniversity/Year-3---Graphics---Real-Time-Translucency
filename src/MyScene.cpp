#include "MyScene.hpp"
#include "FirstPersonMovement.hpp"
#include <tcf/SimpleScene.hpp>
#include <iostream>

MyScene::
MyScene()
{
    start_time_ = std::chrono::system_clock::now();
    time_seconds_ = 0.f;

    if (!readFile("assets/sponza.tcf")) {
        std::cerr << "Failed to read sponza.tcf data file" << std::endl;
    }

    camera_.reset(new FirstPersonMovement());
    camera_->init(glm::vec3(80, 50, 0), 1.5f, 0.5f);
}

MyScene::
~MyScene()
{
}

void MyScene::
setCameraTranslationSpeed(float sideward,
                          float forward)
{
    camera_translation_speed_.x = sideward;
    camera_translation_speed_.y = 0;
    camera_translation_speed_.z = forward;
}

void MyScene::
setCameraRotationSpeed(float horizontal,
                       float vertical)
{
    camera_rotation_speed_.x = horizontal;
    camera_rotation_speed_.y = vertical;
}

bool MyScene::
readFile(std::string filepath)
{
    tcf::Error error;
    tcf::SimpleScene tcf_scene = tcf::simpleSceneFromFile(filepath, &error);
    if (error != tcf::kNoError) {
        return false;
    }

    meshes_.clear();
    models_.clear();

    meshes_.reserve(tcf_scene.meshArray.size());
    for (const auto& mesh : tcf_scene.meshArray) {
        models_.reserve(models_.size() + mesh.instanceArray.size());
        for (const auto& model : mesh.instanceArray) {
            Model new_model;
            new_model.mesh_index = meshes_.size();
            new_model.material_index = 0;
            new_model.xform = glm::mat4x3(model.m00, model.m01, model.m02,
                                          model.m10, model.m11, model.m12,
                                          model.m20, model.m21, model.m22,
                                          model.m30, model.m31, model.m32);
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

    // hardcode some material data ... not good practice!
    int redShapes[] = { 35, 36, 37, 38, 39, 40, 41, 42, 69, 70, 71, 72, 73, 74,
                        75, 76, 77, 78, 79 };
    int greenShapes[] = { 8, 19, 31, 33, 54, 57, 67, 68, 66, 80 };
    int yellowShapes[] = { 4, 5, 6, 7, 23, 24, 25, 26, 27, 28, 29, 30, 44, 45,
                           46, 47, 48, 49, 50, 51, 52, 53 };
    int *shapes[3] = { redShapes, greenShapes, yellowShapes };
    int numberOfShapes[] = { sizeof(redShapes) / sizeof(int),
                             sizeof(greenShapes) / sizeof(int),
                             sizeof(yellowShapes) / sizeof(int) };
    glm::vec3 colours[3] = { glm::vec3(1.f, 0.0f, 0.f),
                             glm::vec3(0.2f, 0.8f, 0.2f),
                             glm::vec3(0.8f, 0.8f, 0.2f) };
    float shininess[3] = { 0.f, 64.f, 128.f };
    Material new_material;
    new_material.colour = glm::vec3(0.8f, 0.8f, 0.8f);
    new_material.shininess = 0.f;
    materials_.push_back(new_material);
    for (int j=0; j<3; ++j) {
        Material new_material;
        new_material.colour = colours[j];
        new_material.shininess = shininess[j];
        materials_.push_back(new_material);
        for (int i=0; i<numberOfShapes[j]; ++i) {
            int index = shapes[j][i];
            models_[index].material_index = j+1;
        }
    }

    return true;
}

void MyScene::
update()
{
    const auto clock_time = std::chrono::system_clock::now() - start_time_;
    const auto clock_millisecs
        = std::chrono::duration_cast<std::chrono::milliseconds>(clock_time);
    const float prev_time = time_seconds_;
    time_seconds_ = 0.001f * clock_millisecs.count();
    const float dt = time_seconds_ - prev_time;

    camera_->moveForward(camera_translation_speed_.z * dt);
    camera_->moveRight(camera_translation_speed_.x * dt);
    camera_->spinHorizontal(camera_rotation_speed_.x * dt);
    camera_->spinVertical(camera_rotation_speed_.y * dt);
}

float MyScene::
time() const
{
    return time_seconds_;
}

glm::vec3 MyScene::
upDirection() const
{
    return glm::vec3(0.f, 1.f, 0.f);
}

glm::vec3 MyScene::
globalLightDirection() const
{
    return glm::normalize(glm::vec3(-10, -5, -2));
}

MyScene::Camera MyScene::
camera() const
{
    Camera cam;
    cam.position = camera_->position();
    cam.direction = camera_->direction();
    cam.vertical_field_of_view_degrees = 75.f;
    cam.near_plane_distance = 1.f;
    cam.far_plane_distance = 1000.f;
    return cam;
}

int MyScene::
lightCount() const
{
    return 7;
}

MyScene::Light MyScene::
light(int index) const
{
    Light light;
    if (index == 0) {
        light.position = glm::vec3(50.f * cosf(time_seconds_), 50.f, 0.f);
        light.range = 70.f;
    } else {
        float A = time_seconds_ + index * 6.28f / 6.f;
        light.position  = glm::vec3(120.f * cosf(A), 10.f, 40.f * sinf(A));
        light.range = 20.f;
    }
    return light;
}

int MyScene::
materialCount() const
{
    return materials_.size();
}

MyScene::Material MyScene::
material(int index) const
{
    return materials_[index];
}

int MyScene::
meshCount() const
{
    return meshes_.size();
}

const MyScene::Mesh& MyScene::
mesh(int index) const
{
    return meshes_[index];
}

int MyScene::
modelCount() const
{
    return models_.size();
}

MyScene::Model MyScene::
model(int index) const
{
    return models_[index];
}
