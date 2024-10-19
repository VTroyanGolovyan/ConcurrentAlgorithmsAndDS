# ConcurrentAlgorithms

### Implemented structures:
* [Treiber Stack](https://books.google.ru/books/about/Systems_Programming_Coping_with_Parallel.html)
* [MSQueue](https://www.cs.rochester.edu/~scott/papers/1996_PODC_queues.pdf?)
* [Flat combining stack]() // TODO
  
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

### Documentation
You can use doxygen to get docs.
```
doxygen Doxyfile
```

Usage example:

```
#include <iostream>
#include "synchronize/scheduler/ThreadPool.hpp"
#include "synchronize/wait_group/WaitGroup.hpp"
#include "synchronize/wait_group/WaitGroup.hpp"
#include "synchronize/future/Future.hpp"

int main() {

    synchronize::tp::ThreadPool tp(4);
    std::atomic<int> cnt = 0;
    tp.Start();
    synchronize::Promise<int> promise;

    synchronize::Future<int> future = promise.GetFuture();

    synchronize::WaitGroup wg;
    for (size_t i = 0; i < 1000; ++i) {
        wg.Add(1);
        tp.Submit(
            [&, k=i]() {
                for (size_t i = 0; i < 10000; ++i) {
                    ++cnt;
                    if (k == 5 && i == 1999) {
                        promise.SetValue(i);
                    }
                }
                wg.Done();
            }
        );
    }

    std::cout << future.Get() << std::endl;

    wg.Wait();
    std::cout << cnt << std::endl;
    tp.Stop();
    std::cout << cnt << std::endl;

    return 0; 
}
```
### Third-party:
* [GoogleTest](https://github.com/google/googletest) (used for Testing)
