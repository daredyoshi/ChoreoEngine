#include "cepch.h"

#include "RenderCommand.h"
#include "Application/Platform/OpenGL/OpenGLRendererAPI.h"

namespace ChoreoEngine {
    // in the future this will be set dynamically 
    RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}
