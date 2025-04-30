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

    // !!!! NOTE !!!!
    // The coroutine is FULLY SUSPENDED right before entering this function
    void await_suspend(std::coroutine_handle<> _) const noexcept { TRACE(); }

    // Called when the coroutine is being resumed
    void await_resume() const noexcept { TRACE(); }
};

// The literal implementation of std::suspend_never
struct SuspendNever {
    bool await_ready() const noexcept {
        TRACE(); // added here for demonstration purposes
        return true;
    }

    // WONT EVER BE CALLED SINCE await_ready is always TRUE
    // therefore, no suspension ever happens here
    void await_suspend(std::coroutine_handle<> _) const noexcept { TRACE(); }

    // Called immediately after await_ready 
    void await_resume() const noexcept { TRACE(); }
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

    // if this replaces the line above, coroutine must not be resumed by handle.resume() below
    // because it would have finished already by then
    // since this won't suspend it and it will just continue
    // calling resume on a finished coroutine will result in a seg fault
    // co_await SuspendNever{};

    std::cout << "2. Hello again from foo\n";

}

int main() {

    TRACE();

    ReturnObject retObj = foo();

    // Foo was suspended and now we're back in main
    TRACE();

    const auto handle = retObj.getHandle();
    std::cout << "coroutine is done? = " << handle.done() << std::endl;

    // Resume the coroutine (main thread goes on to execute the coroutine where it left off)
    handle.resume(); // <--- comment out if SuspendNever is used above instead of SuspendAlways


    // Main thread is back after another suspension
    TRACE();
    std::cout << "coroutine is done? = " << handle.done() << std::endl;

    return 0;
}