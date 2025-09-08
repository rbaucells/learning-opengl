#pragma once
#include <functional>

template<typename... ARGS>
class Event {
private:
    using Handler = std::function<void(ARGS...)>;
    std::vector<Handler> handlers;
public:
    void subscribe(const Handler& functionHandler) {
        handlers.push_back(functionHandler);
    }
    template<class T>
    void subscribe(T *instance, void (T::*method)(ARGS...) const) {
        // push back the function signature from a class instance
        handlers.push_back([instance, method](ARGS... args) {
            (instance->*method)(args...);
        });
    }

    template<class T>
    void subscribe(T *instance, void (T::*method)(ARGS...)) {
        // push back the function signature from a class instance
        handlers.push_back([instance, method](ARGS... args) {
            (instance->*method)(args...);
        });
    }

    template<class T>
    void unSubscribe(T *instance, void (T::*method)(ARGS...) const) {
        // remove the function siganture from the handlers
        std::erase(handlers, [instance, method](ARGS... args) {
            (instance->*method)(args...);
        });
    }

    template<class T>
    void unSubscribe(T *instance, void (T::*method)(ARGS...)) {
        // remove the function siganture from the handlers
        std::erase(handlers, [instance, method](ARGS... args) {
            (instance->*method)(args...);
        });
    }

    void invoke(ARGS... args) {
        if (handlers.empty())
            return;

        for (auto &handler: handlers) {
            handler(args...); // invoke the method with the proper arguments
        }
    }
};
