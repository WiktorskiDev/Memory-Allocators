#pragma once

#include <iostream>


enum class SlotSize{
    B16,
    B32,
    B64,
    B128,
    B256,
    maxSize = 256
};

static constexpr std::size_t toBytes(SlotSize slotVal){

    switch (slotVal)
    {
    case (SlotSize::B16):
        return 16;
        
    case (SlotSize::B32):
        return 32;
        
    case (SlotSize::B64):
        return 64;

    case (SlotSize::B128):
        return 128;
    case (SlotSize::B256):
        return 256;
    case (SlotSize::maxSize):
        return 256;
    }   
}

template<typename T>
static constexpr SlotSize toSlot()
{
    constexpr std::size_t sizeT = sizeof(T);
    static_assert(sizeT>=0, "Size must be greater than 0 when converting to slot");
    static_assert(sizeT<= toBytes(SlotSize::maxSize), "Size must be smaller than slot maxSize (256)");
    
    if constexpr (sizeT<=16) return SlotSize::B16;
    if constexpr (sizeT<=32) return SlotSize::B32;
    if constexpr (sizeT<=64) return SlotSize::B64;
    if constexpr (sizeT<=128) return SlotSize::B128;
    if constexpr (sizeT<=256) return SlotSize::B256;
}
