#include "uclothinterface.h"
#include "uclothdebug.h"

#include <world.hpp>
#include <pbdsimulation.hpp>
#include <uclothcommon.hpp>
#include <umath.hpp>

void ucloth_registerDebugCallback(FuncCallBack cb)
{
    Debug::callbackInstance = cb;
}

WorldHandle ucloth_createWorld()
{
    auto* world = new ucloth::simulation::World();
    return reinterpret_cast<WorldHandle>(world);
}

void ucloth_deleteWorld(WorldHandle handle)
{
    auto* world = reinterpret_cast<ucloth::simulation::World*>(handle);
    delete world;
}

void ucloth_addAcceleration(WorldHandle world, UClothVector3f acceleration)
{
    auto* worldPtr = reinterpret_cast<ucloth::simulation::World*>(world);
    // TODO: add acceleration implement method
    worldPtr->accelerations.push_back({acceleration.x_, acceleration.y_, acceleration.z_});
}


PBDSystemHandle ucloth_createPBDSimulation(void)
{
    auto* system = new ucloth::simulation::PBDSimulation();
    return reinterpret_cast<PBDSystemHandle>(system);
}

void ucloth_deletePBDSimulation(PBDSystemHandle simulationHandle)
{
    auto* systemPtr = reinterpret_cast<ucloth::simulation::PBDSimulation*>(simulationHandle);
    delete systemPtr;
}

void ucloth_simulate(PBDSystemHandle simulationHandle, WorldHandle worldHandle, int solverIterations, float deltaTime)
{
    auto* systemPtr = reinterpret_cast<ucloth::simulation::PBDSimulation*>(simulationHandle);
    auto* worldPtr = reinterpret_cast<ucloth::simulation::World*>(worldHandle);
    systemPtr->simulate(deltaTime, solverIterations, *worldPtr);
}

ClothHandle ucloth_addCloth(
    WorldHandle worldHandle, 
    UClothVector3f* positions, 
    size_t posSize, int* faces, 
    size_t facesSize, 
    float mass, 
    float elasticity,
    float damping)
{
    auto* world = reinterpret_cast<ucloth::simulation::World*>(worldHandle);
    auto* pos = reinterpret_cast<ucloth::umath::Vec3*>(positions); 
    std::vector<ucloth::umath::Vec3> posVector{pos, pos + posSize};

    ucloth::simulation::Mesh mesh;
    mesh.faces.reserve(facesSize / 3);
    mesh.begin = world->positions.size();
    mesh.end = mesh.begin + posSize;
    mesh.kClothThickness = 0;
    mesh.kVelocity = damping;
    mesh.type = ucloth::simulation::MeshType::Cloth;

    std::vector<int> facesVector{faces, faces + facesSize};
    for (int f = 0; f < facesSize; f += 3)
    {
        ucloth::simulation::Particle const p1 = faces[f + 0];
        ucloth::simulation::Particle const p2 = faces[f + 1];
        ucloth::simulation::Particle const p3 = faces[f + 2];
        mesh.faces.emplace_back(ucloth::simulation::Face{p1, p2, p3});
    }

    ucloth::simulation::Mesh const* cloth = &world->addCloth(posVector, mesh, mass, elasticity, 0.0f);
    return reinterpret_cast<ClothHandle>(cloth);
}

void ucloth_retrieveClothInfo(
    ClothHandle clothHandle, 
    WorldHandle worldHandle, 
    UClothVector3f*& positions,
    size_t& posSize,
    int*& faces,
    size_t& facesSize
    )
{
    auto* worldPtr = reinterpret_cast<ucloth::simulation::World*>(worldHandle);
    auto* const clothPtr = reinterpret_cast<const ucloth::simulation::Mesh*>(clothHandle);
    positions = reinterpret_cast<UClothVector3f*>(&worldPtr->positions[clothPtr->begin]);
    posSize = clothPtr->end - clothPtr->begin;
    faces = const_cast<int*>(reinterpret_cast<const int*>(clothPtr->faces.data()));
    facesSize = clothPtr->faces.size() * 3;
}

void ucloth_attachParticleToPosition(WorldHandle worldHandle, ClothHandle clothHandle, unsigned int index, UClothVector3f position)
{
    auto* worldPtr = reinterpret_cast<ucloth::simulation::World*>(worldHandle);
    auto* const clothPtr = reinterpret_cast<const ucloth::simulation::Mesh*>(clothHandle);

    ucloth::umath::Position pos = {pos};
    
}
