#pragma once

#include <iostream>
#include <type_traits>
#include "Slots.h"


template<SlotSize slotSize, std::size_t slotAmount>
class arenaF{

public:
    explicit arenaF()
        :   start(static_cast<std::byte*>(::operator new(toBytes(slotSize)*slotAmount, std::align_val_t{64}))),
            capacity(slotAmount*toBytes(slotSize)),
            current(start),
            end(start+capacity)
    {

    };

    ~arenaF(){
        ::operator delete(start, std::align_val_t{64});
    };

    arenaF(const arenaF&) = delete;
    arenaF& operator=(const arenaF&) = delete;

    template<typename T, typename... Args>
    [[nodiscard]]
    T* create(Args&&... args){
        static_assert(std::is_trivially_destructible_v<T>, "Type to create must be trivially destructible");
        static_assert(alignof(T) <= 64, "Type must have alignment smaller than 64 to be created");
        if(current < end){
        T* ptr = new (current) T(std::forward<Args>(args)...);
        current += slotBytes;
        return ptr;
        } else
        {
            return nullptr;
        }
    } 

    void reset() noexcept  {
        current = start;
    }

    std::size_t getCapacity() noexcept{
    return slotAmount;
    }

private:
    static std::size_t constexpr slotBytes = toBytes(slotSize);
    static std::size_t constexpr arenaAlignment = (slotBytes<=64)?64:slotBytes;
    std::byte* const start;
    std::size_t const capacity;
    std::byte* current;
    std::byte* const end;


    std::size_t capacityLeft(){
        return ((start+capacity)-current);
    }

    /*Input: Size - size of item to be allocated
    *        Alignment - alignment of item to be allocated
    * Returns: pointer to start of buffer where item can be placed or nullptr if item cant be placed
    */
};