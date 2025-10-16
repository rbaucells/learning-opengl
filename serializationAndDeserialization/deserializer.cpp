#include "deserializer.h"

#include "componentRegistry.h"
#include "../scene.h"
#include "../json/jsonArray.h"
#include "../json/jsonLexer.h"
#include "../json/jsonObject.h"
#include "../json/jsonParser.h"
#include "../json/jsonValue.h"

Scene Deserializer::loadSceneFromFile(const std::string& filePath) {
    // open stream and things
    std::ifstream stream(filePath);
    JsonLexer lexer(stream);
    JsonParser parser(lexer);

    // get the scene
    JsonObject jsonScene = parser.parseValue();

    // make a scene for us to return
    Scene scene;

    // loop through objects
    for (JsonObject jsonObject : jsonScene.getArrayField("objects")) {
        objectFromJsonObject(jsonObject, &scene);
    }

    if (!postponedComponents_.empty() || !postponedObjects_.empty()) {
        throw DeserializerError("Circular dependency detected in 2 components/objects");
    }

    return scene;
}

bool Deserializer::isInIdRegistry(const std::string& uuid) const {
    return idRegistry_.contains(uuid);
}

void Deserializer::addToIdRegistry(const std::string& uuid, const Type type, void* data) {
    idRegistry_[uuid] = {type, data};

    // iterate through component entries
    for (auto postponeObjectIt = postponedObjects_.begin(); postponeObjectIt != postponedObjects_.end();) {
        // iterate through the uuids to check if it's the one
        for (auto it = postponeObjectIt->dependenciesToWaitFor.begin(); it != postponeObjectIt->dependenciesToWaitFor.end();) {
            // if it is remove that uuid form the dependencies
            if (*it == uuid) {
                it = postponeObjectIt->dependenciesToWaitFor.erase(it);
            }
            else {
                ++it;
            }
        }

        // if there are no more dependencies, create it
        if (postponeObjectIt->dependenciesToWaitFor.empty()) {
            JsonObject jsonObject = postponeObjectIt->jsonObject;
            Scene* owner = postponeObjectIt->owner;

            postponeObjectIt = postponedObjects_.erase(postponeObjectIt);
            objectFromJsonObject(jsonObject, owner);
        }
        else {
            ++postponeObjectIt;
        }
    }

    // iterate through component entries
    for (auto postponeComponentIt = postponedComponents_.begin(); postponeComponentIt != postponedComponents_.end();) {
        // iterate through the uuids to check if it's the one
        for (auto it = postponeComponentIt->dependenciesToWaitFor.begin(); it != postponeComponentIt->dependenciesToWaitFor.end();) {
            // if it is remove that uuid form the dependencies
            if (*it == uuid) {
                it = postponeComponentIt->dependenciesToWaitFor.erase(it);
            }
            else {
                ++it;
            }
        }

        // if there are no more dependencies, create it
        if (postponeComponentIt->dependenciesToWaitFor.empty()) {
            JsonObject jsonObject = postponeComponentIt->jsonObject;
            Object* owner = postponeComponentIt->owner;
            postponeComponentIt = postponedComponents_.erase(postponeComponentIt);
            componentFromJsonObject(jsonObject, owner);
        }
        else {
            ++postponeComponentIt;
        }
    }
}

Deserializer::UuidRegistryEntry Deserializer::getFromIdRegistry(const std::string& uuid) {
    return idRegistry_[uuid];
}

void Deserializer::postponeObjectCreation(const PostponedObject& entry) {
    postponedObjects_.push_back(entry);
}

void Deserializer::postponeComponentCreation(const PostponeComponent& entry) {
    postponedComponents_.push_back(entry);
}

void Deserializer::objectFromJsonObject(const JsonObject& jsonObject, Scene* owner) {
    // get fields for constructing object
    std::string name = jsonObject.getStringField("name");
    int tag = static_cast<int>(jsonObject.getNumberField("tag"));
    std::string objectUuid = jsonObject.getStringField("uuid");

    // get transform
    JsonObject jsonTransform = jsonObject.getObjectField("transform");
    std::string transformUuid = jsonTransform.getStringField("uuid");

    Transform* transformParent = nullptr;

    // if we have a parent
    if (!jsonTransform.getIsNullField("parent")) {
        std::string parentUuid = jsonTransform.getStringField("parent");

        // if that parent doesn't exist yet, we have to delay this entire objects creation
        if (!isInIdRegistry(parentUuid)) {
            postponeObjectCreation({jsonObject, owner, {parentUuid}});
            return;
        }

        // if we are here that means we have a parent that already exists
        transformParent = static_cast<Transform*>(getFromIdRegistry(parentUuid).data);
    }

    // get the position
    JsonObject posObject = jsonTransform.getObjectField("position");
    Vector2 pos = Vector2(static_cast<float>(posObject.getNumberField("x")), static_cast<float>(posObject.getNumberField("y")));

    // get the rotation
    float rot = static_cast<float>(jsonTransform.getNumberField("rotation"));

    // get the scale
    JsonObject scaleObject = jsonTransform.getObjectField("scale");
    Vector2 scale = Vector2(static_cast<float>(scaleObject.getNumberField("x")), static_cast<float>(scaleObject.getNumberField("y")));

    std::shared_ptr<Object> object;

    if (transformParent) {
        object = owner->addObject(name, tag, pos, rot, scale, transformParent);
    }
    else {
        object = owner->addObject(name, tag, pos, rot, scale);
    }

    addToIdRegistry(objectUuid, Type::object, object.get());
    addToIdRegistry(transformUuid, Type::transform, &object->transform);

    // loop through components
    for (JsonObject jsonComponent : jsonObject.getArrayField("components")) {
        componentFromJsonObject(jsonComponent, object.get());
    }
}

void Deserializer::componentFromJsonObject(const JsonObject& jsonComponent, Object* owner) {
    const std::string componentUuid = jsonComponent.getStringField("uuid");
    const std::string type = jsonComponent.getStringField("type");

    JsonArray dependencies = jsonComponent.getArrayField("dependencies");

    std::vector<std::string> dependenciesToWaitFor;

    for (std::string dependencyUuid : dependencies) {
        if (!isInIdRegistry(dependencyUuid)) {
            dependenciesToWaitFor.push_back(dependencyUuid);
        }
    }

    if (dependenciesToWaitFor.empty()) {
        const std::shared_ptr<Component> component = ComponentRegistry::create(type, owner, jsonComponent, [this](const std::string& id) {
            return getFromIdRegistry(id);
        });

        owner->addComponent(component);
        addToIdRegistry(componentUuid, Type::component, component.get());
    }
    else {
        postponeComponentCreation({jsonComponent, owner, dependenciesToWaitFor});
    }
}