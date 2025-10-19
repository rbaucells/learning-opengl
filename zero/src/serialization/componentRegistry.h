#pragma once
#include <functional>
#include <memory>
#include "deserializer.h"

class JsonObject;
class Component;
class Object;

namespace ComponentRegistry {
    void registerComponent(const std::string& name, const std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)>& factoryFunc);
    std::shared_ptr<Component> create(const std::string& type, Object* owner, const JsonObject& json);

    class ComponentRegistryError final : std::exception {
    public:
        explicit ComponentRegistryError(std::string message) : message_(std::move(message)) {}

        const char* what() const noexcept override {
            return message_.c_str();
        }

    private:
        std::string message_;
    };
}

template<typename T>
concept DeserializableComponent = requires(Object* owner, const JsonObject& json) {
    { T::deserialize(owner, json) } -> std::same_as<std::shared_ptr<Component>>;
};

// helper macro for registering components before int main(). Must be used in .cpp file (Static Linkage)
#define REGISTER_COMPONENT(NAME, TYPE) \
static_assert(DeserializableComponent<TYPE>, #TYPE " must have static std::shared_ptr<Component> deserialize(Object*, const JsonObject&)"); \
namespace { \
    const bool TYPE##_registered = []{ \
        ComponentRegistry::registerComponent(NAME, TYPE::deserialize); \
        return true; \
    }(); \
}
