#pragma once

#include "Application/Core.h"
#include "spdlog/spdlog.h"
#include <memory>


namespace ChoreoEngine {
    class Log{
    public:

        static void Init();

        inline static Ref<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
        inline static Ref<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> m_CoreLogger;        
        static std::shared_ptr<spdlog::logger> m_ClientLogger;        
    };
}


// if dist build

#ifdef CE_DEBUG
// Core log macros
#define CE_CORE_TRACE(...)      ::ChoreoEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CE_CORE_INFO(...)       ::ChoreoEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CE_CORE_WARN(...)       ::ChoreoEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CE_CORE_ERROR(...)      ::ChoreoEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CE_CORE_FATAL(...)      ::ChoreoEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)


// Client log macros
#define CE_TRACE(...)           ::ChoreoEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CE_INFO(...)            ::ChoreoEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define CE_WARN(...)            ::ChoreoEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CE_ERROR(...)           ::ChoreoEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define CE_FATAL(...)           ::ChoreoEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)

#else

#define CE_CORE_TRACE(...) 
#define CE_CORE_INFO(...)  
#define CE_CORE_WARN(...)  
#define CE_CORE_ERROR(...) 
#define CE_CORE_FATAL(...) 


// Client log macros
#define CE_TRACE(...)      
#define CE_INFO(...)       
#define CE_WARN(...)       
#define CE_ERROR(...)      
#define CE_FATAL(...)      

#endif
