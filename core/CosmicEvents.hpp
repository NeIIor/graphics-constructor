#ifndef COSMIC_EVENTS_HPP
#define COSMIC_EVENTS_HPP

#include "QuantumCore.hpp"
#include <functional>

class CosmicElement;

enum EventFlow {
    StopFlow,
    ContinueFlow
};

class CosmicSignal {
protected:
    bool propagating = true;

public:
    virtual ~CosmicSignal    ()                       = default;
    virtual EventFlow process(CosmicElement* element) = 0;
    
    void halt     ()       {        propagating = false; }
    bool isFlowing() const { return propagating;         }
};

class PhotonMoveSignal : public CosmicSignal {
private:
    QuantumVector position;

public:
                  PhotonMoveSignal(const QuantumVector& pos) : position(pos) {}
    QuantumVector getPosition()    const    { return position; }
    EventFlow     process         (CosmicElement* element) override;
};

class QuantumClickSignal : public CosmicSignal {
private:
    QuantumVector position;
    bool pressed;

public:
    QuantumClickSignal(const QuantumVector& pos ,    bool isPressed) 
                                 : position(pos), pressed(isPressed) {}
    
    QuantumVector getPosition() const { return position; }
    bool          isPressed  () const { return pressed;  }

    EventFlow process        (CosmicElement* element) override;
};

class NexusPressSignal : public CosmicSignal {
private:
    int keyCode;
    bool pressed;

public:
              NexusPressSignal(int key, bool isPressed) : keyCode(key), pressed(isPressed) {}
    int       getKey          () const { return keyCode; }
    bool      isPressed       () const { return pressed; }
    EventFlow process         (CosmicElement* element) override;
};

#endif