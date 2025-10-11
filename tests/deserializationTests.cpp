#include "../deserializer.h"
#include "../scene.h"

int main() {
    Deserializer deserializer;
    Scene scene = deserializer.loadSceneFromFile("/Users/ricardito/Projects/learning-opengl/res/json files/scene.json");

    printf("Hello World");
}
