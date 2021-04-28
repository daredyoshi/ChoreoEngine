#pragma once

#include <memory>


// this just shits the bit by x places. 
// so if you have bit 1 you can offset it. 
// This way we can create a bitfield with multiple 
// events set and then just mask them out.
#define BIT(x) (1 << x)

#define CE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// in case we want to sitch away from unique_ptr or shared_ptr at some point
// we could make a class out of these if we wanted to
namespace ChoreoApp {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
