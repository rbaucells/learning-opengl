#pragma once
#include <functional>

template<typename... Args>
class Event {
private:
    using Handler = std::function<void(Args...)>;
    std::vector<Handler> handlers;
public:
    void likeAndSubscribe(const Handler& handler) {
        handlers.push_back(handler);
    };

    template<class T>
    void likeAndSubscribe(T* instance, void (T::*method)(Args...)) {
        handlers.push_back([=](Args... args) {
            (instance->*method)(args...);
        });
        // push back the function signature from a class instance
    }

    void invoke(Args... args) {
        for (auto& handler : handlers) {
            handler(args...); // invoke the method with the proper arguments
        }
    }
};
