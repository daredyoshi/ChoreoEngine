#include "Key.h"


namespace ChoreoApp {

    void FloatKey::addOnSetValCallback(std::string& callbackName, std::function<void()> callback){
        if(m_onSetValCallbacks.find(callbackName) == m_onSetValCallbacks.end()){
            m_onSetValCallbacks[callbackName] = callback;
        }
        else{
            CE_WARN("Callback key already exists new callback has not been added!");
        }
    }


    void FloatKey::setVal(const float val){
        m_val= val;    
        // this is how we are managing dependencies for now 
        for (auto& fn : m_onSetValCallbacks){
            fn.second();
        }
    }
}
