#ifndef RAYCASTINGENGINE_H
#define RAYCASTINGENGINE_H

#include <engine.h>
#include <simpleray.h>
#include <collidable.h>

namespace BQ {

class RaycastingEngine : public Engine
{
public:
    RaycastingEngine();

    SimpleRay& createBasicRay(sf::Vector2f,sf::Vector2f);
    SimpleRay& createOwnedRay(sf::Vector2f,sf::Vector2f,GameObject&);

    void start();
    void run();
    void finish();
    void drawDebug();

    std::vector<SimpleRay> getSimpleRays() const;
    void setSimpleRays(const std::vector<SimpleRay> &value);

protected:

    std::vector<SimpleRay> simpleRays;
};

}

#endif // RAYCASTINGENGINE_H
