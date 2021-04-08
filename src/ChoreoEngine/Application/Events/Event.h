#pragma once

#include "Core.h"

#include <string>
#include <functional>

namespace ChoreoEngine {

    // Events in ChoreoEngine are currently blocknig, meaning when an event occurs, 
    // it immeidately gets dispatched and must be dealt with. 
    //
    // A better strategy in the future might be to use buffer events in an 
    // event bus and process them during the "event" part of the update stage
    
    enum class EventType{
        None = 0,

        WindowClose, 
        WindowResize,
        WindowFocus, 
        WindowLostfocus, 
        WindowMoved,

        AppTick, 
        AppUpdate, 
        AppRender,

        KeyPressed, 
        KeyReleased,
        KeyTyped,

        MouseButtonPressed,
        MouseButtonReleased, 
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory{
        None = 0,
        EventCategoryApplication    =  BIT(1),
        EventCategoryInput          =  BIT(2),
        EventCategoryKeyboard       =  BIT(3),
        EventCategoryMouse          =  BIT(4),
        EventCategoryMouseButton    =  BIT(5)
    };

// this is just to avoid duplicate code for every subclass
#define EVENT_CLASS_TYPE(type)  static EventType getStaticType() { return EventType::type; }\
                                virtual EventType getEventType() const override { return getStaticType(); }\
                                virtual const char* getName() const override { return #type; } 

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

    // base class for event
    class Event{
        friend class EventDispatcher;

    public:
        // need to be implemented
        // We need to know what type this is. 
        // This is static because they won't even change across events
        //
        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual int getCategoryFlags() const = 0;

        bool getHandled() const { return m_handled; }
        // useful for debugging
        operator std::string() const { return toString(); }
        virtual std::string toString() const { return getName(); }


        inline bool IsInCategory(EventCategory category){
            return getCategoryFlags() & category;
        }
    protected:
        // for preventing events not getting propegated further
        // allows consuming of events
        // e.g. consume the event in gui instead of viewer
        bool m_handled = false;
    };

    // When we recive an event, it will be coming in as a reference to that event. 
    // It could be any type of event, so we're using a template to create an instance
    // of the class for every type. 
    class EventDispatcher {
        // T in this case could be any event type
        template<typename T>
        using EventFn = std::function<bool(T&)>;

    public:
        EventDispatcher(Event& event)
            : m_event(event) {}

        // If the event that is being dispatched matches the function it will 
        // actually run that function
        template<typename T>
        bool dispatch(EventFn<T> func){
            if (m_event.getEventType() == T::getStaticType()){
                m_event.m_handled = func(*(T*)&m_event);
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };

    // this is for our loggin library
    inline std::ostream& operator<<(std::ostream& os, const Event& e){
        return os << e.toString();
    }

}

