#pragma once

#include "WindowViewDelegate.hpp"
#include "tgl.h"
#include <glm/glm.hpp>
#include <chrono>

/*
 * Tutorial: This class exists to integrate with the tyga framework. It
 *           implements the tyga::WindowViewDelegate interface which the
 *           tyga::Window object (which initialises OpenGL) uses to perform
 *           all drawing within the operating system window.
 */

class MyView : public tyga::WindowViewDelegate
{
public:
	
    MyView();
	
    ~MyView();

    /*
     * Tutorial: This method is present so that the controller
     *           (google model-view-controller pattern) can order this view
     *           object to reload its shaders at runtime.
     */

    void
    reloadShaders();

private:

    /*
     * Tutorial: These methods are needed to implement the framework's
     *           tyga::WindowViewDelegate interface.
     */

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

    /*
     * Tutorial: OpenGL objects (e.g. shaders or data buffers) are referenced
     *           via integer ids. We store the ids so we can use the objects
     *           each frame when drawing.
     */

    /*
     * Tutorial: Encapsulate the shader and program objects into a data
     *           structure to make it easy to have many of them in the future.
     */

    struct ShaderProgram
    {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;

        ShaderProgram() : vertex_shader(0),
                          fragment_shader(0),
                          program(0) {}
    };
    ShaderProgram first_;

    /*
     * Tutorial: Encapsulate the vertex array and buffer objects into a data
     *           structure to make it eash to have many of them in the future.
     */

    struct Mesh
    {
        GLuint vertex_vbo; // VertexBufferObject for the shape's per-vertex data
        GLuint element_vbo; // VertexBufferObject for the shape's element data
        GLuint vao; // VertexArrayObject for the shape's vertex array settings
        int element_count; // Needed for when we draw using the vertex arrays

        Mesh() : vertex_vbo(0),
                 element_vbo(0),
                 vao(0),
                 element_count(0) {}
    };
    Mesh mesh_;

    /*
     * Tutorial: Describe the per-vertex data using a structure to make it easy
     *           to add/remove attributes later without breaking working code.
     */

    struct Vertex
    {
        glm::vec3 position;
    };

};
