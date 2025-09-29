#include <iostream>
#include "../systems/betterEvents.h"

class Subscriber {
public:
    void setValue(const int value) {
        this->value = value;
    }

    void printValue() const {
        std::cout << "Event called on const member function test passed: " << value << std::endl;
    }

    int value = 0;
};

int main() {
    {
        int x = 0;

        std::shared_ptr<Publisher<int>> publisher = Publisher<int>::create();
        Subscription<int> subscription = publisher->subscribe([&x](int y) {
            x = y;
        });

        publisher->invoke(5);

        if (x != 5)
            std::cerr << "Event called test failed" << std::endl;
        else
            std::cout << "Event called test passed" << std::endl;
    }

    {
        std::shared_ptr<Publisher<int>> publisher = Publisher<int>::create();
        Subscriber* subscriber = new Subscriber();

        Subscription<int> subscription = publisher->subscribe(subscriber, &Subscriber::setValue);

        publisher->invoke(25);

        if (subscriber->value != 25)
            std::cerr << "Event called on non-const member function test failed" << std::endl;
        else
            std::cout << "Event called on non-const member function test passed" << std::endl;

        delete subscriber;

        publisher->invoke(5);

        std::cout << "Subscriber deletion test passed" << std::endl;
    }

    if constexpr (true) {
        {
            std::shared_ptr<Publisher<>> publisher = Publisher<>::create();
            Subscriber subscriber;

            Subscription<> subscription = publisher->subscribe(&subscriber, &Subscriber::printValue);

            publisher->invoke();
        }

        std::cout << "Publisher deletion test passed" << std::endl;
    }

    {
        int x = 0;

        std::shared_ptr<Publisher<int&>> publisher = Publisher<int&>::create();

        {
            publisher->subscribe([](int& y) {
                y = 5;
            }).setForever();

            publisher->invoke(x);
        }

        if (x != 5)
            std::cerr << "Pass by reference test failed" << std::endl;
        else
            std::cout << "Pass by reference test passed" << std::endl;

        x = 10;

        publisher->invoke(x);

        if (x != 5)
            std::cerr << "Forever subscriber test failed" << std::endl;
        else
            std::cout << "Forever subscriber test passed" << std::endl;
    }
}
