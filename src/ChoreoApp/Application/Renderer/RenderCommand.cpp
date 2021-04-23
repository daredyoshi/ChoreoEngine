#include "capch.h"

#include "RenderCommand.h"
#include "Application/Platform/OpenGL/OpenGLRendererAPI.h"

namespace ChoreoApp {
    // in the future this will be set dynamically 
    RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}
