#pragma once
#include <functional>
#include <string>

#include "json/jsonObject.h"

class Object;
class Component;
class Scene;

class Deserializer {
public:
    class SceneLoaderError final : public std::exception {
    public:
        explicit SceneLoaderError(std::string message) : message_(std::move(message)) {};

        [[nodiscard]] const char* what() const noexcept override {
            return message_.c_str();
        }

    private:
        std::string message_;
    };

    /**
     * @note Can be ran asynchronously (on other thread)
     * @param filePath Where is the scenes .json file located
     * @return Scene constructed from .json file
     */
    Scene loadSceneFromFile(const std::string& filePath);

    std::function<std::shared_ptr<Component>()> getCreationFunctionFromType(std::string type);

private:
    struct LaterCreationEntry {
        std::weak_ptr<Object> objectParent;
        std::string type;
        std::vector<std::string> dependenciesToWaitFor;
        JsonObject jsonObject;

        /**
         *
         * @param uuid The uuid of the object that was recently registerd
         * @return Should this LaterCreationEntry be deleted from the laterCreationList
         */
        bool removeFromDependenciesToWaitFor(const std::string& uuid);
    };

    struct UuidRegistryEntry {
        enum Type {
            component,
            object
        };

        void* value;
        Type type;
    };

    void addToUuidRegistry(const std::string& uuid, void* value, UuidRegistryEntry::Type type);

    std::vector<LaterCreationEntry> laterCreationEntries_;
    std::unordered_map<std::string, UuidRegistryEntry> uuidRegistry_;
};

class ComponentRegistry {
public:
    static ComponentRegistry& instance() {
        static ComponentRegistry factory;
        return factory;
    }

    void registerComponent(const std::string& name, std::function<std::shared_ptr<Component>(const JsonObject&)> func) {
        registry_[name] = std::move(func);
    }

    std::shared_ptr<Component> create(const std::string& name, const JsonObject& json) const {
        const auto it = registry_.find(name);

        if (it == registry_.end()) {
            throw std::runtime_error("Component not registered: " + name);
        }

        return it->second(json);
    }

private:
    std::unordered_map<std::string, std::function<std::shared_ptr<Component>(const JsonObject&)>> registry_;
};

#define REGISTER_COMPONENT(NAME, TYPE) \
namespace { \
    const bool TYPE##_registered = []{ \
        ComponentRegistry::instance().registerComponent(NAME, TYPE::deserialize); \
        return true; \
       }(); \
}