#include "myComponent2.h"

#include "myComponent.h"
#include "../object.h"

void myComponent2::myCallbackVoid(float f) {
    std::printf("I HAVE BEEN CALLED");
}


void myComponent2::start() {
    const auto comp = object -> GetComponent<myComponent>();

    comp -> OnMyEvent.likeAndSubscribe(this, &myComponent2::myCallbackVoid);
}
