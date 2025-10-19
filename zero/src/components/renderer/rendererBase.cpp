#include "renderer.h"

void RendererBase::addToAllRenderers(const int layer) {
    if (const auto it = renderersDrawPublishers.find(layer); it != renderersDrawPublishers.end()) {
        drawSubscription_ = it->second->subscribe(this, &RendererBase::draw);
    }
    else {
        renderersDrawPublishers[layer] = Publisher<const Matrix<4, 4>&, const Matrix<4, 4>&>::create();
        drawSubscription_ = renderersDrawPublishers.at(layer)->subscribe(this, &RendererBase::draw);
    }
}
