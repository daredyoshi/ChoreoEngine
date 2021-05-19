#pragma once

#include <bits/stdint-uintn.h>

class UniqueID {
protected:
   static uint32_t nextID;
public:
   uint32_t id;
   UniqueID();
   UniqueID(const UniqueID& orig);
   UniqueID& operator=(const UniqueID& orig);
};

