#pragma once

#include "Application/Core.h"
#include "spdlog/spdlog.h"
#include <memory>


namespace ChoreoApp {
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
#define CE_CORE_TRACE(...)      ::ChoreoApp::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CE_CORE_INFO(...)       ::ChoreoApp::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CE_CORE_WARN(...)       ::ChoreoApp::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CE_CORE_ERROR(...)      ::ChoreoApp::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CE_CORE_FATAL(...)      ::ChoreoApp::Log::GetCoreLogger()->fatal(__VA_ARGS__)


// Client log macros
#define CE_TRACE(...)           ::ChoreoApp::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CE_INFO(...)            ::ChoreoApp::Log::GetClientLogger()->info(__VA_ARGS__)
#define CE_WARN(...)            ::ChoreoApp::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CE_ERROR(...)           ::ChoreoApp::Log::GetClientLogger()->error(__VA_ARGS__)
#define CE_FATAL(...)           ::ChoreoApp::Log::GetClientLogger()->fatal(__VA_ARGS__)

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
