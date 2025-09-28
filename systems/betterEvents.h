#pragma once
#include <functional>
#include <map>
#include <random>

template<typename... ARGS>
class Publisher;

template<typename... ARGS>
class Subscription {
public:
    Subscription(Publisher<ARGS...>* publisher, int id) {
        this->publisher_ = publisher;
        this->id_ = id;
    }

    Subscription(Subscription&& other) noexcept {
        this->publisher_ = other.publisher_;
        other.publisher_ = nullptr;

        this->id_ = other.id_;
    }

    ~Subscription() {
        this->publisher_->unSubscribe(this);
    }

private:
    Publisher<ARGS...>* publisher_ = nullptr;
    int id_;
};

template<typename... ARGS>
class Publisher {
public:
    void invoke(ARGS... args) {
        for (auto func : funcitons_) {
            func(std::forward<ARGS>(args)...);
        }
    }

    Subscription<ARGS...> subscribe(std::function<void(ARGS...)> function) {
        Subscription<ARGS...> subscription(this, curKey_);

        funcitons_.insert(curKey_, function);
        curKey_++;

        return std::move(subscription);
    }

    void unSubscribe(Subscription<ARGS...>* subscription) {
        if (const auto it = funcitons_.find(subscription->id_); it != funcitons_.end()) {
            funcitons_.erase(it);
        }
    }

private:
    std::map<int, std::function<void(ARGS...)>> funcitons_;
    int curKey_ = 0;

    friend class Subscription<ARGS...>;
};
