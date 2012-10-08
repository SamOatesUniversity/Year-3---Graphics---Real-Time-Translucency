#include "MyView.hpp"
#include "FileHelper.hpp"
#include "TcfScene.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

MyView::
MyView()
{
    scene_.reset(new TcfScene());
}

MyView::
~MyView() {
}

void MyView::
windowViewWillStart(std::shared_ptr<tyga::Window> window)
{
    start_time_ = std::chrono::system_clock::now();

    reloadShaders();

    if (!scene_->readFile("sponza.tcf")) {
        std::cerr << "Failed to read sponza.tcf data file" << std::endl;
    }

    /*
     * Tutorial exercise: Create vertex buffers to hold the Sponza geometry.
     */

    glEnable(GL_DEPTH_TEST);
}

void MyView::
reloadShaders()
{
    if (shading_.vertex_shader != 0) {
        glDeleteShader(shading_.vertex_shader);
    }
    if (shading_.fragment_shader != 0) {
        glDeleteShader(shading_.fragment_shader);
    }
    if (shading_.program != 0) {
        glDeleteProgram(shading_.program);
    }

    GLint compile_status = 0;
    
    shading_.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertex_shader_string = tyga::stringFromFile("reprise_vs.glsl");
    const char *vertex_shader_code = vertex_shader_string.c_str();
    glShaderSource(shading_.vertex_shader, 1,
                   (const GLchar **) &vertex_shader_code, NULL);
    glCompileShader(shading_.vertex_shader);
    glGetShaderiv(shading_.vertex_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        const int string_length = 1024;
        GLchar log[string_length]= "";
        glGetShaderInfoLog(shading_.vertex_shader, string_length, NULL, log);
        std::cerr << log << std::endl;
    }
 
    shading_.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragment_shader_string = tyga::stringFromFile("reprise_fs.glsl");
    const char *fragment_shader_code = fragment_shader_string.c_str();
    glShaderSource(shading_.fragment_shader, 1,
                   (const GLchar **) &fragment_shader_code, NULL);
    glCompileShader(shading_.fragment_shader);
    glGetShaderiv(shading_.fragment_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        const int string_length = 1024;
        GLchar log[string_length]= "";
        glGetShaderInfoLog(shading_.fragment_shader, string_length, NULL, log);
        std::cerr << log << std::endl;
    }
 
    shading_.program = glCreateProgram();
    glAttachShader(shading_.program, shading_.vertex_shader);
    glAttachShader(shading_.program, shading_.fragment_shader);

    /*
     * Tutorial exercise: Bind vertex input attributes and fragment output
     *                    attributes in your shaders before linking
     */

    glLinkProgram(shading_.program);
}

void MyView::
windowViewDidReset(std::shared_ptr<tyga::Window> window,
                   int width,
                   int height)
{
    glViewport(0, 0, width, height);
}

void MyView::
windowViewDidStop(std::shared_ptr<tyga::Window> window)
{
    if (shading_.vertex_shader != 0) {
        glDeleteShader(shading_.vertex_shader);
    }
    if (shading_.fragment_shader != 0) {
        glDeleteShader(shading_.fragment_shader);
    }
    if (shading_.program != 0) {
        glDeleteProgram(shading_.program);
    }

    /*
     * Tutorial exercise: delete any GL resources you've made
     */
}

void MyView::
windowViewRender(std::shared_ptr<tyga::Window> window)
{
    glClearColor(0.f, 0.f, 0.25f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint link_status;
    glGetProgramiv(shading_.program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) {
        return;
    }

    glUseProgram(shading_.program);

    GLint viewport_size[4];
    glGetIntegerv(GL_VIEWPORT, viewport_size);
    const float aspect_ratio = viewport_size[2] / (float)viewport_size[3];

    const auto clock_time = std::chrono::system_clock::now() - start_time_;
    const auto clock_millisecs
        = std::chrono::duration_cast<std::chrono::milliseconds>(clock_time);
    const float time_seconds = 0.001f * clock_millisecs.count();

    /*
     * Tutorial exercise: construct projection, view and model matrices and
     *                    send them to your shaders.  Update other uniforms
     *                    as well.
     */

    /*
     * Tutorial exercise: draw each model in the Sponza scene (accessed using
     *                    the scene_ member variable.
     */
}
