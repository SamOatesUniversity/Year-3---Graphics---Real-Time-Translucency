/**
 * @file    tgl.c
 * @author  Tyrone Davison
 * @date    September 2012
 */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "tgl.h"

/* GL_version_1_0 */
#ifdef TGL_PROTOTYPES_GL_1_0
PFNGLCULLFACEPROC glCullFace = 0;
PFNGLFRONTFACEPROC glFrontFace = 0;
PFNGLHINTPROC glHint = 0;
PFNGLLINEWIDTHPROC glLineWidth = 0;
PFNGLPOINTSIZEPROC glPointSize = 0;
PFNGLPOLYGONMODEPROC glPolygonMode = 0;
PFNGLSCISSORPROC glScissor = 0;
PFNGLTEXPARAMETERFPROC glTexParameterf = 0;
PFNGLTEXPARAMETERFVPROC glTexParameterfv = 0;
PFNGLTEXPARAMETERIPROC glTexParameteri = 0;
PFNGLTEXPARAMETERIVPROC glTexParameteriv = 0;
PFNGLTEXIMAGE1DPROC glTexImage1D = 0;
PFNGLTEXIMAGE2DPROC glTexImage2D = 0;
PFNGLDRAWBUFFERPROC glDrawBuffer = 0;
PFNGLCLEARPROC glClear = 0;
PFNGLCLEARCOLORPROC glClearColor = 0;
PFNGLCLEARSTENCILPROC glClearStencil = 0;
PFNGLCLEARDEPTHPROC glClearDepth = 0;
PFNGLSTENCILMASKPROC glStencilMask = 0;
PFNGLCOLORMASKPROC glColorMask = 0;
PFNGLDEPTHMASKPROC glDepthMask = 0;
PFNGLDISABLEPROC glDisable = 0;
PFNGLENABLEPROC glEnable = 0;
PFNGLFINISHPROC glFinish = 0;
PFNGLFLUSHPROC glFlush = 0;
PFNGLBLENDFUNCPROC glBlendFunc = 0;
PFNGLLOGICOPPROC glLogicOp = 0;
PFNGLSTENCILFUNCPROC glStencilFunc = 0;
PFNGLSTENCILOPPROC glStencilOp = 0;
PFNGLDEPTHFUNCPROC glDepthFunc = 0;
PFNGLPIXELSTOREFPROC glPixelStoref = 0;
PFNGLPIXELSTOREIPROC glPixelStorei = 0;
PFNGLREADBUFFERPROC glReadBuffer = 0;
PFNGLREADPIXELSPROC glReadPixels = 0;
PFNGLGETBOOLEANVPROC glGetBooleanv = 0;
PFNGLGETDOUBLEVPROC glGetDoublev = 0;
PFNGLGETERRORPROC glGetError = 0;
PFNGLGETFLOATVPROC glGetFloatv = 0;
PFNGLGETINTEGERVPROC glGetIntegerv = 0;
PFNGLGETSTRINGPROC glGetString = 0;
PFNGLGETTEXIMAGEPROC glGetTexImage = 0;
PFNGLGETTEXPARAMETERFVPROC glGetTexParameterfv = 0;
PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv = 0;
PFNGLGETTEXLEVELPARAMETERFVPROC glGetTexLevelParameterfv = 0;
PFNGLGETTEXLEVELPARAMETERIVPROC glGetTexLevelParameteriv = 0;
PFNGLISENABLEDPROC glIsEnabled = 0;
PFNGLDEPTHRANGEPROC glDepthRange = 0;
PFNGLVIEWPORTPROC glViewport = 0;
#endif
/* GL_version_1_1 */
#ifdef TGL_PROTOTYPES_GL_1_1
PFNGLDRAWARRAYSPROC glDrawArrays = 0;
PFNGLDRAWELEMENTSPROC glDrawElements = 0;
PFNGLGETPOINTERVPROC glGetPointerv = 0;
PFNGLPOLYGONOFFSETPROC glPolygonOffset = 0;
PFNGLCOPYTEXIMAGE1DPROC glCopyTexImage1D = 0;
PFNGLCOPYTEXIMAGE2DPROC glCopyTexImage2D = 0;
PFNGLCOPYTEXSUBIMAGE1DPROC glCopyTexSubImage1D = 0;
PFNGLCOPYTEXSUBIMAGE2DPROC glCopyTexSubImage2D = 0;
PFNGLTEXSUBIMAGE1DPROC glTexSubImage1D = 0;
PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D = 0;
PFNGLBINDTEXTUREPROC glBindTexture = 0;
PFNGLDELETETEXTURESPROC glDeleteTextures = 0;
PFNGLGENTEXTURESPROC glGenTextures = 0;
PFNGLISTEXTUREPROC glIsTexture = 0;
#endif
/* GL_version_1_2 */
PFNGLBLENDCOLORPROC glBlendColor = 0;
PFNGLBLENDEQUATIONPROC glBlendEquation = 0;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = 0;
PFNGLTEXIMAGE3DPROC glTexImage3D = 0;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D = 0;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D = 0;
/* GL_version_1_3 */
PFNGLACTIVETEXTUREPROC glActiveTexture = 0;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage = 0;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D = 0;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = 0;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D = 0;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage = 0;
/* GL_version_1_4 */
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = 0;
PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays = 0;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements = 0;
PFNGLPOINTPARAMETERFPROC glPointParameterf = 0;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv = 0;
PFNGLPOINTPARAMETERIPROC glPointParameteri = 0;
PFNGLPOINTPARAMETERIVPROC glPointParameteriv = 0;
/* GL_version_1_5 */
PFNGLGENQUERIESPROC glGenQueries = 0;
PFNGLDELETEQUERIESPROC glDeleteQueries = 0;
PFNGLISQUERYPROC glIsQuery = 0;
PFNGLBEGINQUERYPROC glBeginQuery = 0;
PFNGLENDQUERYPROC glEndQuery = 0;
PFNGLGETQUERYIVPROC glGetQueryiv = 0;
PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv = 0;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv = 0;
PFNGLBINDBUFFERPROC glBindBuffer = 0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;
PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLISBUFFERPROC glIsBuffer = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLBUFFERSUBDATAPROC glBufferSubData = 0;
PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData = 0;
PFNGLMAPBUFFERPROC glMapBuffer = 0;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = 0;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv = 0;
PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv = 0;
/* GL_version_2_0 */
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = 0;
PFNGLDRAWBUFFERSPROC glDrawBuffers = 0;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate = 0;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate = 0;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate = 0;
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
PFNGLDELETESHADERPROC glDeleteShader = 0;
PFNGLDETACHSHADERPROC glDetachShader = 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = 0;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = 0;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = 0;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
PFNGLGETSHADERSOURCEPROC glGetShaderSource = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLGETUNIFORMFVPROC glGetUniformfv = 0;
PFNGLGETUNIFORMIVPROC glGetUniformiv = 0;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv = 0;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv = 0;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv = 0;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = 0;
PFNGLISPROGRAMPROC glIsProgram = 0;
PFNGLISSHADERPROC glIsShader = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLUNIFORM1FPROC glUniform1f = 0;
PFNGLUNIFORM2FPROC glUniform2f = 0;
PFNGLUNIFORM3FPROC glUniform3f = 0;
PFNGLUNIFORM4FPROC glUniform4f = 0;
PFNGLUNIFORM1IPROC glUniform1i = 0;
PFNGLUNIFORM2IPROC glUniform2i = 0;
PFNGLUNIFORM3IPROC glUniform3i = 0;
PFNGLUNIFORM4IPROC glUniform4i = 0;
PFNGLUNIFORM1FVPROC glUniform1fv = 0;
PFNGLUNIFORM2FVPROC glUniform2fv = 0;
PFNGLUNIFORM3FVPROC glUniform3fv = 0;
PFNGLUNIFORM4FVPROC glUniform4fv = 0;
PFNGLUNIFORM1IVPROC glUniform1iv = 0;
PFNGLUNIFORM2IVPROC glUniform2iv = 0;
PFNGLUNIFORM3IVPROC glUniform3iv = 0;
PFNGLUNIFORM4IVPROC glUniform4iv = 0;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = 0;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = 0;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = 0;
PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d = 0;
PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv = 0;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = 0;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv = 0;
PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s = 0;
PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv = 0;
PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d = 0;
PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv = 0;
PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f = 0;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv = 0;
PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s = 0;
PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv = 0;
PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d = 0;
PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv = 0;
PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f = 0;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv = 0;
PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s = 0;
PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv = 0;
PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv = 0;
PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv = 0;
PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv = 0;
PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub = 0;
PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv = 0;
PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv = 0;
PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv = 0;
PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv = 0;
PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d = 0;
PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv = 0;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f = 0;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = 0;
PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv = 0;
PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s = 0;
PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv = 0;
PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv = 0;
PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv = 0;
PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
/* GL_version_2_1 */
PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv = 0;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv = 0;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv = 0;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv = 0;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv = 0;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv = 0;
/* GL_version_3_0 */
PFNGLCOLORMASKIPROC glColorMaski = 0;
PFNGLGETBOOLEANI_VPROC glGetBooleani_v = 0;
PFNGLGETINTEGERI_VPROC glGetIntegeri_v = 0;
PFNGLENABLEIPROC glEnablei = 0;
PFNGLDISABLEIPROC glDisablei = 0;
PFNGLISENABLEDIPROC glIsEnabledi = 0;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback = 0;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback = 0;
PFNGLBINDBUFFERRANGEPROC glBindBufferRange = 0;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = 0;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings = 0;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying = 0;
PFNGLCLAMPCOLORPROC glClampColor = 0;
PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender = 0;
PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender = 0;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = 0;
PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv = 0;
PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv = 0;
PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i = 0;
PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i = 0;
PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i = 0;
PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i = 0;
PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui = 0;
PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui = 0;
PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui = 0;
PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui = 0;
PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv = 0;
PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv = 0;
PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv = 0;
PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv = 0;
PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv = 0;
PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv = 0;
PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv = 0;
PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv = 0;
PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv = 0;
PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv = 0;
PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv = 0;
PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv = 0;
PFNGLGETUNIFORMUIVPROC glGetUniformuiv = 0;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation = 0;
PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation = 0;
PFNGLUNIFORM1UIPROC glUniform1ui = 0;
PFNGLUNIFORM2UIPROC glUniform2ui = 0;
PFNGLUNIFORM3UIPROC glUniform3ui = 0;
PFNGLUNIFORM4UIPROC glUniform4ui = 0;
PFNGLUNIFORM1UIVPROC glUniform1uiv = 0;
PFNGLUNIFORM2UIVPROC glUniform2uiv = 0;
PFNGLUNIFORM3UIVPROC glUniform3uiv = 0;
PFNGLUNIFORM4UIVPROC glUniform4uiv = 0;
PFNGLTEXPARAMETERIIVPROC glTexParameterIiv = 0;
PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv = 0;
PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv = 0;
PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv = 0;
PFNGLCLEARBUFFERIVPROC glClearBufferiv = 0;
PFNGLCLEARBUFFERUIVPROC glClearBufferuiv = 0;
PFNGLCLEARBUFFERFVPROC glClearBufferfv = 0;
PFNGLCLEARBUFFERFIPROC glClearBufferfi = 0;
PFNGLGETSTRINGIPROC glGetStringi = 0;
/* ARB_framebuffer_object */
PFNGLISRENDERBUFFERPROC glIsRenderbuffer = 0;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = 0;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = 0;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = 0;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = 0;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv = 0;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer = 0;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = 0;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = 0;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = 0;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = 0;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D = 0;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = 0;
PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D = 0;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = 0;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv = 0;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = 0;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = 0;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = 0;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer = 0;
/* ARB_map_buffer_range */
PFNGLMAPBUFFERRANGEPROC glMapBufferRange = 0;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange = 0;
/* ARB_vertex_array_object */
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
PFNGLISVERTEXARRAYPROC glIsVertexArray = 0;
/* GL_version_3_1 */
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = 0;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced = 0;
PFNGLTEXBUFFERPROC glTexBuffer = 0;
PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex = 0;
/* ARB_copy_buffer */
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = 0;
/* ARB_uniform_buffer_object */
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices = 0;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv = 0;
PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName = 0;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = 0;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv = 0;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName = 0;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = 0;
/* GL_version_3_2 */
PFNGLGETINTEGER64I_VPROC glGetInteger64i_v = 0;
PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v = 0;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture = 0;
/* ARB_draw_elements_base_vertex */
PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = 0;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex = 0;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex = 0;
/* ARB_provoking_vertex */
PFNGLPROVOKINGVERTEXPROC glProvokingVertex = 0;
/* ARB_sync */
PFNGLFENCESYNCPROC glFenceSync = 0;
PFNGLISSYNCPROC glIsSync = 0;
PFNGLDELETESYNCPROC glDeleteSync = 0;
PFNGLCLIENTWAITSYNCPROC glClientWaitSync = 0;
PFNGLWAITSYNCPROC glWaitSync = 0;
PFNGLGETINTEGER64VPROC glGetInteger64v = 0;
PFNGLGETSYNCIVPROC glGetSynciv = 0;
/* ARB_texture_multisample */
PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample = 0;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample = 0;
PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv = 0;
PFNGLSAMPLEMASKIPROC glSampleMaski = 0;
/* GL_version_3_3 */
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor = 0;
/* ARB_blend_func_extended */
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed = 0;
PFNGLGETFRAGDATAINDEXPROC glGetFragDataIndex = 0;
/* ARB_sampler_objects */
PFNGLGENSAMPLERSPROC glGenSamplers = 0;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = 0;
PFNGLISSAMPLERPROC glIsSampler = 0;
PFNGLBINDSAMPLERPROC glBindSampler = 0;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = 0;
PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv = 0;
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf = 0;
PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv = 0;
PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv = 0;
PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv = 0;
PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv = 0;
PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv = 0;
PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv = 0;
PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv = 0;
/* ARB_timer_query */
PFNGLQUERYCOUNTERPROC glQueryCounter = 0;
PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64v = 0;
PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v = 0;
/* ARB_vertex_type_2_10_10_10_rev */
PFNGLVERTEXP2UIPROC glVertexP2ui = 0;
PFNGLVERTEXP2UIVPROC glVertexP2uiv = 0;
PFNGLVERTEXP3UIPROC glVertexP3ui = 0;
PFNGLVERTEXP3UIVPROC glVertexP3uiv = 0;
PFNGLVERTEXP4UIPROC glVertexP4ui = 0;
PFNGLVERTEXP4UIVPROC glVertexP4uiv = 0;
PFNGLTEXCOORDP1UIPROC glTexCoordP1ui = 0;
PFNGLTEXCOORDP1UIVPROC glTexCoordP1uiv = 0;
PFNGLTEXCOORDP2UIPROC glTexCoordP2ui = 0;
PFNGLTEXCOORDP2UIVPROC glTexCoordP2uiv = 0;
PFNGLTEXCOORDP3UIPROC glTexCoordP3ui = 0;
PFNGLTEXCOORDP3UIVPROC glTexCoordP3uiv = 0;
PFNGLTEXCOORDP4UIPROC glTexCoordP4ui = 0;
PFNGLTEXCOORDP4UIVPROC glTexCoordP4uiv = 0;
PFNGLMULTITEXCOORDP1UIPROC glMultiTexCoordP1ui = 0;
PFNGLMULTITEXCOORDP1UIVPROC glMultiTexCoordP1uiv = 0;
PFNGLMULTITEXCOORDP2UIPROC glMultiTexCoordP2ui = 0;
PFNGLMULTITEXCOORDP2UIVPROC glMultiTexCoordP2uiv = 0;
PFNGLMULTITEXCOORDP3UIPROC glMultiTexCoordP3ui = 0;
PFNGLMULTITEXCOORDP3UIVPROC glMultiTexCoordP3uiv = 0;
PFNGLMULTITEXCOORDP4UIPROC glMultiTexCoordP4ui = 0;
PFNGLMULTITEXCOORDP4UIVPROC glMultiTexCoordP4uiv = 0;
PFNGLNORMALP3UIPROC glNormalP3ui = 0;
PFNGLNORMALP3UIVPROC glNormalP3uiv = 0;
PFNGLCOLORP3UIPROC glColorP3ui = 0;
PFNGLCOLORP3UIVPROC glColorP3uiv = 0;
PFNGLCOLORP4UIPROC glColorP4ui = 0;
PFNGLCOLORP4UIVPROC glColorP4uiv = 0;
PFNGLSECONDARYCOLORP3UIPROC glSecondaryColorP3ui = 0;
PFNGLSECONDARYCOLORP3UIVPROC glSecondaryColorP3uiv = 0;
PFNGLVERTEXATTRIBP1UIPROC glVertexAttribP1ui = 0;
PFNGLVERTEXATTRIBP1UIVPROC glVertexAttribP1uiv = 0;
PFNGLVERTEXATTRIBP2UIPROC glVertexAttribP2ui = 0;
PFNGLVERTEXATTRIBP2UIVPROC glVertexAttribP2uiv = 0;
PFNGLVERTEXATTRIBP3UIPROC glVertexAttribP3ui = 0;
PFNGLVERTEXATTRIBP3UIVPROC glVertexAttribP3uiv = 0;
PFNGLVERTEXATTRIBP4UIPROC glVertexAttribP4ui = 0;
PFNGLVERTEXATTRIBP4UIVPROC glVertexAttribP4uiv = 0;
/* ARB_debug_output */
PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB = 0;
PFNGLDEBUGMESSAGEINSERTARBPROC glDebugMessageInsertARB = 0;
PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB = 0;
PFNGLGETDEBUGMESSAGELOGARBPROC glGetDebugMessageLogARB = 0;
/* AMD_debug_output */
PFNGLDEBUGMESSAGEENABLEAMDPROC glDebugMessageEnableAMD = 0;
PFNGLDEBUGMESSAGEINSERTAMDPROC glDebugMessageInsertAMD = 0;
PFNGLDEBUGMESSAGECALLBACKAMDPROC glDebugMessageCallbackAMD = 0;
PFNGLGETDEBUGMESSAGELOGAMDPROC glGetDebugMessageLogAMD = 0;

/* success variables */
static GLboolean tgl_extensions[TGL_EXTENSION_MAX];

#define LOADFUNC( type, name, ret ) \
	name = (type)wglGetProcAddress(#name); \
	if (name == NULL) {\
		OutputDebugString("TGL ... failed to load procedure ");\
		OutputDebugString(#name);\
		OutputDebugString("\n");\
		ret = GL_FALSE;\
	}

/* callback to display GL debug messages */
void _stdcall _tglDebugLog(GLenum source,
                           GLenum type,
                           GLuint id,
                           GLenum severity,
                           GLsizei length,
                           const char* message,
                           void *userParam)
{
    OutputDebugStringA("GL debug (source=");
    switch (source) {
    case GL_DEBUG_SOURCE_API_ARB:
        OutputDebugStringA("api");
        break;
    case GL_DEBUG_SOURCE_APPLICATION_ARB:
        OutputDebugStringA("application");
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
        OutputDebugStringA("shader compiler");
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
        OutputDebugStringA("3rd party");
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
        OutputDebugStringA("window system");
        break;
    case GL_DEBUG_SOURCE_OTHER_ARB:
    default:
        OutputDebugStringA("other");
        break;
    }
    OutputDebugStringA(", type=");
    switch (type) {
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        OutputDebugStringA("depreciated behaviour");
        break;
    case GL_DEBUG_TYPE_ERROR_ARB:
        OutputDebugStringA("error");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        OutputDebugStringA("performance");
        break;
    case GL_DEBUG_TYPE_PORTABILITY_ARB:
        OutputDebugStringA("portability");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        OutputDebugStringA("undefined behaviour");
        break;
    case GL_DEBUG_TYPE_OTHER_ARB:
    default:
        OutputDebugStringA("other");
        break;
    }
    OutputDebugStringA(", severity=");
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH_ARB:
        OutputDebugStringA("high");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB:
        OutputDebugStringA("medium");
        break;
    case GL_DEBUG_SEVERITY_LOW_ARB:
        OutputDebugStringA("low");
        break;
    }
    OutputDebugStringA("): ");
    OutputDebugStringA(message);
    OutputDebugStringA("\n");
}
/* AMD version of the debug callback */
void _stdcall _tglDebugLogAMD(GLuint id,
                              GLenum category,
                              GLenum severity,
                              GLsizei length,
                              const char* message,
                              void *userParam)
{
    OutputDebugStringA("GL debug (category=");
    switch (category) {
    case GL_DEBUG_CATEGORY_API_ERROR_AMD:
        OutputDebugStringA("api error");
        break;
    case GL_DEBUG_CATEGORY_APPLICATION_AMD:
        OutputDebugStringA("application");
        break;
    case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
        OutputDebugStringA("deprecation");
        break;
    case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:
        OutputDebugStringA("performance");
        break;
    case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
        OutputDebugStringA("shader compiler");
        break;
    case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
        OutputDebugStringA("undefined behaviour");
        break;
    case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
        OutputDebugStringA("window system");
        break;
    case GL_DEBUG_CATEGORY_OTHER_AMD:
    default:
        OutputDebugStringA("other");
        break;
    }
    OutputDebugStringA(", severity=");
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH_AMD:
        OutputDebugStringA("high");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM_AMD:
        OutputDebugStringA("medium");
        break;
    case GL_DEBUG_SEVERITY_LOW_AMD:
        OutputDebugStringA("low");
        break;
    }
    OutputDebugStringA("): ");
    OutputDebugStringA(message);
    OutputDebugStringA("\n");
}

void tglInit(void) {
    /* assume all extensions will load successfully */
    int i;
    for (i=0; i<TGL_EXTENSION_MAX; ++i) {
        tgl_extensions[i] = GL_TRUE;
    }
#ifdef _DEBUG
    /* print driver info */
    OutputDebugStringA("\nGL vendor: ");
    OutputDebugStringA((const char*)glGetString(GL_VENDOR));
    OutputDebugStringA("\nGL renderer: ");
    OutputDebugStringA((const char*)glGetString(GL_RENDERER));
    OutputDebugStringA("\nGL version: ");
    OutputDebugStringA((const char*)glGetString(GL_VERSION));
    OutputDebugStringA("\nGLSL version: ");
    OutputDebugStringA((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    OutputDebugStringA("\n\n");
#endif
    /* GL_version_1_0 */
#ifdef TGL_PROTOTYPES_GL_1_0
    LOADFUNC(PFNGLCULLFACEPROC, glCullFace, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLFRONTFACEPROC, glFrontFace, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLHINTPROC, glHint, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLLINEWIDTHPROC, glLineWidth, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLPOINTSIZEPROC, glPointSize, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLPOLYGONMODEPROC, glPolygonMode, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLSCISSORPROC, glScissor, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLTEXPARAMETERFPROC, glTexParameterf, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLTEXPARAMETERFVPROC, glTexParameterfv, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLTEXPARAMETERIPROC, glTexParameteri, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLTEXPARAMETERIVPROC, glTexParameteriv, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLTEXIMAGE1DPROC, glTexImage1D, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLTEXIMAGE2DPROC, glTexImage2D, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLDRAWBUFFERPROC, glDrawBuffer, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLCLEARPROC, glClear, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLCLEARCOLORPROC, glClearColor, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLCLEARSTENCILPROC, glClearStencil, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLCLEARDEPTHPROC, glClearDepth, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLSTENCILMASKPROC, glStencilMask, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLCOLORMASKPROC, glColorMask, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLDEPTHMASKPROC, glDepthMask, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLDISABLEPROC, glDisable, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLENABLEPROC, glEnable, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLFINISHPROC, glFinish, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLFLUSHPROC, glFlush, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLBLENDFUNCPROC, glBlendFunc, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLLOGICOPPROC, glLogicOp, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLSTENCILFUNCPROC, glStencilFunc, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLSTENCILOPPROC, glStencilOp, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLDEPTHFUNCPROC, glDepthFunc, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLPIXELSTOREFPROC, glPixelStoref, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLPIXELSTOREIPROC, glPixelStorei, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLREADBUFFERPROC, glReadBuffer, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLREADPIXELSPROC, glReadPixels, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETBOOLEANVPROC, glGetBooleanv, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETDOUBLEVPROC, glGetDoublev, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETERRORPROC, glGetError, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETFLOATVPROC, glGetFloatv, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETINTEGERVPROC, glGetIntegerv, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETSTRINGPROC, glGetString, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETTEXIMAGEPROC, glGetTexImage, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETTEXPARAMETERFVPROC, glGetTexParameterfv, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETTEXPARAMETERIVPROC, glGetTexParameteriv, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETTEXLEVELPARAMETERFVPROC, glGetTexLevelParameterfv, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLGETTEXLEVELPARAMETERIVPROC, glGetTexLevelParameteriv, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLISENABLEDPROC, glIsEnabled, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLDEPTHRANGEPROC, glDepthRange, tgl_extensions[TGL_EXTENSION_GL_1_0])
    LOADFUNC(PFNGLVIEWPORTPROC, glViewport, tgl_extensions[TGL_EXTENSION_GL_1_0])
#endif
    /* GL_version_1_1 */
#ifdef TGL_PROTOTYPES_GL_1_1
    LOADFUNC(PFNGLDRAWARRAYSPROC, glDrawArrays, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLDRAWELEMENTSPROC, glDrawElements, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLGETPOINTERVPROC, glGetPointerv, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLPOLYGONOFFSETPROC, glPolygonOffset, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLCOPYTEXIMAGE1DPROC, glCopyTexImage1D, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLCOPYTEXIMAGE2DPROC, glCopyTexImage2D, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLCOPYTEXSUBIMAGE1DPROC, glCopyTexSubImage1D, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLCOPYTEXSUBIMAGE2DPROC, glCopyTexSubImage2D, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLTEXSUBIMAGE1DPROC, glTexSubImage1D, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLTEXSUBIMAGE2DPROC, glTexSubImage2D, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLBINDTEXTUREPROC, glBindTexture, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLDELETETEXTURESPROC, glDeleteTextures, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLGENTEXTURESPROC, glGenTextures, tgl_extensions[TGL_EXTENSION_GL_1_1])
    LOADFUNC(PFNGLISTEXTUREPROC, glIsTexture, tgl_extensions[TGL_EXTENSION_GL_1_1])
#endif
    /* GL_version_1_2 */
    LOADFUNC(PFNGLBLENDCOLORPROC, glBlendColor, tgl_extensions[TGL_EXTENSION_GL_1_2])
    LOADFUNC(PFNGLBLENDEQUATIONPROC, glBlendEquation, tgl_extensions[TGL_EXTENSION_GL_1_2])
    LOADFUNC(PFNGLDRAWRANGEELEMENTSPROC, glDrawRangeElements, tgl_extensions[TGL_EXTENSION_GL_1_2])
    LOADFUNC(PFNGLTEXIMAGE3DPROC, glTexImage3D, tgl_extensions[TGL_EXTENSION_GL_1_2])
    LOADFUNC(PFNGLTEXSUBIMAGE3DPROC, glTexSubImage3D, tgl_extensions[TGL_EXTENSION_GL_1_2])
    LOADFUNC(PFNGLCOPYTEXSUBIMAGE3DPROC, glCopyTexSubImage3D, tgl_extensions[TGL_EXTENSION_GL_1_2])
    /* GL_version_1_3 */
    LOADFUNC(PFNGLACTIVETEXTUREPROC, glActiveTexture, tgl_extensions[TGL_EXTENSION_GL_1_3])
    LOADFUNC(PFNGLSAMPLECOVERAGEPROC, glSampleCoverage, tgl_extensions[TGL_EXTENSION_GL_1_3])
    LOADFUNC(PFNGLCOMPRESSEDTEXIMAGE3DPROC, glCompressedTexImage3D, tgl_extensions[TGL_EXTENSION_GL_1_3])
    LOADFUNC(PFNGLCOMPRESSEDTEXIMAGE2DPROC, glCompressedTexImage2D, tgl_extensions[TGL_EXTENSION_GL_1_3])
    LOADFUNC(PFNGLCOMPRESSEDTEXIMAGE1DPROC, glCompressedTexImage1D, tgl_extensions[TGL_EXTENSION_GL_1_3])
    LOADFUNC(PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC, glCompressedTexSubImage3D, tgl_extensions[TGL_EXTENSION_GL_1_3])
    LOADFUNC(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC, glCompressedTexSubImage2D, tgl_extensions[TGL_EXTENSION_GL_1_3])
    LOADFUNC(PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC, glCompressedTexSubImage1D, tgl_extensions[TGL_EXTENSION_GL_1_3])
    LOADFUNC(PFNGLGETCOMPRESSEDTEXIMAGEPROC, glGetCompressedTexImage, tgl_extensions[TGL_EXTENSION_GL_1_3])
    /* GL_version_1_4 */
    LOADFUNC(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate, tgl_extensions[TGL_EXTENSION_GL_1_4])
    LOADFUNC(PFNGLMULTIDRAWARRAYSPROC, glMultiDrawArrays, tgl_extensions[TGL_EXTENSION_GL_1_4])
    LOADFUNC(PFNGLMULTIDRAWELEMENTSPROC, glMultiDrawElements, tgl_extensions[TGL_EXTENSION_GL_1_4])
    LOADFUNC(PFNGLPOINTPARAMETERFPROC, glPointParameterf, tgl_extensions[TGL_EXTENSION_GL_1_4])
    LOADFUNC(PFNGLPOINTPARAMETERFVPROC, glPointParameterfv, tgl_extensions[TGL_EXTENSION_GL_1_4])
    LOADFUNC(PFNGLPOINTPARAMETERIPROC, glPointParameteri, tgl_extensions[TGL_EXTENSION_GL_1_4])
    LOADFUNC(PFNGLPOINTPARAMETERIVPROC, glPointParameteriv, tgl_extensions[TGL_EXTENSION_GL_1_4])
    /* GL_version_1_5 */
    LOADFUNC(PFNGLGENQUERIESPROC, glGenQueries, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLDELETEQUERIESPROC, glDeleteQueries, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLISQUERYPROC, glIsQuery, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLBEGINQUERYPROC, glBeginQuery, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLENDQUERYPROC, glEndQuery, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLGETQUERYIVPROC, glGetQueryiv, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLGETQUERYOBJECTIVPROC, glGetQueryObjectiv, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLGETQUERYOBJECTUIVPROC, glGetQueryObjectuiv, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLBINDBUFFERPROC, glBindBuffer, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLGENBUFFERSPROC, glGenBuffers, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLISBUFFERPROC, glIsBuffer, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLBUFFERDATAPROC, glBufferData, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLBUFFERSUBDATAPROC, glBufferSubData, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLGETBUFFERSUBDATAPROC, glGetBufferSubData, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLMAPBUFFERPROC, glMapBuffer, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLUNMAPBUFFERPROC, glUnmapBuffer, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLGETBUFFERPARAMETERIVPROC, glGetBufferParameteriv, tgl_extensions[TGL_EXTENSION_GL_1_5])
    LOADFUNC(PFNGLGETBUFFERPOINTERVPROC, glGetBufferPointerv, tgl_extensions[TGL_EXTENSION_GL_1_5])
    /* GL_version_2_0 */
    LOADFUNC(PFNGLBLENDEQUATIONSEPARATEPROC, glBlendEquationSeparate, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLDRAWBUFFERSPROC, glDrawBuffers, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLSTENCILOPSEPARATEPROC, glStencilOpSeparate, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLSTENCILFUNCSEPARATEPROC, glStencilFuncSeparate, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLSTENCILMASKSEPARATEPROC, glStencilMaskSeparate, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLATTACHSHADERPROC, glAttachShader, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLCOMPILESHADERPROC, glCompileShader, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLCREATEPROGRAMPROC, glCreateProgram, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLCREATESHADERPROC, glCreateShader, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLDELETEPROGRAMPROC, glDeleteProgram, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLDELETESHADERPROC, glDeleteShader, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLDETACHSHADERPROC, glDetachShader, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETACTIVEATTRIBPROC, glGetActiveAttrib, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETACTIVEUNIFORMPROC, glGetActiveUniform, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETATTACHEDSHADERSPROC, glGetAttachedShaders, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETPROGRAMIVPROC, glGetProgramiv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETSHADERIVPROC, glGetShaderiv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETSHADERSOURCEPROC, glGetShaderSource, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETUNIFORMFVPROC, glGetUniformfv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETUNIFORMIVPROC, glGetUniformiv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETVERTEXATTRIBDVPROC, glGetVertexAttribdv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETVERTEXATTRIBFVPROC, glGetVertexAttribfv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETVERTEXATTRIBIVPROC, glGetVertexAttribiv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLGETVERTEXATTRIBPOINTERVPROC, glGetVertexAttribPointerv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLISPROGRAMPROC, glIsProgram, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLISSHADERPROC, glIsShader, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLLINKPROGRAMPROC, glLinkProgram, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLSHADERSOURCEPROC, glShaderSource, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUSEPROGRAMPROC, glUseProgram, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM1FPROC, glUniform1f, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM2FPROC, glUniform2f, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM3FPROC, glUniform3f, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM4FPROC, glUniform4f, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM1IPROC, glUniform1i, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM2IPROC, glUniform2i, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM3IPROC, glUniform3i, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM4IPROC, glUniform4i, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM1FVPROC, glUniform1fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM2FVPROC, glUniform2fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM3FVPROC, glUniform3fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM4FVPROC, glUniform4fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM1IVPROC, glUniform1iv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM2IVPROC, glUniform2iv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM3IVPROC, glUniform3iv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORM4IVPROC, glUniform4iv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORMMATRIX2FVPROC, glUniformMatrix2fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORMMATRIX3FVPROC, glUniformMatrix3fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVALIDATEPROGRAMPROC, glValidateProgram, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB1DPROC, glVertexAttrib1d, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB1DVPROC, glVertexAttrib1dv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB1FPROC, glVertexAttrib1f, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB1FVPROC, glVertexAttrib1fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB1SPROC, glVertexAttrib1s, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB1SVPROC, glVertexAttrib1sv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB2DPROC, glVertexAttrib2d, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB2DVPROC, glVertexAttrib2dv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB2FPROC, glVertexAttrib2f, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB2FVPROC, glVertexAttrib2fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB2SPROC, glVertexAttrib2s, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB2SVPROC, glVertexAttrib2sv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB3DPROC, glVertexAttrib3d, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB3DVPROC, glVertexAttrib3dv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB3FPROC, glVertexAttrib3f, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB3FVPROC, glVertexAttrib3fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB3SPROC, glVertexAttrib3s, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB3SVPROC, glVertexAttrib3sv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4NBVPROC, glVertexAttrib4Nbv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4NIVPROC, glVertexAttrib4Niv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4NSVPROC, glVertexAttrib4Nsv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4NUBPROC, glVertexAttrib4Nub, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4NUBVPROC, glVertexAttrib4Nubv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4NUIVPROC, glVertexAttrib4Nuiv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4NUSVPROC, glVertexAttrib4Nusv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4BVPROC, glVertexAttrib4bv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4DPROC, glVertexAttrib4d, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4DVPROC, glVertexAttrib4dv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4FPROC, glVertexAttrib4f, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4FVPROC, glVertexAttrib4fv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4IVPROC, glVertexAttrib4iv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4SPROC, glVertexAttrib4s, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4SVPROC, glVertexAttrib4sv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4UBVPROC, glVertexAttrib4ubv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4UIVPROC, glVertexAttrib4uiv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIB4USVPROC, glVertexAttrib4usv, tgl_extensions[TGL_EXTENSION_GL_2_0])
    LOADFUNC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer, tgl_extensions[TGL_EXTENSION_GL_2_0])
    /* GL_version_2_1 */
    LOADFUNC(PFNGLUNIFORMMATRIX2X3FVPROC, glUniformMatrix2x3fv, tgl_extensions[TGL_EXTENSION_GL_2_1])
    LOADFUNC(PFNGLUNIFORMMATRIX3X2FVPROC, glUniformMatrix3x2fv, tgl_extensions[TGL_EXTENSION_GL_2_1])
    LOADFUNC(PFNGLUNIFORMMATRIX2X4FVPROC, glUniformMatrix2x4fv, tgl_extensions[TGL_EXTENSION_GL_2_1])
    LOADFUNC(PFNGLUNIFORMMATRIX4X2FVPROC, glUniformMatrix4x2fv, tgl_extensions[TGL_EXTENSION_GL_2_1])
    LOADFUNC(PFNGLUNIFORMMATRIX3X4FVPROC, glUniformMatrix3x4fv, tgl_extensions[TGL_EXTENSION_GL_2_1])
    LOADFUNC(PFNGLUNIFORMMATRIX4X3FVPROC, glUniformMatrix4x3fv, tgl_extensions[TGL_EXTENSION_GL_2_1])
    /* GL_version_3_0 */
    LOADFUNC(PFNGLCOLORMASKIPROC, glColorMaski, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETBOOLEANI_VPROC, glGetBooleani_v, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETINTEGERI_VPROC, glGetIntegeri_v, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLENABLEIPROC, glEnablei, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLDISABLEIPROC, glDisablei, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLISENABLEDIPROC, glIsEnabledi, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLBEGINTRANSFORMFEEDBACKPROC, glBeginTransformFeedback, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLENDTRANSFORMFEEDBACKPROC, glEndTransformFeedback, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLBINDBUFFERRANGEPROC, glBindBufferRange, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLBINDBUFFERBASEPROC, glBindBufferBase, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLTRANSFORMFEEDBACKVARYINGSPROC, glTransformFeedbackVaryings, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETTRANSFORMFEEDBACKVARYINGPROC, glGetTransformFeedbackVarying, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLCLAMPCOLORPROC, glClampColor, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLBEGINCONDITIONALRENDERPROC, glBeginConditionalRender, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLENDCONDITIONALRENDERPROC, glEndConditionalRender, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBIPOINTERPROC, glVertexAttribIPointer, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETVERTEXATTRIBIIVPROC, glGetVertexAttribIiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETVERTEXATTRIBIUIVPROC, glGetVertexAttribIuiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI1IPROC, glVertexAttribI1i, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI2IPROC, glVertexAttribI2i, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI3IPROC, glVertexAttribI3i, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI4IPROC, glVertexAttribI4i, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI1UIPROC, glVertexAttribI1ui, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI2UIPROC, glVertexAttribI2ui, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI3UIPROC, glVertexAttribI3ui, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI4UIPROC, glVertexAttribI4ui, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI1IVPROC, glVertexAttribI1iv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI2IVPROC, glVertexAttribI2iv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI3IVPROC, glVertexAttribI3iv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI4IVPROC, glVertexAttribI4iv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI1UIVPROC, glVertexAttribI1uiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI2UIVPROC, glVertexAttribI2uiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI3UIVPROC, glVertexAttribI3uiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI4UIVPROC, glVertexAttribI4uiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI4BVPROC, glVertexAttribI4bv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI4SVPROC, glVertexAttribI4sv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI4UBVPROC, glVertexAttribI4ubv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLVERTEXATTRIBI4USVPROC, glVertexAttribI4usv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETUNIFORMUIVPROC, glGetUniformuiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLBINDFRAGDATALOCATIONPROC, glBindFragDataLocation, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETFRAGDATALOCATIONPROC, glGetFragDataLocation, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLUNIFORM1UIPROC, glUniform1ui, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLUNIFORM2UIPROC, glUniform2ui, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLUNIFORM3UIPROC, glUniform3ui, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLUNIFORM4UIPROC, glUniform4ui, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLUNIFORM1UIVPROC, glUniform1uiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLUNIFORM2UIVPROC, glUniform2uiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLUNIFORM3UIVPROC, glUniform3uiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLUNIFORM4UIVPROC, glUniform4uiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLTEXPARAMETERIIVPROC, glTexParameterIiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLTEXPARAMETERIUIVPROC, glTexParameterIuiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETTEXPARAMETERIIVPROC, glGetTexParameterIiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETTEXPARAMETERIUIVPROC, glGetTexParameterIuiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLCLEARBUFFERIVPROC, glClearBufferiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLCLEARBUFFERUIVPROC, glClearBufferuiv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLCLEARBUFFERFVPROC, glClearBufferfv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLCLEARBUFFERFIPROC, glClearBufferfi, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETSTRINGIPROC, glGetStringi, tgl_extensions[TGL_EXTENSION_GL_3_0])
    /* ARB_framebuffer_object */
    LOADFUNC(PFNGLISRENDERBUFFERPROC, glIsRenderbuffer, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETRENDERBUFFERPARAMETERIVPROC, glGetRenderbufferParameteriv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLISFRAMEBUFFERPROC, glIsFramebuffer, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLFRAMEBUFFERTEXTURE1DPROC, glFramebufferTexture1D, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLFRAMEBUFFERTEXTURE3DPROC, glFramebufferTexture3D, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC, glGetFramebufferAttachmentParameteriv, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLBLITFRAMEBUFFERPROC, glBlitFramebuffer, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC, glRenderbufferStorageMultisample, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLFRAMEBUFFERTEXTURELAYERPROC, glFramebufferTextureLayer, tgl_extensions[TGL_EXTENSION_GL_3_0])
    /* ARB_map_buffer_range */
    LOADFUNC(PFNGLMAPBUFFERRANGEPROC, glMapBufferRange, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLFLUSHMAPPEDBUFFERRANGEPROC, glFlushMappedBufferRange, tgl_extensions[TGL_EXTENSION_GL_3_0])
    /* ARB_vertex_array_object */
    LOADFUNC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays, tgl_extensions[TGL_EXTENSION_GL_3_0])
    LOADFUNC(PFNGLISVERTEXARRAYPROC, glIsVertexArray, tgl_extensions[TGL_EXTENSION_GL_3_0])
    /* GL_version_3_1 */
    LOADFUNC(PFNGLDRAWARRAYSINSTANCEDPROC, glDrawArraysInstanced, tgl_extensions[TGL_EXTENSION_GL_3_1])
    LOADFUNC(PFNGLDRAWELEMENTSINSTANCEDPROC, glDrawElementsInstanced, tgl_extensions[TGL_EXTENSION_GL_3_1])
    LOADFUNC(PFNGLTEXBUFFERPROC, glTexBuffer, tgl_extensions[TGL_EXTENSION_GL_3_1])
    LOADFUNC(PFNGLPRIMITIVERESTARTINDEXPROC, glPrimitiveRestartIndex, tgl_extensions[TGL_EXTENSION_GL_3_1])
    /* ARB_copy_buffer */
    LOADFUNC(PFNGLCOPYBUFFERSUBDATAPROC, glCopyBufferSubData, tgl_extensions[TGL_EXTENSION_GL_3_1])
    /* ARB_uniform_buffer_object */
    LOADFUNC(PFNGLGETUNIFORMINDICESPROC, glGetUniformIndices, tgl_extensions[TGL_EXTENSION_GL_3_1])
    LOADFUNC(PFNGLGETACTIVEUNIFORMSIVPROC, glGetActiveUniformsiv, tgl_extensions[TGL_EXTENSION_GL_3_1])
    LOADFUNC(PFNGLGETACTIVEUNIFORMNAMEPROC, glGetActiveUniformName, tgl_extensions[TGL_EXTENSION_GL_3_1])
    LOADFUNC(PFNGLGETUNIFORMBLOCKINDEXPROC, glGetUniformBlockIndex, tgl_extensions[TGL_EXTENSION_GL_3_1])
    LOADFUNC(PFNGLGETACTIVEUNIFORMBLOCKIVPROC, glGetActiveUniformBlockiv, tgl_extensions[TGL_EXTENSION_GL_3_1])
    LOADFUNC(PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC, glGetActiveUniformBlockName, tgl_extensions[TGL_EXTENSION_GL_3_1])
    LOADFUNC(PFNGLUNIFORMBLOCKBINDINGPROC, glUniformBlockBinding, tgl_extensions[TGL_EXTENSION_GL_3_1])
    /* GL_version_3_2 */
    LOADFUNC(PFNGLGETINTEGER64I_VPROC, glGetInteger64i_v, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLGETBUFFERPARAMETERI64VPROC, glGetBufferParameteri64v, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLFRAMEBUFFERTEXTUREPROC, glFramebufferTexture, tgl_extensions[TGL_EXTENSION_GL_3_2])
    /* ARB_draw_elements_base_vertex */
    LOADFUNC(PFNGLDRAWELEMENTSBASEVERTEXPROC, glDrawElementsBaseVertex, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC, glDrawRangeElementsBaseVertex, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC, glDrawElementsInstancedBaseVertex, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC, glMultiDrawElementsBaseVertex, tgl_extensions[TGL_EXTENSION_GL_3_2])
    /* ARB_provoking_vertex */
    LOADFUNC(PFNGLPROVOKINGVERTEXPROC, glProvokingVertex, tgl_extensions[TGL_EXTENSION_GL_3_2])
    /* ARB_sync */
    LOADFUNC(PFNGLFENCESYNCPROC, glFenceSync, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLISSYNCPROC, glIsSync, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLDELETESYNCPROC, glDeleteSync, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLCLIENTWAITSYNCPROC, glClientWaitSync, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLWAITSYNCPROC, glWaitSync, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLGETINTEGER64VPROC, glGetInteger64v, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLGETSYNCIVPROC, glGetSynciv, tgl_extensions[TGL_EXTENSION_GL_3_2])
    /* ARB_texture_multisample */
    LOADFUNC(PFNGLTEXIMAGE2DMULTISAMPLEPROC, glTexImage2DMultisample, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLTEXIMAGE3DMULTISAMPLEPROC, glTexImage3DMultisample, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLGETMULTISAMPLEFVPROC, glGetMultisamplefv, tgl_extensions[TGL_EXTENSION_GL_3_2])
    LOADFUNC(PFNGLSAMPLEMASKIPROC, glSampleMaski, tgl_extensions[TGL_EXTENSION_GL_3_2])
    /* GL_version_3_3 */
    LOADFUNC(PFNGLVERTEXATTRIBDIVISORPROC, glVertexAttribDivisor, tgl_extensions[TGL_EXTENSION_GL_3_3])
    /* ARB_blend_func_extended */
    LOADFUNC(PFNGLBINDFRAGDATALOCATIONINDEXEDPROC, glBindFragDataLocationIndexed, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLGETFRAGDATAINDEXPROC, glGetFragDataIndex, tgl_extensions[TGL_EXTENSION_GL_3_3])
    /* ARB_sampler_objects */
    LOADFUNC(PFNGLGENSAMPLERSPROC, glGenSamplers, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLDELETESAMPLERSPROC, glDeleteSamplers, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLISSAMPLERPROC, glIsSampler, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLBINDSAMPLERPROC, glBindSampler, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLSAMPLERPARAMETERIPROC, glSamplerParameteri, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLSAMPLERPARAMETERIVPROC, glSamplerParameteriv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLSAMPLERPARAMETERFPROC, glSamplerParameterf, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLSAMPLERPARAMETERFVPROC, glSamplerParameterfv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLSAMPLERPARAMETERIIVPROC, glSamplerParameterIiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLSAMPLERPARAMETERIUIVPROC, glSamplerParameterIuiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLGETSAMPLERPARAMETERIVPROC, glGetSamplerParameteriv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLGETSAMPLERPARAMETERIIVPROC, glGetSamplerParameterIiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLGETSAMPLERPARAMETERFVPROC, glGetSamplerParameterfv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLGETSAMPLERPARAMETERIUIVPROC, glGetSamplerParameterIuiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    /* ARB_timer_query */
    LOADFUNC(PFNGLQUERYCOUNTERPROC, glQueryCounter, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLGETQUERYOBJECTI64VPROC, glGetQueryObjecti64v, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLGETQUERYOBJECTUI64VPROC, glGetQueryObjectui64v, tgl_extensions[TGL_EXTENSION_GL_3_3])
    /* ARB_vertex_type_2_10_10_10_rev */
    LOADFUNC(PFNGLVERTEXP2UIPROC, glVertexP2ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXP2UIVPROC, glVertexP2uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXP3UIPROC, glVertexP3ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXP3UIVPROC, glVertexP3uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXP4UIPROC, glVertexP4ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXP4UIVPROC, glVertexP4uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLTEXCOORDP1UIPROC, glTexCoordP1ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLTEXCOORDP1UIVPROC, glTexCoordP1uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLTEXCOORDP2UIPROC, glTexCoordP2ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLTEXCOORDP2UIVPROC, glTexCoordP2uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLTEXCOORDP3UIPROC, glTexCoordP3ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLTEXCOORDP3UIVPROC, glTexCoordP3uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLTEXCOORDP4UIPROC, glTexCoordP4ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLTEXCOORDP4UIVPROC, glTexCoordP4uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLMULTITEXCOORDP1UIPROC, glMultiTexCoordP1ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLMULTITEXCOORDP1UIVPROC, glMultiTexCoordP1uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLMULTITEXCOORDP2UIPROC, glMultiTexCoordP2ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLMULTITEXCOORDP2UIVPROC, glMultiTexCoordP2uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLMULTITEXCOORDP3UIPROC, glMultiTexCoordP3ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLMULTITEXCOORDP3UIVPROC, glMultiTexCoordP3uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLMULTITEXCOORDP4UIPROC, glMultiTexCoordP4ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLMULTITEXCOORDP4UIVPROC, glMultiTexCoordP4uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLNORMALP3UIPROC, glNormalP3ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLNORMALP3UIVPROC, glNormalP3uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLCOLORP3UIPROC, glColorP3ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLCOLORP3UIVPROC, glColorP3uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLCOLORP4UIPROC, glColorP4ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLCOLORP4UIVPROC, glColorP4uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLSECONDARYCOLORP3UIPROC, glSecondaryColorP3ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLSECONDARYCOLORP3UIVPROC, glSecondaryColorP3uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXATTRIBP1UIPROC, glVertexAttribP1ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXATTRIBP1UIVPROC, glVertexAttribP1uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXATTRIBP2UIPROC, glVertexAttribP2ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXATTRIBP2UIVPROC, glVertexAttribP2uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXATTRIBP3UIPROC, glVertexAttribP3ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXATTRIBP3UIVPROC, glVertexAttribP3uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXATTRIBP4UIPROC, glVertexAttribP4ui, tgl_extensions[TGL_EXTENSION_GL_3_3])
    LOADFUNC(PFNGLVERTEXATTRIBP4UIVPROC, glVertexAttribP4uiv, tgl_extensions[TGL_EXTENSION_GL_3_3])
    /* ARB_debug_output */
    LOADFUNC(PFNGLDEBUGMESSAGECONTROLARBPROC, glDebugMessageControlARB, tgl_extensions[TGL_EXTENSION_ARB_DEBUG_OUTPUT])
    LOADFUNC(PFNGLDEBUGMESSAGEINSERTARBPROC, glDebugMessageInsertARB, tgl_extensions[TGL_EXTENSION_ARB_DEBUG_OUTPUT])
    LOADFUNC(PFNGLDEBUGMESSAGECALLBACKARBPROC, glDebugMessageCallbackARB, tgl_extensions[TGL_EXTENSION_ARB_DEBUG_OUTPUT])
    LOADFUNC(PFNGLGETDEBUGMESSAGELOGARBPROC, glGetDebugMessageLogARB, tgl_extensions[TGL_EXTENSION_ARB_DEBUG_OUTPUT])
    /* AMD_debug_output */
    if (tglIsAvailable(TGL_EXTENSION_ARB_DEBUG_OUTPUT)) {
        tgl_extensions[TGL_EXTENSION_AMD_DEBUG_OUTPUT] = GL_FALSE;
    } else {
        LOADFUNC(PFNGLDEBUGMESSAGEENABLEAMDPROC, glDebugMessageEnableAMD, tgl_extensions[TGL_EXTENSION_AMD_DEBUG_OUTPUT])
        LOADFUNC(PFNGLDEBUGMESSAGEINSERTAMDPROC, glDebugMessageInsertAMD, tgl_extensions[TGL_EXTENSION_AMD_DEBUG_OUTPUT])
        LOADFUNC(PFNGLDEBUGMESSAGECALLBACKAMDPROC, glDebugMessageCallbackAMD, tgl_extensions[TGL_EXTENSION_AMD_DEBUG_OUTPUT])
        LOADFUNC(PFNGLGETDEBUGMESSAGELOGAMDPROC, glGetDebugMessageLogAMD, tgl_extensions[TGL_EXTENSION_AMD_DEBUG_OUTPUT])
    }

#ifdef _DEBUG
    if (tglIsAvailable(TGL_EXTENSION_ARB_DEBUG_OUTPUT)) {
        glDebugMessageCallbackARB(_tglDebugLog, NULL);
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageControlARB(GL_DONT_CARE,
                                 GL_DONT_CARE,
                                 GL_DONT_CARE,
                                 0, NULL, GL_TRUE);
        glDebugMessageControlARB(GL_DEBUG_SOURCE_OTHER_ARB,
                                 GL_DONT_CARE,
                                 GL_DONT_CARE,
                                 0, NULL, GL_FALSE);
        glDebugMessageControlARB(GL_DONT_CARE,
                                 GL_DEBUG_TYPE_OTHER_ARB,
                                 GL_DONT_CARE,
                                 0, NULL, GL_FALSE);
        glDebugMessageControlARB(GL_DEBUG_SOURCE_APPLICATION_ARB,
                                 GL_DEBUG_TYPE_OTHER_ARB,
                                 GL_DONT_CARE,
                                 0, NULL, GL_TRUE);
    } else if (tglIsAvailable(TGL_EXTENSION_AMD_DEBUG_OUTPUT)) {
        glDebugMessageCallbackAMD(_tglDebugLogAMD, NULL);
        glDebugMessageEnableAMD(0, 0, 0, 0, GL_TRUE);
        glDebugMessageEnableAMD(GL_DEBUG_CATEGORY_OTHER_AMD, 0, 0, 0, GL_FALSE);
    } else {
        OutputDebugStringA("Warning: no GL debug information available from driver\n");
    }
#endif
}

GLboolean tglIsAvailable(TGLEXTENSION ext) {
    return tgl_extensions[ext];
}

void tglDebugMessage(GLenum severity, const char *msg) {
#ifdef _DEBUG
    if (tglIsAvailable(TGL_EXTENSION_ARB_DEBUG_OUTPUT)) {
        glDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_OTHER_ARB, 0, severity, 0, msg);
    } else if (tglIsAvailable(TGL_EXTENSION_AMD_DEBUG_OUTPUT)) {
        glDebugMessageInsertAMD(GL_DEBUG_CATEGORY_APPLICATION_AMD, severity, 0, 0, msg);
    } else {
        OutputDebugStringA("GL debug: ");
        OutputDebugStringA(msg);
        OutputDebugStringA("\n");
    }
#endif
}
