#include <iostream>
#include <coroutine>

#define TRACE() \
    std::cout << "---------\n[ENTERED]: " << __func__ << "\n---------\n" << std::endl;


// The literal implementation of std::suspend_always
struct SuspendAlways {
    bool await_ready() const noexcept {
        TRACE(); // added here for demonstration purposes
        return false;
    }
    constexpr void await_suspend(std::coroutine_handle<> _) const noexcept { }
    constexpr void await_resume() const noexcept { }
};

// The literal implementation of std::suspend_never
struct SuspendNever {
    bool await_ready() const noexcept {
        TRACE(); // added here for demonstration purposes
        return true;
    }
    constexpr void await_suspend(std::coroutine_handle<> _) const noexcept { }
    constexpr void await_resume() const noexcept { }
};

class ReturnObject {
    public:
        struct promise_type {

            ReturnObject get_return_object() { 
                TRACE();

                // Instantiates the parent object (ReturnObject) using the constructor we created below ("2.")
                return ReturnObject{std::coroutine_handle<promise_type>::from_promise(*this)}; 
            }

            std::suspend_never initial_suspend() { 
                TRACE();
                return {}; // default constructor of the std::suspend_never class
            }

            void return_void() {
                TRACE();
            }

            std::suspend_always final_suspend() noexcept { 
                TRACE();
                return std::suspend_always{};  // same as just {};
            }

            void unhandled_exception() {
                TRACE();
            }
        };

    private:
        std::coroutine_handle<promise_type> handle_;

    public:
        ReturnObject(std::coroutine_handle<promise_type> handle_): handle_{handle_} {TRACE();}
        auto getHandle() const { return handle_; }

};

ReturnObject foo() {
    std::cout << "1. Hello from foo\n";

    // SUSPEND
    co_await SuspendAlways{};

    std::cout << "2. Hello again from foo\n";

}

int main() {

    TRACE();

    ReturnObject retObj = foo();

    //6. Foo was suspended and now we're back in main
    TRACE();

    const auto handle = retObj.getHandle();
    std::cout << "coroutine is done? = " << handle.done() << std::endl;

    // 7. resume the coroutine
    handle.resume();

    TRACE();
    std::cout << "coroutine is done? = " << handle.done() << std::endl;

    return 0;
}