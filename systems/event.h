// #pragma once
// #include <functional>
// #include <map>
// #include <vector>

// template<typename... ARGS>
// class Event {
// private:
//     std::map<int, std::function<void(ARGS...)>> handlers_;
//     int currentKey_ = 0;
//
// public:
//     int subscribe(const std::function<void(ARGS...)>& functionHandler) {
//         // Adds functionHandler into the handlers_,
//         // increments the index after its value is read
//         // then returns the place where its at AKA the old currentKey
//         return handlers_.insert(currentKey_++, functionHandler).first;
//     }
//
//     template<class T>
//     int subscribe(T* instance, void (T::*method)(ARGS...) const) {
//         // Adds a lambda that calls the class instance at the const function memory address into the handlers_,
//         // increments the index after its value is read
//         // then returns the place where its at AKA the old currentKey
//         return handlers_.insert(currentKey_++, [instance, method](ARGS... args) {
//             (instance->*method)(args...);
//         }).first;
//     }
//
//     template<class T>
//     int subscribe(T* instance, void (T::*method)(ARGS...)) {
//         // Adds a lambda that calls the class instance at the function memory address into the handlers_,
//         // increments the index after its value is read
//         // then returns the place where its at AKA the old currentKey
//         return handlers_.insert(currentKey_++, [instance, method](ARGS... args) {
//             (instance->*method)(args...);
//         }).first;
//     }
//
//     void unSubscribe(const int i) {
//         // Removes the std::function whos key is i
//         handlers_.erase(i);
//     }
//
//     void invoke(ARGS... args) {
//         for (auto& handler : handlers_) {
//             handler(args...); // invoke the method with the proper arguments
//         }
//     }
//
//     void operator()(ARGS... args) {
//         invoke(args...);
//     }
// };

// #pragma once
// #include <functional>
//
// template<typename... ARGS>
// class Event {
// private:
//     using Handler = std::function<void(ARGS...)>;
//     std::vector<Handler> handlers_;
//
// public:
//     void subscribe(const Handler& functionHandler) {
//         handlers_.push_back(functionHandler);
//     }
//
//     template<class T>
//     void subscribe(T* instance, void (T::*method)(ARGS...) const) {
//         // push back the function signature from a class instance
//         handlers_.push_back([instance, method](ARGS... args) {
//             (instance->*method)(args...);
//         });
//     }
//
//     template<class T>
//     void subscribe(T* instance, void (T::*method)(ARGS...)) {
//         // push back the function signature from a class instance
//         handlers_.push_back([instance, method](ARGS... args) {
//             (instance->*method)(args...);
//         });
//     }
//
//     template<class T>
//     void unSubscribe(T* instance, void (T::*method)(ARGS...) const) {
//         // remove the function siganture from the handlers
//         std::erase(handlers_, [instance, method](ARGS... args) {
//             (instance->*method)(args...);
//         });
//     }
//
//     template<class T>
//     void unSubscribe(T* instance, void (T::*method)(ARGS...)) {
//         // remove the function siganture from the handlers
//         std::erase(handlers_, [instance, method](ARGS... args) {
//             (instance->*method)(args...);
//         });
//     }
//
//     void invoke(ARGS... args) {
//         if (handlers_.empty())
//             return;
//
//         for (auto& handler : handlers_) {
//             handler(args...); // invoke the method with the proper arguments
//         }
//     }
//
//     void operator()(ARGS... args) {
//         invoke(args...);
//     }
// };

