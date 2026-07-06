#pragma once

#include <iostream>
#include <type_traits>
#include "Slots.h"


template<SlotSize size, std::size_t slotAmount>
class pool{

public:
    explicit pool()
        :   start(static_cast<std::byte*>(::operator new(slotSize*slotAmount, std::align_val_t{poolAlignment}))),
            capacity(slotAmount),
            end(start+(slotSize*slotAmount))
    {

        currentHead = start;
        std::byte* current = start;
        std::byte* next;
        //begin initialisation
        for(; current + slotSize < end; current += slotSize)
        {
            next = current + slotSize;
            *reinterpret_cast<std::byte**>(current) = next;
        }
        *reinterpret_cast<std::byte**>(current) = nullptr; //Last memory location as nullptr. this means weve ran out of memory.

        slotsLeft = slotAmount;
    }

    pool(const pool&) = delete;
    pool& operator=(const pool&) = delete;

    ~pool(){
        ::operator delete (start, std::align_val_t{poolAlignment});
    }

    void checkPtr(){ //debug function delete later
        
        std::byte* nextHead = *reinterpret_cast<std::byte**>(currentHead);
        std::cout << "head ptr: " << currentHead << '\n';
        std::cout << "ptr to head ptr: " << nextHead << '\n';
    }

    template<typename T, typename... Args>
    T* create(Args&&... args)
    {
        static_assert(sizeof(T)<= slotSize, "Item to create must be smaller than the slot size");
        static_assert(alignof(T) <= poolAlignment, "Items needs stronger alignment than pool provides");
        static_assert(std::is_trivially_destructible_v<T>, "Item needs to be trivially destructible");

        if(slotsLeft <= 0)  return nullptr;
        std::byte* nextHead = *reinterpret_cast<std::byte**>(currentHead);

        T* ptr = new (currentHead) T(std::forward<Args>(args)...);

        currentHead = nextHead;
        slotsLeft--;
        return ptr;


    }

    template<typename T>
    void destroy(T*& ptr){
        *reinterpret_cast<std::byte**>(ptr) = currentHead;
        currentHead = reinterpret_cast<std::byte*>(ptr);
        slotsLeft++;
        ptr = nullptr;
    }

    std::size_t getFreeSlots(){
        return slotsLeft;
    }

    std::size_t getCapacity()
    {
        return slotAmount;
    }


private:
    static std::size_t constexpr slotSize = toBytes(size);
    static std::size_t constexpr poolAlignment = (slotSize<=64)?64:slotSize;
    std::byte* const start; //Start of pool memory block
    std::byte* currentHead; //Current top of the list of free memory
    const std::size_t capacity; //total capacity of pool in slots
    std::byte* const end;   //end + 1  of pool memory block
    std::size_t slotsLeft;


    
};