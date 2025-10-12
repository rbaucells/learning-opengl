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

    for (auto entry : postponeObjectEntries_) {
        for (auto it = entry.dependenciesToWaitFor.begin(); it != entry.dependenciesToWaitFor.end();) {
            if (*it == uuid) {
                it = entry.dependenciesToWaitFor.erase(it);
            }
            else {
                ++it;
            }
        }

        if (entry.dependenciesToWaitFor.empty()) {}
    }

    for (auto entry : postponeComponentEntries_) {
        for (auto it = entry.dependenciesToWaitFor.begin(); it != entry.dependenciesToWaitFor.end();) {
            if (*it == uuid) {
                it = entry.dependenciesToWaitFor.erase(it);
            }
            else {
                ++it;
            }
        }

        if (entry.dependenciesToWaitFor.empty()) {
            const std::shared_ptr<Component> component = ComponentRegistry::instance().create(entry.type, entry.owner, entry.jsonObject);
            entry.owner->addComponent(component);
            addToUuidRegistry(uuid, Type::component, component.get());
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

    Transform* transformParent = nullptr;

    // get the position
    JsonObject posObject = jsonTransform.getObjectField("position");
    Vector2 pos = Vector2(posObject.getNumberField("x"), posObject.getNumberField("y"));

    // get the rotation
    float rot = jsonTransform.getNumberField("rotation");

    // get the scale
    JsonObject scaleObject = jsonTransform.getObjectField("scale");
    Vector2 scale = Vector2(scaleObject.getNumberField("x"), scaleObject.getNumberField("y"));

    // if we have a perent
    if (!jsonTransform.getIsNullField("parent")) {
        std::string parentUuid = jsonTransform.getStringField("parent");

        // if that parent doesnt exist yet, we have to delay this entire objects creation
        if (!isInUuidRegistry(parentUuid)) {
            addObjectToPostponeCreation({name, objectUuid, tag, pos, rot, scale, owner, {parentUuid}});
            return;
        }

        // if we are here that means we have a parent that already exists
        transformParent = static_cast<Transform*>(getFromUuidRegistry(parentUuid).data);
    }

    std::shared_ptr<Object> object;

    if (transformParent) {
        object = owner->addObject(name, tag, pos, rot, scale, transformParent);
    }
    else {
        object = owner->addObject(name, tag, pos, rot, scale);
    }

    addToUuidRegistry(objectUuid, Type::object, object.get());

    // loop through components
    for (JsonObject jsonComponent : jsonObject.getArrayField("components")) {
        componentFromJsonObject(jsonComponent, object.get());
    }
}

void Deserializer::componentFromJsonObject(const JsonObject& jsonComponent, Object* owner) {
    const std::string componentUuid = jsonComponent.getStringField("uuid");
    const std::string type = jsonComponent.getStringField("type");

    JsonArray dependencies = jsonComponent.getArrayField("dependencies");

    std::vector<std::string> dependenciesToWaitFor(dependencies.size());

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
        addComponentToPostponeCreation({type, owner, jsonComponent, componentUuid, dependenciesToWaitFor});
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
