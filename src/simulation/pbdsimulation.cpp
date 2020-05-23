#include "pbdsimulation.hpp"
#include <numeric>

namespace ucloth
{
    namespace simulation
    {
        void PBDSimulation::simulate(umath::Real const deltaTime, size_t const solverIterations, World& world)
        {

        }

        void PBDSimulation::applyExternalAccelerations(std::vector<umath::Vec3> const& externalAccelerations, umath::Real const deltaTime, std::vector<umath::Vec3>& velocities)
        {
            for(auto const& acceleration : externalAccelerations)
            {
                for(auto& velocity : velocities)
                {
                    velocity += acceleration * deltaTime;
                }
            }
        }

        void PBDSimulation::dampVelocities(std::vector<Mesh> const& meshes, std::vector<umath::Position> const& positions, std::vector<umath::Real> const& inverseMasses, std::vector<umath::Vec3> & velocities)
        {
            for(auto const& mesh : meshes)
            {
                // Total mass
                umath::Real const totalMass = std::accumulate(inverseMasses.begin() + mesh.begin, inverseMasses.begin() + mesh.end, 0.0f, [](umath::Real input, umath::Real const inverseMass) {
                    return std::move(input) + (1.0f / inverseMass);
                }); 

                // Calculate Center of Mass
                umath::Position xcm(0.0f, 0.0f, 0.0f);
                for(Particle p = mesh.begin; p < mesh.end; ++p)
                {
                    xcm += positions[p] / (inverseMasses[p]);
                }
                xcm /= totalMass;

                // Calculate Velocity
                umath::Position vcm(0.0f, 0.0f, 0.0f);
                for(Particle p = mesh.begin; p < mesh.end; ++p)
                {
                    vcm += positions[p] / (inverseMasses[p]);
                }
                vcm /= totalMass;

                umath::Vec3 L(0.0f, 0.0f, 0.0f);
                for(Particle p = mesh.begin; p < mesh.end; ++p)
                {
                    umath::Position const ri = positions[p] - xcm;
                    L += umath::cross(ri, velocities[p] / inverseMasses[p]);
                }
                
                umath::Mat3x3 I(0.0f);
                for(Particle p = mesh.begin; p < mesh.end; ++p)
                {
                    umath::Position const ri = positions[p] - xcm;
                    umath::Mat3x3 const riPrime({{0, -ri.z, ri.y}, {ri.z, 0, -ri.y}, {-ri.y, ri.x, 0}});
                    I += riPrime * umath::transpose(riPrime) / inverseMasses[p];
                }

                umath::Vec3 const angularVelocity = umath::inverse(I) * L;
                for(Particle p = mesh.begin; p < mesh.end; ++p)
                {
                    umath::Position const ri = positions[p] - xcm;
                    umath::Vec3 const deltaV = vcm + umath::cross(angularVelocity, ri) - velocities[p];
                    velocities[p] += mesh.kVelocity * deltaV;
                }

            }
        }
    }
}