#pragma once
#include <functional>
#include <random>

template<typename... ARGS>
class EventDispatcher;

template<typename... ARGS>
class EventListener {
public:
    EventListener(EventDispatcher<ARGS...>* dispatcher, std::function<void(ARGS...)> function) {
        this->dispatcher_ = dispatcher;
        setFunction(function);
        dispatcher->add(this);
    }

    template<typename T>
    EventListener(EventDispatcher<ARGS...>* dispatcher, T* instance, void (T::*method)(ARGS...)) {
        this->dispatcher_ = dispatcher;
        setFunction(instance, method);
        dispatcher->add(this);
    }

    void setFunction(std::function<void(ARGS...)> function) {
        this->function_ = function;

        auto randomNumberGenerator = std::mt19937_64(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::uniform_int_distribution dist(1, 100);

        this->id_ = dist(randomNumberGenerator);
    }

    template<typename T>
    void setFunction(T* instance, void (T::*method)(ARGS...)) {
        this->function_ = [instance, method](ARGS... args) {
            (instance->*method)(args...);
        };
    }

    void invoke(ARGS... args) {
        function_(std::forward<ARGS>(args)...);
    }

    friend bool operator==(const EventListener& a, const EventListener& b) {
        return a.id_ == b.id_;
    }

    ~EventListener() {
        dispatcher_->remove(this);
    }

private:
    EventDispatcher<ARGS...>* dispatcher_;
    std::function<void(ARGS...)> function_;
    int id_ = 0;
};

template<typename... ARGS>
class EventDispatcher {
public:
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

    void invoke(ARGS... args) {
        for (auto* listener : listeners_) {
            listener->invoke(std::forward<ARGS>(args)...);
        }
    }

private:
    std::vector<EventListener<ARGS...>*> listeners_;
};
