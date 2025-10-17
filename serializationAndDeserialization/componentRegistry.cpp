#include "componentRegistry.h"

// registry is only available in this file
namespace {
    std::unordered_map<std::string, std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)>> registry;
    std::mutex registryMutex;
}

void ComponentRegistry::registerComponent(const std::string& name, const std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)>& factoryFunc) {
    std::lock_guard lockGuard(registryMutex);
    registry[name] = factoryFunc;
}

std::shared_ptr<Component> ComponentRegistry::create(const std::string& type, Object* owner, const JsonObject& json) {
    std::lock_guard lockGuard(registryMutex);
    const auto it = registry.find(type);

    if (it == registry.end())
        throw ComponentRegistryError("Type: " + type + " was not found in ComponentRegistry");

    return it->second(owner, json);
}
