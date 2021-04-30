#include "Controller.h"
#include "Scene.h"

namespace ChoreoApp{
    double LinearFloatController::eval() const {
        if ( m_keys.size() == 1){
            return FloatKey{m_keys[0]};
        }

        uint32_t t = *( m_scene->getTime() );
        for(unsigned int i = 0; i < m_keys.size(); ++i){
            FloatKey k = m_keys[i]; 
            if ((uint32_t)k > t){
                return k; 
            }
        }
        CE_CORE_ASSERT(false, "Controller did not find keys")
        return 0; 
    };
}
