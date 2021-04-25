#pragma once 

#include <bits/stdint-uintn.h>
#include "Application/Core.h"
#include "capch.h"

namespace ChoreoApp{
    class Texture{
    public:
        virtual ~Texture() = default;

        virtual uint32_t  getWidth() const = 0;
        virtual uint32_t  getHeight() const = 0;
        virtual uint32_t getId() const = 0; 
        
        virtual void bind(uint32_t slot) const  = 0;

        virtual bool operator==(const Texture& other) const =0;
    };

    class Texture2D : public Texture{
    public:
        static Ref<Texture2D> create(const std::string& path);
        static Ref<Texture2D> create(uint32_t width, uint32_t height);
        virtual void setdata(void* data, uint32_t size) = 0;

    };

}
