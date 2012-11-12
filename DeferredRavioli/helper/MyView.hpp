#pragma once

#include "WindowViewDelegate.hpp"
#include "tgl.h"

#include <glm/glm.hpp>
#include <chrono>
#include <vector>
#include <memory>

class TcfScene;

class MyView : public tyga::WindowViewDelegate
{
public:
	
    MyView();
	
    ~MyView();

    void
    reloadShaders();

private:

    void
    windowViewWillStart(std::shared_ptr<tyga::Window> window);
	
    void
    windowViewDidReset(std::shared_ptr<tyga::Window> window,
                       int width,
                       int height);

    void
    windowViewDidStop(std::shared_ptr<tyga::Window> window);
    
    void
    windowViewRender(std::shared_ptr<tyga::Window> window);

private:

    std::chrono::system_clock::time_point start_time_;

    struct ShaderProgram
    {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;

        ShaderProgram() : vertex_shader(0),
                          fragment_shader(0),
                          program(0) {}
    };
    ShaderProgram shading_;

    struct Vertex
    {
		glm::vec3 position;
		glm::vec3 normal;
    };

    struct Mesh
    {
        GLuint vertex_vbo;
        GLuint element_vbo;
        GLuint vao;
        int element_count;

        Mesh() : vertex_vbo(0),
                 element_vbo(0),
                 vao(0),
                 element_count(0) {}
    };

    std::vector<Mesh> meshes_;

    std::shared_ptr<TcfScene> scene_;

	struct {
		GLuint						texture;					//!< The GL Id of our shadow texture
		GLuint						frameBuffer;				//!< The open GL frame buffer used for the depth texture
		ShaderProgram				shader;						//!< The shading program to use with shadow rendering
		int							shadowMapSize;				//!< THe size of the shadow map texture
	} m_shadow;

	ShaderProgram					m_ambient;					//!< The ambient shader
};
