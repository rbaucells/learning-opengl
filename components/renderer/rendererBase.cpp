#include "renderer.h"

void RendererBase::removeFromAllRenderers() {
    if (const auto it = allRenderers.find(renderingLayer_); it != allRenderers.end()) {
        auto& layer = it->second;

        std::erase(layer, this);

        if (layer.empty())
            allRenderers.erase(it);
    }
}

void RendererBase::addToAllRenderers(const int layer) {
    allRenderers[layer].push_back(this);
}