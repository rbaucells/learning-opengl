#include "deserializer.h"

#include "componentRegistry.h"
#include "../scene.h"
#include "json++/json.h"

std::unique_ptr<Scene> Deserializer::loadSceneFromFile(const std::string& filePath) {
    // open stream and things
    std::ifstream stream(filePath);
    JsonLexer lexer(stream);
    JsonParser parser(lexer);

    // get the scene
    JsonObject jsonScene = parser.parseValue();

    // make a scene for us to return
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    // loop through objects
    for (JsonObject jsonObject : jsonScene.getArrayField("objects")) {
        createObjectFromJsonObject(jsonObject, scene.get());
    }

    if (!postponedComponents_.empty() || !postponedObjects_.empty()) {
        throw DeserializerError("Circular dependency detected in 2 components/objects");
    }

    return std::move(scene);
}

bool Deserializer::isInIdRegistry(const std::string& id) const {
    return idRegistry_.contains(id);
}

void Deserializer::addToIdRegistry(const std::string& id, const Type type, void* data) {
    idRegistry_[id] = {type, data};

    // iterate through component entries
    for (auto postponeObjectIt = postponedObjects_.begin(); postponeObjectIt != postponedObjects_.end();) {
        // iterate through the ids to check if it's the one
        for (auto it = postponeObjectIt->dependenciesToWaitFor.begin(); it != postponeObjectIt->dependenciesToWaitFor.end();) {
            // if it is remove that id form the dependencies
            if (*it == id) {
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
            createObjectFromJsonObject(jsonObject, owner);
        }
        else {
            ++postponeObjectIt;
        }
    }

    // iterate through component entries
    for (auto postponeComponentIt = postponedComponents_.begin(); postponeComponentIt != postponedComponents_.end();) {
        // iterate through the ids to check if it's the one
        for (auto it = postponeComponentIt->dependenciesToWaitFor.begin(); it != postponeComponentIt->dependenciesToWaitFor.end();) {
            // if it is remove that id form the dependencies
            if (*it == id) {
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
            createComponentFromJsonObject(jsonObject, owner);
        }
        else {
            ++postponeComponentIt;
        }
    }
}

Deserializer::IdRegistryEntry Deserializer::getFromIdRegistry(const std::string& id) {
    return idRegistry_[id];
}

void Deserializer::postponeObjectCreation(const PostponedObject& entry) {
    postponedObjects_.push_back(entry);
}

void Deserializer::postponeComponentCreation(const PostponedComponent& entry) {
    postponedComponents_.push_back(entry);
}

void Deserializer::createObjectFromJsonObject(const JsonObject& jsonObject, Scene* owner) {
    // get fields for constructing object
    std::string name = jsonObject.getStringField("name");
    int tag = static_cast<int>(jsonObject.getNumberField("tag"));
    std::string objectid = jsonObject.getStringField("id");

    // get transform
    JsonObject jsonTransform = jsonObject.getObjectField("transform");
    std::string transformid = jsonTransform.getStringField("id");

    Transform* transformParent = nullptr;

    // if we have a parent
    if (!jsonTransform.getIsNullField("parent")) {
        std::string parentid = jsonTransform.getStringField("parent");

        // if that parent doesn't exist yet, we have to delay this entire objects creation
        if (!isInIdRegistry(parentid)) {
            postponeObjectCreation({jsonObject, owner, {parentid}});
            return;
        }

        // if we are here that means we have a parent that already exists
        transformParent = static_cast<Transform*>(getFromIdRegistry(parentid).data);
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

    addToIdRegistry(objectid, Type::object, object.get());
    addToIdRegistry(transformid, Type::transform, &object->transform);

    // loop through components
    for (JsonObject jsonComponent : jsonObject.getArrayField("components")) {
        createComponentFromJsonObject(jsonComponent, object.get());
    }
}

void Deserializer::createComponentFromJsonObject(const JsonObject& jsonComponent, Object* owner) {
    const std::string componentid = jsonComponent.getStringField("id");
    const std::string type = jsonComponent.getStringField("type");

    JsonArray dependencies = jsonComponent.getArrayField("dependencies");

    std::vector<std::string> dependenciesToWaitFor;

    for (std::string dependencyid : dependencies) {
        if (!isInIdRegistry(dependencyid)) {
            dependenciesToWaitFor.push_back(dependencyid);
        }
    }

    if (dependenciesToWaitFor.empty()) {
        const std::shared_ptr<Component> component = ComponentRegistry::create(type, owner, jsonComponent);

        owner->addComponent(component);
        addToIdRegistry(componentid, Type::component, component.get());
    }
    else {
        postponeComponentCreation({jsonComponent, owner, dependenciesToWaitFor});
    }
}