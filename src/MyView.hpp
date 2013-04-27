#pragma once

#include <profy.h>

#include "WindowViewDelegate.hpp"
#include "MyScene.hpp"
#include "tgl.h"
#include "Shader.h"
#include "Mesh.h"
#include "Light.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <map>

#include <AntTweakBar.h>
//#include <vld.h>

class MyScene;

struct GBufferTexture {
	enum {
		position,
		normal,
		material,
		noof
	};
};

struct TranslucencyTexture {
	enum {
		depth,
		irradiance,
		surfacenormal,
		xin,
		noof
	};
};

struct GFXCardType {
	enum Enum {
		ATI,
		NVIDIA,
		UNKNOWN
	};
};

struct RenderFlags
{
	bool enableShadows;
	bool enableShadowPCF;
	bool respectShadowFlag;
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

												//! 
	void										ShowProfilingPieChart(
													bool open										//!< 
												) const;

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

												//! Create the LBuffer
	void										CreateLBuffer(
													int windowWidth,
													int windowHeight
												);

												//! Create the Shadow Buffer
	void										CreateShadowBuffer(
													int windowWidth,
													int windowHeight
												);

												//! Render the scene to the GBuffer
	void										RenderGBuffer(
													const MyScene::Camera &camera,					//!< The camera of which we want to render from
													const float &aspect_ratio						//!< The aspect ratio of the window
												);

												//! Render the LBuffer from the GBuffer data
	void										RenderLBuffer(
													const MyScene::Camera &camera,					//!< The camera of which we want to render from
													const float &aspect_ratio						//!< The aspect ratio of the window
												);

												//! Perform post processing on the lbuffer
	void										PerformPostProcessing();

												//! Draw the scenes directional light to the lbuffer
	void										DrawDirectionalLight(
													const MyScene::Camera &camera					//!< The camera of which we want to render from
												);

												//! Draw the scenes spot lights to the lbuffer
	void										DrawSpotLights(
													const MyScene::Camera &camera,					//!< The camera of which we want to render from
													const float &aspect_ratio						//!< The aspect ratio of the window
												);

												//! Bind all the gbuffer textures to a shader sampler
	void										BindGBufferTextures(
													const Shader *const shader						//!< A pointer to the shader of the samplers to bind
												);
	
												//! 
	void										CreateTweakBar();

private:

	RenderFlags									m_flags;

	struct {
		GLuint frameBuffer;
		GLuint texture[GBufferTexture::noof];
		GLuint depth;
	} m_gbuffer;


	struct {
		GLuint frameBuffer;
		GLuint texture;
		GLuint depth;
		glm::vec2 size;
	} m_lbuffer;

	struct {
		GLuint frameBuffer;
		GLuint texture[TranslucencyTexture::noof];
		GLuint depth;
		glm::vec2 size;
	} m_shadowbuffer;

	GFXCardType::Enum							m_gfxCard;

    std::shared_ptr<const MyScene> scene_;

	std::map<char*, Shader*>					m_shader;				//!< 
	std::vector<Mesh>							m_meshes;				//!<
	Mesh										m_meshQuad;				//!< 
	Mesh										m_sphereMesh;			//!< The mesh that represents a point lights volume
	Mesh										m_coneMesh;				//!< The mesh that represents a cone lights volume

	std::vector<Light*>							m_light;

	struct {
		GLuint diffuse;
		GLuint normal;
	} m_brickTexture;


	struct Timer {
		enum Enum {
			ShaderLoadtime,
			GBufferCreation,
			LBuffer_DirectionalLight,
			LBuffer_SpotLightFirstPass,
			LBuffer_SpotLightSecondPass,
			PostProcessing,
			Noof
		};
	};
	ProFy::TimerID								m_timer[Timer::Noof];

	struct {
		TwBar *bar;
	}											m_lightbar;

	struct {
		TwBar *bar;
		int fps;
		struct {
			float wholeFrame;
			float gbufferCreation;
			float lbufferdirectional;
			float lbufferspotp1;
			float lbufferspotp2;
			float postProcessing;
		} timer;
	}											m_debugbar;

	enum eTranslucencyMode
	{
		TSM,
		SOApprocimation
	};

	struct {
		TwBar *bar;
		eTranslucencyMode translucencyMode;
	}											m_renderbar;

};
