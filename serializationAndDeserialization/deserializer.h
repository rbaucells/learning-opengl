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

    struct IdRegistryEntry {
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

    struct PostponedComponent {
        JsonObject jsonObject;
        Object* owner;

        std::vector<std::string> dependenciesToWaitFor;
    };

    std::vector<PostponedObject> postponedObjects_;
    std::vector<PostponedComponent> postponedComponents_;

    std::unordered_map<std::string, IdRegistryEntry> idRegistry_;

    [[nodiscard]] bool isInIdRegistry(const std::string& uuid) const;
    void addToIdRegistry(const std::string& uuid, Type type, void* data);
    IdRegistryEntry getFromIdRegistry(const std::string& uuid);

    void postponeObjectCreation(const PostponedObject& entry);
    void postponeComponentCreation(const PostponedComponent& entry);

    void createObjectFromJsonObject(const JsonObject& jsonObject, Scene* owner);
    void createComponentFromJsonObject(const JsonObject& jsonComponent, Object* owner);
};