#include "componentRegistry.h"

// registry is only available in this file
namespace {
    std::unordered_map<std::string, std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)>>& getRegistry() {
        static std::unordered_map<std::string, std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)>> registry;
        return registry;
    }

    std::mutex& getRegistryMutex() {
        static std::mutex registryMutex;
        return registryMutex;
    }
}

void ComponentRegistry::registerComponent(const std::string& name, const std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)>& factoryFunc) {
    std::lock_guard lockGuard(getRegistryMutex());
    getRegistry()[name] = factoryFunc;
}

std::shared_ptr<Component> ComponentRegistry::create(const std::string& type, Object* owner, const JsonObject& json) {
    std::lock_guard lockGuard(getRegistryMutex());
    const auto it = getRegistry().find(type);

    if (it == getRegistry().end())
        throw ComponentRegistryError("Type: " + type + " was not found in ComponentRegistry");

    return it->second(owner, json);
}
