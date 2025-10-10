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

    JsonObject jsonScene = parser.parseValue();

    Scene scene;

    for (JsonObject jsonObject : jsonScene.getArrayField("objects")) {
        std::string name = jsonObject.getStringField("name");
        int tag = jsonObject.getNumberField("tag");

        std::shared_ptr<Object> object = scene.addObject();

        for (JsonObject component : jsonObject.getArrayField("components")) {
            std::string type = component.getStringField("type");

            if (JsonArray dependencies = component.getArrayField("dependencies"); dependencies.size() != 0) {
                for (JsonValue dependency: dependencies) {
                    std::string dependencyUuid = std::get<std::string>(dependency.value);

                    // if the uuid isnt in the registry yet, we need to wait for it to be there
                    if (!uuidRegistry_.contains(dependencyUuid)) {
                        laterCreationEntries_.emplace_back(object, type, );
                    }
                }
            }
        }
    }

    return scene;
}

bool Deserializer::LaterCreationEntry::removeFromDependenciesToWaitFor(const std::string& uuid) {
    for (auto it = dependenciesToWaitFor.begin(); it != dependenciesToWaitFor.end(); ) {
        if (*it == uuid) {
            it = dependenciesToWaitFor.erase(it);

            if (dependenciesToWaitFor.empty()) {
                if (const auto object = objectParent.lock()) {
                    const auto comp = ComponentRegistry::instance().create(type, jsonObject);
                    object->addComponent(comp);
                }
                return true;
            }
        }
        else {
            ++it;
        }
    }

    return false;
}

void Deserializer::addToUuidRegistry(const std::string& uuid, void* value, const UuidRegistryEntry::Type type) {
    uuidRegistry_[uuid] = {value, type};

    for (LaterCreationEntry& entry : laterCreationEntries_) {
        entry.removeFromDependenciesToWaitFor(uuid);
    }
}
