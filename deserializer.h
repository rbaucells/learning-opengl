#pragma once
#include <functional>
#include <string>

#include "json/jsonObject.h"
#include "json/jsonArray.h"

struct Vector2;
class Object;
class Component;
class Scene;

class Deserializer {
public:
    class DeserializerError final : public std::exception {
    public:
        explicit DeserializerError(std::string message) : message_(std::move(message)) {};

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
        object,
        transform
    };

    struct UuidRegistryEntry {
        Type type;
        void* data;
    };

    struct PostponeObjectEntry {
        JsonObject jsonObject;
        Scene* owner;

        std::vector<std::string> dependenciesToWaitFor;
    };

    struct PostponeComponentEntry {
        JsonObject jsonObject;
        Object* owner;

        std::vector<std::string> dependenciesToWaitFor;
    };

    std::vector<PostponeObjectEntry> postponeObjectEntries_;
    std::vector<PostponeComponentEntry> postponeComponentEntries_;

    std::unordered_map<std::string, UuidRegistryEntry> uuidRegistry_;

    [[nodiscard]] bool isInUuidRegistry(const std::string& uuid) const;
    void addToUuidRegistry(const std::string& uuid, Type type, void* data);
    UuidRegistryEntry getFromUuidRegistry(const std::string& uuid);

    void addObjectToPostponeCreation(const PostponeObjectEntry& entry);
    void addComponentToPostponeCreation(const PostponeComponentEntry& entry);

    void objectFromJsonObject(const JsonObject& jsonObject, Scene* owner);
    void componentFromJsonObject(const JsonObject& jsonComponent, Object* owner);
};

class ComponentRegistry {
public:
    static ComponentRegistry& instance();

    void registerComponent(const std::string& name, std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)> func);

    std::shared_ptr<Component> create(const std::string& type, Object* owner, const JsonObject& json) const;

private:
    std::unordered_map<std::string, std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)>> registry_;
};

template<typename T>
concept DeserializableComponent = requires(Object* owner, const JsonObject& json) {
    { T::deserialize(owner, json) } -> std::same_as<std::shared_ptr<Component>>;
};

// helper macro for registering components before int main()
#define REGISTER_COMPONENT(NAME, TYPE) \
static_assert(DeserializableComponent<TYPE>, \
#TYPE " must have static std::shared_ptr<Component> deserialize(Object*, const JsonObject&)"); \
namespace { \
    const bool TYPE##_registered = []{ \
        ComponentRegistry::instance().registerComponent(NAME, TYPE::deserialize); \
        return true; \
       }(); \
}
