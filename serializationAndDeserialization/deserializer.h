#pragma once
#include <functional>
#include <string>

#include "../json/jsonObject.h"
#include "../json/jsonArray.h"

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

    enum class Type {
        component,
        object,
        transform
    };

    struct UuidRegistryEntry {
        Type type;
        void* data;
    };

    /**
     * @note Can be ran asynchronously (on other thread)
     * @param filePath Where is the scenes .json file located
     * @return Scene constructed from .json file
     */
    Scene loadSceneFromFile(const std::string& filePath);

private:
    struct PostponedObject {
        JsonObject jsonObject;
        Scene* owner;

        std::vector<std::string> dependenciesToWaitFor;
    };

    struct PostponeComponent {
        JsonObject jsonObject;
        Object* owner;

        std::vector<std::string> dependenciesToWaitFor;
    };

    std::vector<PostponedObject> postponedObjects_;
    std::vector<PostponeComponent> postponedComponents_;

    std::unordered_map<std::string, UuidRegistryEntry> idRegistry_;

    [[nodiscard]] bool isInIdRegistry(const std::string& uuid) const;
    void addToIdRegistry(const std::string& uuid, Type type, void* data);
    UuidRegistryEntry getFromIdRegistry(const std::string& uuid);

    void postponeObjectCreation(const PostponedObject& entry);
    void postponeComponentCreation(const PostponeComponent& entry);

    void objectFromJsonObject(const JsonObject& jsonObject, Scene* owner);
    void componentFromJsonObject(const JsonObject& jsonComponent, Object* owner);
};
