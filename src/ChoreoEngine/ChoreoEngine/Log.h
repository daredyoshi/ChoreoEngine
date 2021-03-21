#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include <memory>


namespace ChoreoEngine {
    class Log{
    public:

        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> m_CoreLogger;        
        static std::shared_ptr<spdlog::logger> m_ClientLogger;        
    };
}


// if dist build

#ifdef DEBUG
// Core log macros
#define CF_CORE_TRACE(...)      ::ChoreoEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CF_CORE_INFO(...)       ::ChoreoEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CF_CORE_WARN(...)       ::ChoreoEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CF_CORE_ERROR(...)      ::ChoreoEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CF_CORE_FATAL(...)      ::ChoreoEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)


// Client log macros
#define CF_TRACE(...)           ::ChoreoEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CF_INFO(...)            ::ChoreoEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define CF_WARN(...)            ::ChoreoEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CF_ERROR(...)           ::ChoreoEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define CF_FATAL(...)           ::ChoreoEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)

#else

#define CF_CORE_TRACE(...) 
#define CF_CORE_INFO(...)  
#define CF_CORE_WARN(...)  
#define CF_CORE_ERROR(...) 
#define CF_CORE_FATAL(...) 


// Client log macros
#define CF_TRACE(...)      
#define CF_INFO(...)       
#define CF_WARN(...)       
#define CF_ERROR(...)      
#define CF_FATAL(...)      

#endif
