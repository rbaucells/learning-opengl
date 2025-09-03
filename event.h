#pragma once
#include <functional>

template<typename... Args>
class Event {
private:
    using handler = std::function<void(Args...)>;
    std::vector<handler> handlers;

public:
    void subscribe(const handler& functionHandler) {
        handlers.push_back(functionHandler);
    }
    template<class T>
    void subscribe(T *instance, void (T::*method)(Args...) const) {
        // push back the function signature from a class instance
        handlers.push_back([instance, method](Args... args) {
            (instance->*method)(args...);
        });
    }

    template<class T>
    void subscribe(T *instance, void (T::*method)(Args...)) {
        // push back the function signature from a class instance
        handlers.push_back([instance, method](Args... args) {
            (instance->*method)(args...);
        });
    }

    template<class T>
    void unSubscribe(T *instance, void (T::*method)(Args...) const) {
        // remove the function siganture from the handlers
        std::erase(handlers, [instance, method](Args... args) {
            (instance->*method)(args...);
        });
    }

    template<class T>
    void unSubscribe(T *instance, void (T::*method)(Args...)) {
        // remove the function siganture from the handlers
        std::erase(handlers, [instance, method](Args... args) {
            (instance->*method)(args...);
        });
    }

    void invoke(Args... args) {
        if (handlers.empty())
            return;

        for (auto &handler: handlers) {
            handler(args...); // invoke the method with the proper arguments
        }
    }
};
