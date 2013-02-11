#include "MyView.hpp"
#include "FileHelper.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

MyView::
MyView() : cbuffer_fbo_(0),
           cbuffer_colour_tex_(0),
           cbuffer_depth_rbo_(0),
           view_azimuth_(0),
           view_inclination_(60),
           quad_vbo_(0),
           quad_vao_(0)
{
}

MyView::
~MyView() {
}

void MyView::
adjustInclination(float degrees)
{
    view_inclination_ += degrees;
}

void MyView::
adjustAzimuth(float degrees)
{
    view_azimuth_ += degrees;
}

GLuint MyView::
compileShader(GLenum type, std::string filepath)
{
    GLuint shader = 0;

    shader = glCreateShader(type);
    std::string shader_string = tyga::stringFromFile(filepath);
    const char *shader_code = shader_string.c_str();
    glShaderSource(shader, 1, (const GLchar **) &shader_code, NULL);
    glCompileShader(shader);

    GLint compile_status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        const int string_length = 1024;
        GLchar log[string_length]= "";
        glGetShaderInfoLog(shader, string_length, NULL, log);
        std::cerr << log << std::endl;
    }

    return shader;
}

void MyView::
windowViewWillStart(std::shared_ptr<tyga::Window> window)
{
    reloadShaders();


    std::vector<glm::vec2> vertices(4);
    vertices[0] = glm::vec2(-1, -1);
    vertices[1] = glm::vec2(1, -1);
    vertices[2] = glm::vec2(1, 1);
    vertices[3] = glm::vec2(-1, 1);
    glGenVertexArrays(1, &quad_vao_);
    glBindVertexArray(quad_vao_);
    glGenBuffers(1, &quad_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(glm::vec2),
                 vertices.data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(glm::vec2), TGL_BUFFER_OFFSET(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glGenFramebuffers(1, &cbuffer_fbo_);
    glGenRenderbuffers(1, &cbuffer_depth_rbo_);
    glGenTextures(1, &cbuffer_colour_tex_);
    glBindTexture(GL_TEXTURE_2D, cbuffer_colour_tex_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MyView::
reloadShaders()
{
    ShaderProgram* progs[] = { &draw_sp_, &filter_sp_ };
    std::string filenames[] = { "draw", "filter" };

    for (int i=0; i<2; ++i) {
        if (progs[i]->vertex_shader != 0) {
            glDeleteShader(progs[i]->vertex_shader);
        }
        if (progs[i]->fragment_shader != 0) {
            glDeleteShader(progs[i]->fragment_shader);
        }
        if (progs[i]->program != 0) {
            glDeleteProgram(progs[i]->program);
        }

        progs[i]->vertex_shader
            = compileShader(GL_VERTEX_SHADER, filenames[i] + "_vs.glsl");
        progs[i]->fragment_shader
            = compileShader(GL_FRAGMENT_SHADER, filenames[i] + "_fs.glsl");
 
        progs[i]->program = glCreateProgram();
        glAttachShader(progs[i]->program, progs[i]->vertex_shader);
        glBindAttribLocation(progs[i]->program, 0, "vertex_position");
        glAttachShader(progs[i]->program, progs[i]->fragment_shader);
        glBindFragDataLocation(progs[i]->program, 0, "fragment_colour");
        glLinkProgram(progs[i]->program);

        GLint link_status = 0;
        glGetProgramiv(progs[i]->program, GL_LINK_STATUS, &link_status);
        if (link_status != GL_TRUE) {
            const int string_length = 1024;
            GLchar log[string_length]= "";
            glGetProgramInfoLog(progs[i]->program, string_length, NULL, log);
            std::cerr << log << std::endl;
        }
    }
}

void MyView::
windowViewDidReset(std::shared_ptr<tyga::Window> window,
                   int width,
                   int height)
{
    glViewport(0, 0, width, height);

    glBindRenderbuffer(GL_RENDERBUFFER, cbuffer_depth_rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, cbuffer_colour_tex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,
                 width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, cbuffer_fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, cbuffer_colour_tex_, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, cbuffer_depth_rbo_);

    GLuint framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
        tglDebugMessage(GL_DEBUG_SEVERITY_HIGH, "cbuffer not complete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MyView::
windowViewDidStop(std::shared_ptr<tyga::Window> window)
{
    glDeleteShader(draw_sp_.vertex_shader);
    glDeleteShader(draw_sp_.fragment_shader);
    glDeleteProgram(draw_sp_.program);

    glDeleteShader(filter_sp_.vertex_shader);
    glDeleteShader(filter_sp_.fragment_shader);
    glDeleteProgram(filter_sp_.program);

    glDeleteBuffers(1, &quad_vbo_);
    glDeleteVertexArrays(1, &quad_vao_);

    glDeleteFramebuffers(1, &cbuffer_fbo_);
    glDeleteTextures(1, &cbuffer_colour_tex_);
    glDeleteRenderbuffers(1, &cbuffer_depth_rbo_);
}

void MyView::
windowViewRender(std::shared_ptr<tyga::Window> window)
{
    GLint link_status = 0;
    glGetProgramiv(draw_sp_.program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) {
        glClearColor(0.25f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        return;
    }


    GLint viewport_size[4];
    glGetIntegerv(GL_VIEWPORT, viewport_size);
    const float aspect_ratio = viewport_size[2] / (float)viewport_size[3];


    glBindFramebuffer(GL_FRAMEBUFFER, cbuffer_fbo_);

    glClearColor(0.f, 0.f, 0.25f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(draw_sp_.program);

    glm::mat4 projection_xform
        = glm::perspective(60.f, aspect_ratio, 0.1f, 100.f);
    glm::mat4 view_xform =
        glm::translate(glm::mat4(1), glm::vec3(0, 0, -10))
        * glm::rotate(glm::mat4(1), -view_inclination_, glm::vec3(1, 0, 0))
        * glm::rotate(glm::mat4(1), view_azimuth_, glm::vec3(0, 0, 1));
    glm::mat4 projection_view_xform = projection_xform * view_xform;
    glm::mat4 model_xform = glm::scale(glm::mat4(1), glm::vec3(10, 10, 10));
    glm::mat4 projection_view_model_xform = projection_view_xform * model_xform;

    glUniformMatrix4fv(
        glGetUniformLocation(draw_sp_.program, "projection_view_model_xform"),
        1, GL_FALSE, glm::value_ptr(projection_view_model_xform));

    glBindVertexArray(quad_vao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    link_status = 0;
    glGetProgramiv(filter_sp_.program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) {
        glClearColor(0.25f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        return;
    }

    glUseProgram(filter_sp_.program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cbuffer_colour_tex_);
    glGenerateMipmap(GL_TEXTURE_2D);
    glUniform1i(glGetUniformLocation(filter_sp_.program, "sampler_pixel"), 0);

    glBindVertexArray(quad_vao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
    glBindVertexArray(0);
}
