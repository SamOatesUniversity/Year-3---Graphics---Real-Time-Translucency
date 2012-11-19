#pragma once

#include "WindowViewDelegate.hpp"
#include "tgl.h"
#include "Shader.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <map>

class MyScene;

struct GBufferTexture {
	enum {
		position,
		normal,
		noof
	};
};

class MyView : public tyga::WindowViewDelegate
{
public:
	
    MyView();
	
    ~MyView();

    void
    setScene(std::shared_ptr<const MyScene> scene);

												//! Reload the programs shaders
	void										reloadShaders();

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

												//! Create the GBuffer
	void										CreateGBuffer(
													int windowWidth,
													int windowHeight
												);

private:

	struct {
		GLuint frameBuffer;
		GLuint texture[GBufferTexture::noof];
		GLuint depth;
	} m_gbuffer;


	struct {
		GLuint frameBuffer;
		GLuint colorBuffer;
		GLuint depth;
	} m_lbuffer;

    std::shared_ptr<const MyScene> scene_;

	std::map<char*, Shader*>					m_shader;				//!< 
	std::vector<Mesh>							m_meshes;				//!<
	Mesh										m_meshQuad;				//!< 
};
