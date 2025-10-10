#include "deserializer.h"
#include "scene.h"
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
        // get fields for constructing object
        std::string name = jsonObject.getStringField("name");
        int tag = jsonObject.getNumberField("tag");
        std::string objectUuid = jsonObject.getStringField("uuid");

        // get transform
        JsonObject jsonTransform = jsonObject.getObjectField("transform");

        Transform* transformParent = nullptr;

        // if we have a perent
        if (!jsonTransform.getIsNullField("parent")) {
            std::string parentUuid = jsonTransform.getStringField("parent");

            // if that parent doesnt exist yet, we have to delay this entire objects creation
            if (!isInUuidRegistry(parentUuid)) {
                postponeCreation(jsonObject, Type::object, &scene, {parentUuid});
                continue;
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
            object = scene.addObject(name, tag, pos, rot, scale, transformParent);
        }
        else {
            object = scene.addObject(name, tag, pos, rot, scale);
        }

        addToUuidRegistry(objectUuid, Type::object, object.get());

        // loop through components
        for (JsonObject jsonComponent : jsonObject.getArrayField("components")) {
            std::string componentUuid = jsonComponent.getStringField("uuid");
            std::string type = jsonComponent.getStringField("type");

            JsonArray dependencies = jsonComponent.getArrayField("dependencies");

            std::vector<std::string> dependenciesToWaitFor(dependencies.size());

            for (std::string dependencyUuid : dependencies) {
                if (!isInUuidRegistry(dependencyUuid)) {
                    dependenciesToWaitFor.push_back(dependencyUuid);
                }
            }

            if (dependenciesToWaitFor.empty()) {
                std::shared_ptr<Component> component = ComponentRegistry::instance().create(type, object.get(), jsonComponent);
                object->addComponent(component);
                addToUuidRegistry(componentUuid, Type::component, component.get());
            }
            else {
                postponeCreation(jsonComponent, Type::component, object.get(), dependenciesToWaitFor);
                continue;
            }
        }
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
