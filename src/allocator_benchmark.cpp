#include <benchmark/benchmark.h>
#include <ArenaFast.h>
#include <Pool.h>
#include <BenchStructs.h>

static void BM_Arena_1kCreate_Reset(benchmark::State& state) {
    //Setup here isnt measured
    arenaF<toSlot<BenchOrder>(), 1000> arena; 
    for (auto _ : state) {
        //Inside the loop is measured
        for(int i =0;i<1000;++i){
            BenchOrder* x = arena.create<BenchOrder>(i, i, 100, 100, Side::Buy, 32);
            benchmark::DoNotOptimize(x);
            benchmark::DoNotOptimize(*x);
        }
        
        benchmark::ClobberMemory();
        arena.reset();
    }
}

// Define another benchmark
static void BM_Pool_1kCreate(benchmark::State& state) {
  pool<toSlot<BenchOrder>(), 1000> pool;
  BenchOrder* orders[1000];
  for (auto _ : state)
  {
    for(int i =0;i<1000;++i){
        orders[i] = pool.create<BenchOrder>(i, i, 100, 100, Side::Buy, 32);
        benchmark::DoNotOptimize(orders[i]);
        benchmark::DoNotOptimize(*orders[i]);
    }
    state.PauseTiming();
    for(BenchOrder* order:orders){
        pool.destroy<BenchOrder>(order);
    }
    state.ResumeTiming();

    benchmark::ClobberMemory();

  }
}


// Define another benchmark
static void BM_Pool_1kCreate_1kDestroy(benchmark::State& state) {
  pool<toSlot<BenchOrder>(), 1000> pool;
  BenchOrder* orders[1000];
  for (auto _ : state)
  {
    for(int i =0;i<1000;++i){
        orders[i] = pool.create<BenchOrder>(i, i, 100, 100, Side::Buy, 32);
        benchmark::DoNotOptimize(orders[i]);
        benchmark::DoNotOptimize(*orders[i]);
    }

    for(BenchOrder* order:orders){
        pool.destroy<BenchOrder>(order);
    }


    benchmark::ClobberMemory();

  }
}


// Define another benchmark
static void BM_Pool_1kDestroy(benchmark::State& state) {
  pool<toSlot<BenchOrder>(), 1000> pool;
  BenchOrder* orders[1000];
  for (auto _ : state)
  {
    state.PauseTiming();
    for(int i =0;i<1000;++i){
        orders[i] = pool.create<BenchOrder>(i, i, 100, 100, Side::Buy, 32);
        benchmark::DoNotOptimize(orders[i]);
        benchmark::DoNotOptimize(*orders[i]);
    }
    state.ResumeTiming();
    for(BenchOrder* order:orders){
        pool.destroy<BenchOrder>(order);
    }

    benchmark::ClobberMemory();

  }
}

static void BM_1kNew(benchmark::State& state) {

  BenchOrder* orders[1000];
  for (auto _ : state)
  {
    for(int i =0;i<1000;++i){
        orders[i] = new BenchOrder(i, i, 100, 100, Side::Buy, 32);
        benchmark::DoNotOptimize(orders[i]);
        benchmark::DoNotOptimize(*orders[i]);
    }
    state.PauseTiming();
    for(BenchOrder* order:orders){
        delete order;
    }
    state.ResumeTiming();
    benchmark::ClobberMemory();

  }
}

static void BM_1kDelete(benchmark::State& state) {

  BenchOrder* orders[1000];
  for (auto _ : state)
  {
    state.PauseTiming();
    for(int i =0;i<1000;++i){
        orders[i] = new BenchOrder(i, i, 100, 100, Side::Buy, 32);
        benchmark::DoNotOptimize(orders[i]);
        benchmark::DoNotOptimize(*orders[i]);
    }
    state.ResumeTiming();
    for(BenchOrder* order:orders){
        delete order;
    }

    benchmark::ClobberMemory();

  }
}

static void BM_1kNew_1kDelete(benchmark::State& state) {

  BenchOrder* orders[1000];
  for (auto _ : state)
  {
    for(int i =0;i<1000;++i){
        orders[i] = new BenchOrder(i, i, 100, 100, Side::Buy, 32);
        benchmark::DoNotOptimize(orders[i]);
        benchmark::DoNotOptimize(*orders[i]);
    }

    for(BenchOrder* order:orders){
        delete order;
    }

    benchmark::ClobberMemory();

  }
}

BENCHMARK(BM_Arena_1kCreate_Reset);

BENCHMARK(BM_Pool_1kCreate);

BENCHMARK(BM_Pool_1kDestroy);

BENCHMARK(BM_Pool_1kCreate_1kDestroy);

BENCHMARK(BM_1kNew);

BENCHMARK(BM_1kDelete);

BENCHMARK(BM_1kNew_1kDelete);

BENCHMARK_MAIN();