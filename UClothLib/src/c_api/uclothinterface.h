#ifndef UCLOTH_INTERFACE_H
#define UCLOTH_INTERFACE_H

#include "uclothexport.h"
#include "uclothhandles.h"
#include "uclothstructures.h"

extern "C"
{
    UCLOTH_EXPORT WorldHandle ucloth_createWorld(void);
    UCLOTH_EXPORT void ucloth_deleteWorld(WorldHandle world);
    UCLOTH_EXPORT void ucloth_addAcceleration(WorldHandle world, UClothVector3f acceleration);

    UCLOTH_EXPORT PBDSystemHandle ucloth_createPBDSimulation(void);
    UCLOTH_EXPORT void ucloth_deletePBDSimulation(PBDSystemHandle pbdSimulation);
    UCLOTH_EXPORT void ucloth_simulate(PBDSystemHandle simulationHandle, WorldHandle worldHandle, int solverIterations, float deltaTime);
    UCLOTH_EXPORT ClothHandle ucloth_addCloth(
        WorldHandle worldHandle, 
        UClothVector3f* positions, 
        size_t posSize, int* faces, 
        size_t facesSize, 
        float mass, 
        float elasticity,
        float damping);

    UCLOTH_EXPORT void ucloth_retrieveClothInfo(
        ClothHandle clothHandle, 
        WorldHandle worldHandle, 
        UClothVector3f*& positions,
        size_t& posSize,
        int*& faces,
        size_t& faces_size
        );

    UCLOTH_EXPORT void ucloth_attachParticleToPosition();
}

#endif // UCLOTH_INTERFACE_H