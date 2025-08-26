#pragma once
#include "component.h"


class myComponent2 : public Component {
public:
    using Component::Component;
    void start() override;
    void myCallbackVoid(float f);
};
