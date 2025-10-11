#pragma once
#include <functional>
#include <string>

#include "json/jsonObject.h"
#include "math/vector2.h"

struct Vector2;
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

private:
    enum class Type {
        component,
        object
    };

    struct UuidRegistryEntry {
        Type type;
        void* data;
    };

    struct PostponeObjectEntry {
        std::string name;
        std::string uuid;
        int tag;

        Vector2 pos;
        float rot;
        Vector2 scale;

        Scene* owner;

        std::vector<std::string> dependenciesToWaitFor;
    };

    struct PostponeComponentEntry {
        std::string type;
        Object* owner;
        JsonObject jsonObject;
        std::string uuid;

        std::vector<std::string> dependenciesToWaitFor;
    };

    std::vector<PostponeObjectEntry> postponeObjectEntries_;
    std::vector<PostponeComponentEntry> postponeComponentEntries_;

    std::unordered_map<std::string, UuidRegistryEntry> uuidRegistry_;

    bool isInUuidRegistry(const std::string& uuid) const;
    void addToUuidRegistry(const std::string& uuid, Type type, void* data);
    UuidRegistryEntry getFromUuidRegistry(const std::string& uuid);

    void addObjectToPostponeCreation(PostponeObjectEntry entry);
    void addComponentToPostponeCreation(PostponeComponentEntry entry);

    void objectFromJsonObject(const JsonObject& jsonObject, Scene* owner);
    void componentFromJsonObject(const JsonObject& jsonComponent, Object* owner);
};

class ComponentRegistry {
public:
    static ComponentRegistry& instance() {
        static ComponentRegistry factory;
        return factory;
    }

    void registerComponent(const std::string& name, std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)> func) {
        registry_[name] = std::move(func);
    }

    std::shared_ptr<Component> create(const std::string& type, Object* owner, const JsonObject& json) const {
        const auto it = registry_.find(type);

        if (it == registry_.end()) {
            throw std::runtime_error("Component not registered: " + type);
        }

        return it->second(owner, json);
    }

private:
    std::unordered_map<std::string, std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)>> registry_;
};

// helper macro for registering components before int main()
#define REGISTER_COMPONENT(NAME, TYPE) \
namespace { \
    const bool TYPE##_registered = []{ \
        ComponentRegistry::instance().registerComponent(NAME, TYPE::deserialize); \
        return true; \
       }(); \
}
