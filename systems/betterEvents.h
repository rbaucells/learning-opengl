#pragma once
#include <functional>
#include <random>

template<typename... ARGS>
class EventDispatcher;

template<typename... ARGS>
class EventListener {
public:
    EventListener() = default;

    EventListener(EventDispatcher<ARGS...>* dispatcher, std::function<void(ARGS...)> func) {
        bind(dispatcher, func);
    }

    template<typename T>
    EventListener(EventDispatcher<ARGS...>* dispatcher, T* instance, void (T::*method)(ARGS...)) {
        bind(dispatcher, instance, method);
    }

    template<typename T>
    EventListener(EventDispatcher<ARGS...>* dispatcher, T* instance, void (T::*method)(ARGS...) const) {
        bind(dispatcher, instance, method);
    }

    void bind(EventDispatcher<ARGS...>* dispatcher, std::function<void(ARGS...)> func) {
        unbind();

        dispatcher_ = dispatcher;
        function_ = func;
        dispatcher_->add(this);

        printf("Event Binded\n");
    }

    template<typename T>
    void bind(EventDispatcher<ARGS...>* dispatcher, T* instance, void (T::*method)(ARGS...)) {
        unbind();

        dispatcher_ = dispatcher;

        function_ = [instance, method](ARGS... args) {
            (instance->*method)(args...);
        };

        dispatcher_->add(this);

        printf("Event Binded\n");
    }

    template<typename T>
    void bind(EventDispatcher<ARGS...>* dispatcher, T* instance, void (T::*method)(ARGS...) const) {
        unbind();

        dispatcher_ = dispatcher;

        function_ = [instance, method](ARGS... args) {
            (instance->*method)(args...);
        };

        dispatcher_->add(this);

        printf("Event Binded\n");
    }

    void unbind() {
        if (dispatcher_) {
            dispatcher_->remove(this);
            dispatcher_ = nullptr;

            printf("Event UnBinded\n");
        }
    }

    EventListener(EventListener& other) = delete;
    EventListener& operator=(const EventListener&) = delete;

    bool operator==(const EventListener& b) const {
        return this == &b;
    }

    ~EventListener() {
        unbind();
    }

private:
    EventDispatcher<ARGS...>* dispatcher_;
    std::function<void(ARGS...)> function_;

    friend class EventDispatcher<ARGS...>;

    void invoke(ARGS... args) {
        if (function_)
            function_(std::forward<ARGS>(args)...);
    }
};

template<typename... ARGS>
class EventDispatcher {
public:
    void invoke(ARGS... args) {
        for (auto* listener : listeners_) {
            listener->invoke(std::forward<ARGS>(args)...);
        }
    }

private:
    std::vector<EventListener<ARGS...>*> listeners_;

    void add(EventListener<ARGS...>* listenerToAdd) {
        listeners_.push_back(listenerToAdd);
    }

    void remove(EventListener<ARGS...>* listenerToRemove) {
        for (auto it = listeners_.begin(); it != listeners_.end();) {
            if (**it == *listenerToRemove) {
                listeners_.erase(it);
                return;
            }

            ++it;
        }
    }

    friend class EventListener<ARGS...>;
};
