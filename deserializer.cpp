#include "deserializer.h"
#include "scene.h"
#include "json/jsonArray.h"
#include "json/jsonLexer.h"
#include "json/jsonObject.h"
#include "json/jsonParser.h"
#include "json/jsonValue.h"

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

    return scene;
}

bool Deserializer::isInUuidRegistry(const std::string& uuid) const {
    return uuidRegistry_.contains(uuid);
}

void Deserializer::addToUuidRegistry(const std::string& uuid, const Type type, void* data) {
    uuidRegistry_[uuid] = {type, data};

    // iterate through component entries
    for (auto postponeObjectIt = postponeObjectEntries_.begin(); postponeObjectIt != postponeObjectEntries_.end();) {
        // iterate through the uuids to check if its the one
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
        if (postponeObjectIt->dependenciesToWaitFor.empty()) {}
        else {
            ++postponeObjectIt;
        }
    }

    // iterate through component entries
    for (auto postponeComponentIt = postponeComponentEntries_.begin(); postponeComponentIt != postponeComponentEntries_.end();) {
        // iterate through the uuids to check if its the one
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
            postponeComponentIt = postponeComponentEntries_.erase(postponeComponentIt);
            componentFromJsonObject(jsonObject, owner);
        }
        else {
            ++postponeComponentIt;
        }
    }
}

Deserializer::UuidRegistryEntry Deserializer::getFromUuidRegistry(const std::string& uuid) {
    return uuidRegistry_[uuid];
}

void Deserializer::addObjectToPostponeCreation(PostponeObjectEntry entry) {
    postponeObjectEntries_.push_back(entry);
}

void Deserializer::addComponentToPostponeCreation(PostponeComponentEntry entry) {
    postponeComponentEntries_.push_back(entry);
}

void Deserializer::objectFromJsonObject(const JsonObject& jsonObject, Scene* owner) {
    // get fields for constructing object
    std::string name = jsonObject.getStringField("name");
    int tag = jsonObject.getNumberField("tag");
    std::string objectUuid = jsonObject.getStringField("uuid");

    // get transform
    JsonObject jsonTransform = jsonObject.getObjectField("transform");
    std::string transformUuid = jsonTransform.getStringField("uuid");

    Transform* transformParent = nullptr;

    // if we have a parent
    if (!jsonTransform.getIsNullField("parent")) {
        std::string parentUuid = jsonTransform.getStringField("parent");

        // if that parent doesnt exist yet, we have to delay this entire objects creation
        if (!isInUuidRegistry(parentUuid)) {
            addObjectToPostponeCreation({jsonObject, owner, {parentUuid}});
            return;
        }

        // if we are here that means we have a parent that already exists
        transformParent = static_cast<Transform*>(getFromUuidRegistry(parentUuid).data);
    }

    // get the position
    JsonObject posObject = jsonTransform.getObjectField("position");
    Vector2 pos = Vector2(posObject.getNumberField("x"), posObject.getNumberField("y"));

    // get the rotation
    float rot = jsonTransform.getNumberField("rotation");

    // get the scale
    JsonObject scaleObject = jsonTransform.getObjectField("scale");
    Vector2 scale = Vector2(scaleObject.getNumberField("x"), scaleObject.getNumberField("y"));

    std::shared_ptr<Object> object;

    if (transformParent) {
        object = owner->addObject(name, tag, pos, rot, scale, transformParent);
    }
    else {
        object = owner->addObject(name, tag, pos, rot, scale);
    }

    addToUuidRegistry(objectUuid, Type::object, object.get());
    addToUuidRegistry(transformUuid, Type::transform, &object->transform);

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
        if (!isInUuidRegistry(dependencyUuid)) {
            dependenciesToWaitFor.push_back(dependencyUuid);
        }
    }

    if (dependenciesToWaitFor.empty()) {
        const std::shared_ptr<Component> component = ComponentRegistry::instance().create(type, owner, jsonComponent);
        owner->addComponent(component);
        addToUuidRegistry(componentUuid, Type::component, component.get());
    }
    else {
        addComponentToPostponeCreation({jsonComponent, owner, dependenciesToWaitFor});
    }
}

ComponentRegistry& ComponentRegistry::instance() {
    static ComponentRegistry factory;
    return factory;
}

void ComponentRegistry::registerComponent(const std::string& name, std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)> func) {
    registry_[name] = std::move(func);
}

std::shared_ptr<Component> ComponentRegistry::create(const std::string& type, Object* owner, const JsonObject& json) const {
    const auto it = registry_.find(type);

    if (it == registry_.end()) {
        throw std::runtime_error("Component not registered: " + type);
    }

    return it->second(owner, json);
}
