#include <iostream>
#include <coroutine>

struct ReturnObject {

    struct promise_type {
        ReturnObject get_return_object() { return ReturnObject{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };

    std::coroutine_handle<> handle;
    ReturnObject(std::coroutine_handle<> handle_): handle{handle_} {}

};


ReturnObject foo() {
    std::cout << "1. Hello from foo\n";

    co_await std::suspend_always{};

    std::cout << "2. Hello again from foo\n";
}



int main() {

    ReturnObject retObj = foo();
    std::cout << "coroutine is done? = " << retObj.handle.done() << std::endl;

    retObj.handle.resume();
    std::cout << "coroutine is done? = " << retObj.handle.done() << std::endl;

    // retObj.handle.resume();
    // std::cout << "coroutine is done? = " << retObj.handle.done() << std::endl;
    return 0;
}