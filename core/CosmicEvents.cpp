#include "CosmicEvents.hpp"
#include "../interface/CosmicElement.hpp"

EventFlow PhotonMoveSignal::process(CosmicElement* element) {
    if (element) {
        element->onPhotonMove(position);
    }
    return propagating ? ContinueFlow : StopFlow;
}

EventFlow QuantumClickSignal::process(CosmicElement* element) {
    if (element) {
        element->onQuantumClick(position, pressed);
    }
    return propagating ? ContinueFlow : StopFlow;
}

EventFlow NexusPressSignal::process(CosmicElement* element) {
    if (element) {
        element->onNexusPress(keyCode, pressed);
    }
    return propagating ? ContinueFlow : StopFlow;
}