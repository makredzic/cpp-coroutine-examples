#include <iostream>
#include <coroutine>

#define TRACE() \
    std::cout << "---------\n[ENTERED]: " << __func__ << "\n---------\n" << std::endl;

class ReturnObject {

    public:
        struct promise_type {

            // 1. Called first when the coroutine is created
            ReturnObject get_return_object() { 
                TRACE();


                // Instantiates the parent object (ReturnObject) using the constructor we created below ("2.")
                return ReturnObject{std::coroutine_handle<promise_type>::from_promise(*this)}; 
            }

            // 3. Coroutine is constructed and initial_suspend fires
            // if ret type is:
            // suspend_always -> lazy coroutine (immediately suspended upon creation, must be explicitly started)
            // suspend_never -> eager coroutine (starts executing upon creation until suspension point)
            std::suspend_never initial_suspend() { 
                TRACE();
                return {}; // default constructor of the std::suspend_never class
            }

            // 6. coroutine executes this function body and after returning
            // suspends itself
            std::suspend_always yield_value(int val) {
                TRACE();
                val_= val;
                return {}; 
            }

            // 9. called after co_return is evaluated in the coroutine
            void return_value(int i) {
                TRACE();
                retValue_ = i;
            }

            // 10. called immediately after return_value
            // and the coroutine does co_await on the returned std::suspend_always{}
            // thus finally suspending the coroutine as its marked as done()
            
            // If this is returned std::suspend_never, the coroutine wouldn't be marked as done()
            // so in most cases, we want to always suspend here
            std::suspend_always final_suspend() noexcept { 
                TRACE();
                return std::suspend_always{};  // same as just {};
            }

            // Only in case of an uncaught exception in the coroutine
            // afterwhich final_suspend will be called
            void unhandled_exception() {
                TRACE();
            }

            int val_;
            int retValue_;
        };


    private:
        std::coroutine_handle<promise_type> handle_;

    public:

        // 2. A constructor we created that copies the coroutine handle and 
        // stores it in a member variable of this ReturnObject struct
        ReturnObject(std::coroutine_handle<promise_type> handle_): handle_{handle_} {TRACE();}

        auto getHandle() const { return handle_; }
        int getValue() const { return handle_.promise().val_; }

};

ReturnObject foo() {
    // 4. coroutine starts executing until suspended

    std::cout << "1. Hello from foo\n";

    // 5. equivalent to:
    // co_await promise_object.yield_value(expr)
        // which means that yield_value(21) will be called (look at "6.")
        // which returns std::suspend_always and we call co_await on it
        // which causes this point to suspend and returns the control
        // to the caller of foo()
    co_yield 21;

    // 8. coroutine is resumed from this point

    std::cout << "2. Hello again from foo\n";

    co_return 8;
}

int main() {

    TRACE();

    ReturnObject retObj = foo();

    //6. Foo was suspended and now we're back in main
    TRACE();

    const auto handle = retObj.getHandle();
    std::cout << "coroutine is done? = " << handle.done() << std::endl;

    // 7. Main thread resumes the coroutine and goes on
    // to execute its body where it left off
    handle.resume();

    // Main thread is back after the foo() coroutine suspended again
    TRACE();
    std::cout << "coroutine is done? = " << handle.done() << std::endl;

    return 0;
}