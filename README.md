# ConcurrentAlgorithms

### Implemented structures:
* [Treiber Stack](https://books.google.ru/books/about/Systems_Programming_Coping_with_Parallel.html)
* [MSQueue](https://www.cs.rochester.edu/~scott/papers/1996_PODC_queues.pdf?)
* Flat combining stack
  
### SMR schema(safe memory reclamation):
* [Hazard pointers](http://erdani.org/publications/cuj-2004-12.pdf)

Project contains some popular synchronization primitives implementation:
### Synchronization:
* [Mutex](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/blob/main/synchronize/Mutex.hpp)
* [Condition Variable](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/blob/main/synchronize/ConditionVariable.hpp)
* [Shared Mutex](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/blob/main/synchronize/SharedMutex.hpp)
* [Latch](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/blob/main/synchronize/Latch.hpp)
* [Cyclic Barierr](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/blob/main/synchronize/CyclicBarrier.hpp)
* [Semaphore](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/blob/main/synchronize/Semaphore.hpp)
* [Wait Group](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/blob/main/synchronize/wait_group/WaitGroup.hpp)
* [Feature/Promise](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/blob/main/synchronize/future/)

### Other:
* [ThreadPool](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/blob/main/synchronize/scheduler/)
* [Defer](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/blob/main/utils/)
* [Coroutine](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/tree/main/synchronize/coro)
* [Fibers](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/tree/main/synchronize/fiber)
* [Non-blocking socket operations for fibers](https://github.com/VTroyanGolovyan/ConcurrentAlgorithmsAndDS/tree/main/synchronize/fiber/io)

### Documentation
You can use doxygen to get docs.
```
doxygen Doxyfile
```

Usage examples:

### Echo Server
```cpp

synchronize::tp::ThreadPool scheduler{2};
fiber::IoService io(9000);
scheduler.Start();

fiber::Go(scheduler, [&] {
    while (true) {
        auto socket_ptr = io.Accept(); // fiber will be suspended, while asynchronous accept works
        fiber::Go(scheduler, [&wg, &io, socket_ptr]() mutable {
          while (true) {
              auto res = io.ReadSome(*socket_ptr); // fiber will be suspended, while asynchronous read works
              io.Write(*socket_ptr, res); // fiber will be suspended, while asynchronous write works
          }
        });
    }
});

io.RunService();
scheduler.Stop();
```

### Fibers

```cpp
synchronize::tp::ThreadPool scheduler{2};
scheduler.Start();

synchronize::WaitGroup wg;

std::atomic<int> x{0};
for (size_t i = 0; i < 400000; ++i) {
    wg.Add(1);
    fiber::Go(scheduler, [&wg, &scheduler, &x] {
        utils::Defer def([&wg]() { wg.Done(); });
        for (size_t j = 0; j < 100; ++j) {
            ++x;
            fiber::Yield();
        }
    });
}

wg.Wait();
std::cout << x.load();
scheduler.Stop();
```

### ThreadPool
```cpp
synchronize::tp::ThreadPool tp(4);
std::atomic<int> cnt = 0;
tp.Start();
synchronize::WaitGroup wg;

for (size_t i = 0; i < 1000; ++i) {
    wg.Add(1);
    tp.Submit(
        [&, k=i]() {
            utils::Defer def([&wg]() { wg.Done(); });
            for (size_t i = 0; i < 10000; ++i) {
                ++cnt;
            }
        }
    );
}

wg.Wait();
std::cout << cnt << std::endl;
tp.Stop();
std::cout << cnt << std::endl;
```
### Third-party:
* [GoogleTest](https://github.com/google/googletest) (used for Testing)
