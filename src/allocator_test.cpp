#include <ArenaFast.h>
#include <Pool.h>
#include <BenchStructs.h>
#include <cstdint>
#include <gtest/gtest.h>

TEST(ArenaTests, CanCreate)
{
    //Arrange
    arenaF<SlotSize::B32, 100> arena;

    //Act
    int* firstInt = arena.create<int>(42);
    int* secondInt = arena.create<int>(221);
    BenchOrder* thirdOrder = arena.create<BenchOrder>(123456, 150150, 100, 100, Side::Buy, 32);

    //Assert
    ASSERT_NE(firstInt, nullptr);
    ASSERT_NE(secondInt, nullptr);
    ASSERT_NE(thirdOrder, nullptr);
    ASSERT_NE(firstInt, secondInt);

    auto firstAdd = reinterpret_cast<std::uintptr_t>(firstInt);
    auto thirdAdd = reinterpret_cast<std::uintptr_t>(thirdOrder);
    ASSERT_NE(firstAdd, thirdAdd);

    EXPECT_EQ(*firstInt, 42);
    EXPECT_EQ(*secondInt, 221);
    EXPECT_EQ(thirdOrder->orderId, 123456);
    
}

TEST(ArenaTests, UsesAllSlots)
{
    //Arrange
    arenaF<SlotSize::B32, 100> arena;
    int* ptrs[100];

    //Act
    for(int i = 0;i<100;++i){
        ptrs[i] = arena.create<int>(100+i);
    }

    //Assert
    for(int i = 0; i<100;++i){
        ASSERT_NE(ptrs[i], nullptr);
        EXPECT_EQ(*ptrs[i], 100+i);
    }
    
}

TEST(ArenaTests, OverflowsCleanly)
{
    //Arrange
    arenaF<SlotSize::B32, 100> arena;
    int* ptrs[100];

    //Act
    for(int i = 0;i<100;++i){
        ptrs[i] = arena.create<int>(100+i);
    }

    //Assert
    for(int* ptr : ptrs)
    {
        ASSERT_NE(ptr, nullptr);
    }

    int* overflow = arena.create<int>(300);

    EXPECT_EQ(overflow, nullptr);
    
}

TEST(ArenaTests, CanReset)
{
    //Arrange
    arenaF<SlotSize::B32, 100> arena;

    //Act
    int* firstInt = arena.create<int>(42);
    arena.reset();
    int* dupedPtr = arena.create<int>(600);

    //Assert
    ASSERT_NE(firstInt, nullptr);
    ASSERT_NE(dupedPtr, nullptr);

    EXPECT_EQ(firstInt, dupedPtr);
    EXPECT_EQ(*dupedPtr, 600);
    
}

TEST(ArenaTests, IsAligned)
{
    //Arrange
    arenaF<SlotSize::B32, 100> arena;

    //Act
    int* firstInt = arena.create<int>(42);
    int* secondInt = arena.create<int>(221);
    BenchOrder* thirdOrder = arena.create<BenchOrder>(123456, 150150, 100, 100, Side::Buy, 32);

    //Assert
    ASSERT_NE(firstInt, nullptr);
    ASSERT_NE(secondInt, nullptr);
    ASSERT_NE(thirdOrder, nullptr);

    auto address = reinterpret_cast<std::uintptr_t>(firstInt);
    EXPECT_EQ(address % alignof(int), 0);

    address = reinterpret_cast<std::uintptr_t>(thirdOrder);
    EXPECT_EQ(address % alignof(BenchOrder), 0);
    
}


TEST(PoolTests, CanCreate){
    //Arrange
    pool<SlotSize::B32, 100> pool;

    //Act
    int* firstInt = pool.create<int>(42);
    int* secondInt = pool.create<int>(221);
    BenchOrder* thirdOrder = pool.create<BenchOrder>(123456, 150150, 100, 100, Side::Buy, 32);

    //Assert
    ASSERT_NE(firstInt, nullptr);
    ASSERT_NE(secondInt, nullptr);
    ASSERT_NE(thirdOrder, nullptr);
    ASSERT_NE(firstInt, secondInt);

    auto firstAdd = reinterpret_cast<std::uintptr_t>(firstInt);
    auto thirdAdd = reinterpret_cast<std::uintptr_t>(thirdOrder);
    ASSERT_NE(firstAdd, thirdAdd);

    EXPECT_EQ(*firstInt, 42);
    EXPECT_EQ(*secondInt, 221);
    EXPECT_EQ(thirdOrder->orderId, 123456);
    
    //Cleanup
    pool.destroy<int>(firstInt);
    pool.destroy<int>(secondInt);
    pool.destroy<BenchOrder>(thirdOrder);
}

TEST(PoolTests, IsAligned){
    //Arrange
    pool<SlotSize::B32, 100> pool;

    //Act
    int* firstInt = pool.create<int>(42);
    int* secondInt = pool.create<int>(221);
    BenchOrder* thirdOrder = pool.create<BenchOrder>(123456, 150150, 100, 100, Side::Buy, 32);

    //Assert
    ASSERT_NE(firstInt, nullptr);
    ASSERT_NE(secondInt, nullptr);
    ASSERT_NE(thirdOrder, nullptr);

    auto address = reinterpret_cast<std::uintptr_t>(firstInt);
    EXPECT_EQ(address % alignof(int), 0);

    address = reinterpret_cast<std::uintptr_t>(thirdOrder);
    EXPECT_EQ(address % alignof(BenchOrder), 0);
    
    //Cleanup
    pool.destroy<int>(firstInt);
    pool.destroy<int>(secondInt);
    pool.destroy<BenchOrder>(thirdOrder);
}

TEST(PoolTests, IsSlotReused){
    //Arrange
    pool<SlotSize::B32, 100> pool;

    //Act
    int* firstInt = pool.create<int>(42);
    int* secondInt = pool.create<int>(221);
    BenchOrder* thirdOrder = pool.create<BenchOrder>(123456, 150150, 100, 100, Side::Buy, 32);

    //Assert
    ASSERT_NE(firstInt, nullptr);
    ASSERT_NE(secondInt, nullptr);
    ASSERT_NE(thirdOrder, nullptr);

    int* oldSlot = secondInt;
    pool.destroy<int>(secondInt);

    ASSERT_EQ(secondInt, nullptr);

    int* reusedSlot = pool.create<int>(100);

    ASSERT_NE(reusedSlot, nullptr);
    ASSERT_EQ(reusedSlot, oldSlot);

    EXPECT_EQ(*reusedSlot, 100);
    
    
    //Cleanup
    pool.destroy<int>(firstInt);
    pool.destroy<int>(reusedSlot);
    pool.destroy<BenchOrder>(thirdOrder);
}

TEST(PoolTests, UsesAllSlots){
    pool<SlotSize::B32, 100> pool;
    int* ptrs[100];

    for(int i = 0; i<100;++i){
        ptrs[i] = pool.create<int>(100+i);
    }

    for(int i = 0; i<100;++i){
        ASSERT_NE(ptrs[i], nullptr);
        EXPECT_EQ(*ptrs[i], 100+i);
    }
}

TEST(PoolTests, OverflowsCleanly){
    pool<SlotSize::B32, 100> pool;
    int* ptrs[100];

    for(int i = 0; i<100;++i){
        ptrs[i] = pool.create<int>(100+i);
    }
    int* overflow = pool.create<int>(600);
    ASSERT_EQ(overflow, nullptr);
}