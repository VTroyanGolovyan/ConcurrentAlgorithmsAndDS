#include "../synchronize/Mutex.hpp"

TEST(TestMutex, CounterLotWriters) {
    for (int j = 2; j < 10; j += 2) {
        synchronize::Mutex m;
        int x = 0;
        std::vector<std::thread> ths;
        for (int i = 0; i < j; ++i) {
            ths.emplace_back([&]() {
                for (size_t i = 0; i < 100000; ++i) {
                    m.lock();
                    ++x;
                    m.unlock();
                }
            });
        }
        for (auto& th : ths) {
            th.join();
        }
        ASSERT_EQ(x, j * 100000);
    }
}
