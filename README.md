
A small C++17 project implementing custom bump/arena and pool memory allocators.

The project currently includes a fixed size Arena Allocator and fixed size Pool allocator both optimised for low overhead allocation speed over memory efficiency. This means that memory can be wasted when the object is smaller than the slot size of the allocator.

The goal of this project is to explore and understand memory allocation and what changes and optimisation must be undertaken to increase speed of said allocators.

Features:
    -   Slot sized determined via SlotSize enum exposed on type creation.
    -   Placement new object construction.
    -   When constructed with a slot size of 64 or less the allocator is aligned to 64 as to increase cache friendliness.
    -   Clean overflow handling by returning nullptr.
    -   Google unit tests for both ArenaFast and Pool.
    -   Google Benchmark Tests for both ArenaFast and Pool.
    -   CMake based setup.

## Allocators:

### ArenaFast

ArenaFast is a remake of my original Arena Allocator.
In the original each create<T>(Args) would align the current pointer to the alignof(T) which i found to be very inefficient.
Using a slot based approach the ArenaFast allocator is able to achieve a much quicker time than the previous arena.

It allocates objects from a pre-allocated memory block and advances through the memory as objects are created.
It does not support freeing individual objects one by one.
It does not support non trivially destrucible types as a result.
The arena is reset which allows it to start allocating form the start of the memory block however it doesnt invalidate previously given pointers as to keep the speed fast and reduce overhead.

Use case for this is when many objects need to be allocated and they all have the same fixed life time. for exmple per-frame data.

Example:

```cpp
arenaF<SlotSize::B32, 100> arena;
int* x = arena.create<int>(200);
arena.reset();
```



### Pool

Pool is a fixed size slot allocator which uses a LIFO style free list.

Pool also only supports trivially destructible types as to keep overhead low.

It manages a block of equally sized slots. Each allocation takes the first slot available in the list.
When an object is destroyed it's slot is returned to the front of the free list ready to be re-used.

Unlike ArenaFast, pool supports individual object destruction which returns the slot to the free list and invalidates the given pointer so that it can no longer be used.

This makes the pool useful for objects where the lifetime differs as they can be individually destroyed.

Example:

```cpp
pool<SlotSize::B32, 100> pool;
int* ptr = pool.create<int>(100);
pool.destroy<int>(ptr);
```



## Tests

GoogleTest is used to check basic allocator correctness.

Covered:

- Object creation
- Slot capacity
- Overflow returning `nullptr`
- Alignment
- Arena reset behaviour
- Pool slot reuse

Current tests:

```text
ArenaTests.CanCreate
ArenaTests.UsesAllSlots
ArenaTests.OverflowsCleanly
ArenaTests.CanReset
ArenaTests.IsAligned

PoolTests.CanCreate
PoolTests.IsAligned
PoolTests.IsSlotReused
PoolTests.UsesAllSlots
PoolTests.OverflowsCleanly
```

## Benchmarks

Google Benchmark is used to compare allocator performance.

Current benchmarks cover:

- Arena create/reset
- Pool create
- Pool destroy
- Pool create + destroy
- new 
- delete
- new + delete

Example local result:

```text
BM_Arena_1kCreate_Reset             ~926    ns
BM_Pool_1kCreate                    ~1088   ns
BM_Pool_1kDestroy                   ~675    ns
BM_Pool_1kCreate_1kDestroy          ~1055   ns
BM_1kNew                            ~6985   ns
BM_1kDelete                         ~10974  ns
BM_1kNew_1kDelete                   ~14183  ns
```

Using these results we can Approximate the following times for each operation:

```text
BM_Arena_1kCreate_Reset             ~0.93  ns
BM_Pool_1kCreate                    ~1.09  ns
BM_Pool_1kDestroy                   ~0.68  ns
BM_Pool_1kCreate_1kDestroy          ~1.06  ns
BM_1kNew                            ~6.99  ns
BM_1kDelete                         ~10.98 ns
BM_1kNew_1kDelete                   ~14.18 ns
```

Results are synthetic and mainly useful for comparing allocator changes inside this project.

## Current limitations

- `ArenaFast` does not invalidate old pointers after reset.
- `Pool` only supports trivial types and doesnt call destructors on destroy<T>(T* &ptr);
- The allocators are not thread-safe.
- Benchmarks are synthetic and not a full real-world allocation workload.


## Roadmap

- Add more realistic benchmark workloads.
- Improve documentation of allocator internals.
- Add CI for automated test runs.
- Use the allocators in a future order book / matching engine project.