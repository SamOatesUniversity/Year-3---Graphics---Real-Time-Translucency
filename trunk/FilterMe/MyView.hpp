#pragma once

#include "WindowViewDelegate.hpp"
#include "tgl.h"
#include <glm/glm.hpp>
#include <string>

class MyView : public tyga::WindowViewDelegate
{
public:
	
    MyView();
	
    ~MyView();

    void
    reloadShaders();

    void
    adjustInclination(float degrees);

    void
    adjustAzimuth(float degrees);

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

    struct ShaderProgram
    {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;

        ShaderProgram() : vertex_shader(0),
                          fragment_shader(0),
                          program(0) {}
    };
    ShaderProgram draw_sp_;
    ShaderProgram filter_sp_;

    static GLuint
    compileShader(GLenum type, std::string filepath);

    GLuint quad_vbo_;
    GLuint quad_vao_;

    GLuint cbuffer_fbo_;
    GLuint cbuffer_colour_tex_;
    GLuint cbuffer_depth_rbo_;

    float view_inclination_;
    float view_azimuth_;
};
