#ifndef UCLOTH_WORLD_H_
#define UCLOTH_WORLD_H_

#include <umath.hpp>
#include <vector>

#include "uclothcommon.hpp"
#include "constraint.hpp"

namespace ucloth
{
    namespace simulation
    {
        struct World
        {
            // Particles as Components
            std::vector<umath::Position> positions;
            std::vector<umath::Vec3> velocities;
            std::vector<umath::Real> inverseMasses;

            // Relations between particles
            std::vector<Attachment> attachments;
            
            std::vector<DistanceConstraint> distanceConstraints;

            // Meshes
            std::vector<Mesh> meshes;

            // External forces
            std::vector<umath::Vec3> accelerations;

            Mesh const& addCloth(std::vector<umath::Position> const& pos, 
                            Mesh const& mesh,
                            umath::Real mass,
                            umath::Real elasticity,
                            umath::Real stiffness);

            void attachParticle(Mesh const& mesh, Particle particle, umath::Position const& position);

        private:
            void reserveForNParticles(size_t const nParticles);

            
        };

    }
    
}

#endif // !UCLOTH_WORLD_H_