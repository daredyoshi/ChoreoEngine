#pragma once

#include "Application/Events/Event.h"

#include <sstream>

namespace ChoreoEngine {
    class KeyEvent : public Event{
    public:
        inline int getKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        // can't make this class unless deriving from it
        KeyEvent(int keyCode)
            : m_KeyCode(keyCode) {}

        int m_KeyCode;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        // not sure if we want to keep track of how many times the key has repeated
        // but this can also just work as a boolean to see wether to re-trigger shits
        KeyPressedEvent(int keyCode, int repeatCount)
            : KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

        inline int getRepeatCount() const { return m_RepeatCount; }

        
        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }
        EVENT_CLASS_TYPE(KeyPressed);

    private:
        int m_RepeatCount;
    };

   class KeyTypedEvent: public KeyEvent {
    public:
        // not sure if we want to keep track of how many times the key has repeated
        // but this can also just work as a boolean to see wether to re-trigger shits
        KeyTypedEvent(int keyCode)
            : KeyEvent(keyCode) {}


        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }
        EVENT_CLASS_TYPE(KeyTyped);
    };


    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(int keyCode)
            : KeyEvent(keyCode) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode;
            return ss.str();
        }
        EVENT_CLASS_TYPE(KeyReleased);
    };

}



