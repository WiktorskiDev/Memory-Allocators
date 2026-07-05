#include "Arena.h"
#include "ArenaFast.h"
#include "Pool.h"
#include <iostream>

template<typename Allocator>
void testArenaAllocatorAdd(Allocator& allocator, std::vector<std::pair<std::size_t, double>>& perfTimes)
{

    std::size_t iterations = 100; //Amount of operations to do at once
    std::size_t itMax = 10000;
    for(; iterations <= itMax; iterations*=10)
    {
        double totalAvgNS = 0.0;
        allocator.reset();
        for(std::size_t it = 0; it<iterations; it++)
        {
            //Setup in this part, time isnt measured
            std::vector<int*> ptrs;
            ptrs.resize(allocator.getCapacity());
            const auto start{std::chrono::steady_clock::now()};
            //Everything ran inside the loop is measured
            for(std::size_t x = 0; x<allocator.getCapacity(); x++)
            {
                ptrs[x] = (allocator.template create<int>(x));
            }
            const auto end{std::chrono::steady_clock::now()};
            //Everythign after this isnt measured
            allocator.reset();
            auto elapsed = end-start;
            std::int64_t nsTaken =  std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
            double avgNSPerAlloc = static_cast<double>(nsTaken)/allocator.getCapacity();
            totalAvgNS += avgNSPerAlloc;

        }

        perfTimes.emplace_back(std::make_pair(iterations, totalAvgNS)); //Stores into a vector that sotres pairs of size_t for iteration amount and double for ttoal NS taken amount. 

        
    }
}

template<typename Allocator>
void testArenaAllocatorAddDel(Allocator& allocator, std::vector<std::pair<std::size_t, double>>& perfTimes)
{

    std::size_t iterations = 100; //Amount of operations to do at once
    std::size_t itMax = 10000;
    for(; iterations <= itMax; iterations*=10)
    {
        double totalAvgNS = 0.0;
        allocator.reset();
        for(std::size_t it = 0; it<iterations; it++)
        {
            //Setup in this part, time isnt measured
            std::vector<int*> ptrs;
            ptrs.resize(allocator.getCapacity());
            const auto start{std::chrono::steady_clock::now()};
            //Everything ran inside the loop is measured
            for(std::size_t x = 0; x<allocator.getCapacity(); x++)
            {
                ptrs[x] = (allocator.template create<int>(x));
            }
            allocator.reset();
            const auto end{std::chrono::steady_clock::now()};
            //Everythign after this isnt measured

            auto elapsed = end-start;
            std::int64_t nsTaken =  std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
            double avgNSPerAlloc = static_cast<double>(nsTaken)/allocator.getCapacity();
            totalAvgNS += avgNSPerAlloc;

        }

        perfTimes.emplace_back(std::make_pair(iterations, totalAvgNS)); //Stores into a vector that sotres pairs of size_t for iteration amount and double for ttoal NS taken amount. 

        
    }
}

void testPoolAllocatorAdd(std::vector<std::pair<std::size_t, double>>& perfTimes)
{

    std::size_t iterations = 100; //Amount of operations to do at once
    std::size_t itMax = 10000;
    for(; iterations <= itMax; iterations*=10)
    {
        double totalAvgNS = 0.0;
        for(std::size_t it = 0; it<iterations; it++)
        {
            //Setup in this part, time isnt measured
            std::vector<int*> ptrs;
            pool<SlotSize::B16, 1024> pool;
            ptrs.resize(pool.getCapacity());
            const auto start{std::chrono::steady_clock::now()};
            //Everything ran inside the loop is measured
            for(std::size_t x = 0; x<pool.getCapacity(); x++)
            {
                ptrs[x] = (pool.template create<int>(x));
            }
            const auto end{std::chrono::steady_clock::now()};
            //Everythign after this isnt measured
            auto elapsed = end-start;
            std::int64_t nsTaken =  std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
            double avgNSPerAlloc = static_cast<double>(nsTaken)/pool.getCapacity();
            totalAvgNS += avgNSPerAlloc;

        }

        perfTimes.emplace_back(std::make_pair(iterations, totalAvgNS)); //Stores into a vector that sotres pairs of size_t for iteration amount and double for ttoal NS taken amount. 
        
    }
}


void testPoolAllocatorAddDel(std::vector<std::pair<std::size_t, double>>& perfTimes)
{

    std::size_t iterations = 100; //Amount of operations to do at once
    std::size_t itMax = 10000;
    for(; iterations <= itMax; iterations*=10)
    {
        double totalAvgNS = 0.0;
        for(std::size_t it = 0; it<iterations; it++)
        {
            //Setup in this part, time isnt measured
            std::vector<int*> ptrs;
            pool<SlotSize::B16, 1024> pool;
            ptrs.resize(pool.getCapacity());
            const auto start{std::chrono::steady_clock::now()};
            //Everything ran inside the loop is measured
            for(std::size_t x = 0; x<pool.getCapacity(); x++)
            {
                ptrs[x] = (pool.template create<int>(x));
            }
            for(std::size_t x = 0; x<pool.getCapacity(); x++)
            {
                pool.destroy<int>(ptrs[x]);
            }
            const auto end{std::chrono::steady_clock::now()};
            //Everythign after this isnt measured
            auto elapsed = end-start;
            std::int64_t nsTaken =  std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
            double avgNSPerAlloc = static_cast<double>(nsTaken)/(pool.getCapacity()*2);
            totalAvgNS += avgNSPerAlloc;

        }

        perfTimes.emplace_back(std::make_pair(iterations, totalAvgNS)); //Stores into a vector that sotres pairs of size_t for iteration amount and double for ttoal NS taken amount. 
        
    }
}

void testNewAllocatorAdd(std::vector<std::pair<std::size_t, double>>& perfTimes)
{

    std::size_t iterations = 100; //Amount of operations to do at once
    std::size_t itMax = 10000;
    for(; iterations <= itMax; iterations*=10)
    {
        double totalAvgNS = 0.0;
        for(std::size_t it = 0; it<iterations; it++)
        {
            //Setup in this part, time isnt measured
            int* ptrs[1024];
            const auto start{std::chrono::steady_clock::now()};
            //Everything ran inside the loop is measured
            for(std::size_t x = 0; x<1024; x++)
            {
                ptrs[x] = new int(static_cast<int>(x));
            }
            const auto end{std::chrono::steady_clock::now()};
            //Everythign after this isnt measured

            for(std::size_t x = 0; x<1024; x++)
            {
                delete ptrs[x];
            }
            auto elapsed = end-start;
            std::int64_t nsTaken =  std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
            double avgNSPerAlloc = static_cast<double>(nsTaken)/1024;
            totalAvgNS += avgNSPerAlloc;

        }

        perfTimes.emplace_back(std::make_pair(iterations, totalAvgNS)); //Stores into a vector that sotres pairs of size_t for iteration amount and double for ttoal NS taken amount. 
        
    }
}

void testNewAllocatorAddDel(std::vector<std::pair<std::size_t, double>>& perfTimes)
{

    std::size_t iterations = 100; //Amount of operations to do at once
    std::size_t itMax = 10000;
    for(; iterations <= itMax; iterations*=10)
    {
        double totalAvgNS = 0.0;
        for(std::size_t it = 0; it<iterations; it++)
        {
            //Setup in this part, time isnt measured
            int* ptrs[1024];
            const auto start{std::chrono::steady_clock::now()};
            //Everything ran inside the loop is measured
            for(std::size_t x = 0; x<1024; x++)
            {
                ptrs[x] = new int(static_cast<int>(x));
            }
            for(std::size_t x = 0; x<1024; x++)
            {
                delete ptrs[x];
            }
            const auto end{std::chrono::steady_clock::now()};
            //Everythign after this isnt measured


            auto elapsed = end-start;
            std::int64_t nsTaken =  std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
            double avgNSPerAlloc = static_cast<double>(nsTaken)/(1024*2);
            totalAvgNS += avgNSPerAlloc;

        }

        perfTimes.emplace_back(std::make_pair(iterations, totalAvgNS)); //Stores into a vector that sotres pairs of size_t for iteration amount and double for ttoal NS taken amount. 
        
    }
}

int main() {


    arenaF<SlotSize::B16, 1024> arena;

    std::vector<std::pair<std::size_t, double>> aTimes;

    std::vector<std::pair<std::size_t, double>> pTimes;

    std::vector<std::pair<std::size_t, double>> nTimes;

    std::cout << "Starting Add/Del Tests" << '\n';

    testArenaAllocatorAddDel(arena, aTimes);

    testPoolAllocatorAddDel(pTimes);

    testNewAllocatorAddDel(nTimes);

    std::cout << "Arena Fast Allocator Times:" << '\n';
    std::cout << "100 Iterations ns Average: " << aTimes[0].second/aTimes[0].first << '\n';
    std::cout << "1000 Iterations ns Average: " << aTimes[1].second/aTimes[1].first << '\n';
    std::cout << "10000 Iterations ns Average: " << aTimes[2].second/aTimes[2].first << '\n';

    std::cout << "Pool Allocator Times:" << '\n';
    std::cout << "100 Iterations ns Average: " << pTimes[0].second/pTimes[0].first << '\n';
    std::cout << "1000 Iterations ns Average: " << pTimes[1].second/pTimes[1].first << '\n';
    std::cout << "10000 Iterations ns Average: " << pTimes[2].second/pTimes[2].first << '\n';

    std::cout << "Heap New Times:" << '\n';
    std::cout << "100 Iterations ns Average: " << nTimes[0].second/nTimes[0].first << '\n';
    std::cout << "1000 Iterations ns Average: " << nTimes[1].second/nTimes[1].first << '\n';
    std::cout << "10000 Iterations ns Average: " << nTimes[2].second/nTimes[2].first << '\n';
    return 0;
}


