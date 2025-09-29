#pragma once
#include <functional>
#include <map>
#include <memory>
#include <random>

template<typename... ARGS>
class Publisher;

template<typename... ARGS>
class Subscription {
public:
    Subscription() = default;

    Subscription(const Subscription& other) = delete;
    Subscription& operator=(const Subscription& other) = delete;

    Subscription(std::shared_ptr<Publisher<ARGS...>> publisher, int id) {
        this->publisher_ = publisher;
        this->id_ = id;
    }

    Subscription(Subscription&& other) noexcept {
        moveFrom(std::move(other));
    }

    Subscription& operator=(Subscription&& other) noexcept {
        if (this != &other) {
            cleanup();
            moveFrom(std::move(other));
        }

        return *this;
    }

    void setForever() {
        publisher_.reset();
    }

    ~Subscription() {
        cleanup();
    }

private:
    std::weak_ptr<Publisher<ARGS...>> publisher_;
    int id_ = 0;

    friend class Publisher<ARGS...>;

    void cleanup() {
        if (auto publisher = publisher_.lock())
            publisher->unSubscribe(this);

        publisher_.reset();
    }

    void moveFrom(Subscription&& other) {
        publisher_ = std::move(other.publisher_);
        id_ = other.id_;
        other.publisher_.reset();
    }
};

template<typename... ARGS>
class Publisher : public std::enable_shared_from_this<Publisher<ARGS...>> {
public:
    static std::shared_ptr<Publisher<ARGS...>> create() {
        // ReSharper disable once CppSmartPointerVsMakeFunction
        return std::shared_ptr<Publisher>(new Publisher());
    }

    void invoke(ARGS... args) {
        // make a copy in case the event does something that causes a subscription to destruct
        for (auto functionsCopy = functions_; auto& func : functionsCopy) {
            func.second(std::forward<ARGS>(args)...);
        }
    }

    Subscription<ARGS...> subscribe(std::function<void(ARGS...)> function) {
        Subscription<ARGS...> subscription(this->shared_from_this(), curKey_);

        functions_[curKey_] = function;
        curKey_++;

        return subscription;
    }

    template<typename T>
    Subscription<ARGS...> subscribe(T* instance, void (T::*method)(ARGS...)) {
        Subscription<ARGS...> subscription(this->shared_from_this(), curKey_);

        functions_[curKey_] = [instance, method](ARGS... args) {
            (instance->*method)(std::forward<ARGS>(args)...);
        };
        curKey_++;

        return std::move(subscription);
    }

    template<typename T>
    Subscription<ARGS...> subscribe(T* instance, void (T::*method)(ARGS...) const) {
        Subscription<ARGS...> subscription(this->shared_from_this(), curKey_);

        functions_[curKey_] = [instance, method](ARGS... args) {
            (instance->*method)(std::forward<ARGS>(args)...);
        };
        curKey_++;

        return std::move(subscription);
    }

    void unSubscribe(Subscription<ARGS...>* subscription) {
        if (const auto it = functions_.find(subscription->id_); it != functions_.end()) {
            functions_.erase(it);
        }
    }

private:
    std::map<int, std::function<void(ARGS...)>> functions_;
    int curKey_ = 0;

    Publisher() = default;
};
