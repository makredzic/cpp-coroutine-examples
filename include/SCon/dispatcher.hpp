#pragma once
#include <BS/BS_thread_pool.hpp>

namespace SCon {

    class Dispatcher {

        private:
            BS::thread_pool<> threadPool_; 



        public:

            inline Dispatcher(size_t threadCount) : threadPool_{threadCount} {}
            Dispatcher() = delete;

            ~Dispatcher() {

                // BS::thread_pool's destructor automatically waits for tasks to complete
                // and then destroys the threads
            }

            template <typename T>
            inline std::future<T> submit_task() {

            }
            


    };

}