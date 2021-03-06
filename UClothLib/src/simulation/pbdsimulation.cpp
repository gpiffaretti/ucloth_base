#include "pbdsimulation.hpp"
#include "uclothdebug.h"
#include <numeric>
#include <variant>
#include <algorithm>

namespace ucloth
{
    namespace simulation
    {
        void PBDSimulation::simulate(umath::Real const deltaTime, size_t const solverIterations, World& world)
        {
            applyExternalAccelerations(world.accelerations, deltaTime, world.velocities);
            dampVelocities(world.meshes, world.positions, world.inverseMasses, world.velocities);
            createPositionEstimates(world.positions, world.velocities, deltaTime);
            solveAttachments(world.attachments);

            ignoreAttachmentMasses(world.attachments, world.inverseMasses);
            for(size_t iteration = 0; iteration < solverIterations; ++iteration)
            {
                // Fill
                projectDistanceConstraints(world.distanceConstraints, world.inverseMasses, solverIterations);
                solveAttachments(world.attachments);
            }
            restoreAttachmentMasses(world.attachments, world.inverseMasses);

            returnResultsToWorld(world.positions, deltaTime, world.velocities);
        }

        void PBDSimulation::applyExternalAccelerations(std::vector<umath::Vec3> const& externalAccelerations, umath::Real const deltaTime, std::vector<umath::Vec3>& velocities) const
        {
            for(auto const& acceleration : externalAccelerations)
            {
                for(auto& velocity : velocities)
                {
                    velocity += acceleration * deltaTime;
                }
            }
        }

        void PBDSimulation::dampVelocities(std::vector<Mesh> const& meshes, std::vector<umath::Position> const& positions, std::vector<umath::Real> const& inverseMasses, std::vector<umath::Vec3> & velocities) const
        {
            for(auto const& mesh : meshes)
            {
                // Total mass
                umath::Real const totalMass = 
                    std::accumulate(inverseMasses.begin() + mesh.begin, 
                                    inverseMasses.begin() + mesh.end, 
                                    0.0f, 
                                    [](umath::Real input, umath::Real const inverseMass) {
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
                umath::Vec3 vcm(0.0f, 0.0f, 0.0f);
                for(Particle p = mesh.begin; p < mesh.end; ++p)
                {
                    vcm += velocities[p] / (inverseMasses[p]);
                }
                vcm /= totalMass;

                umath::Vec3 L(0.0f, 0.0f, 0.0f);
                for(Particle p = mesh.begin; p < mesh.end; ++p)
                {
                    umath::Position const ri = positions[p] - xcm;
                    L += umath::cross(ri, velocities[p] / inverseMasses[p]);
                }
                
                // inertia matrix
                umath::Mat3x3 I(0.0f);
                for(Particle p = mesh.begin; p < mesh.end; ++p)
                {
                    umath::Position const ri = positions[p] - xcm;
                    umath::Mat3x3 const riPrime({{0, -ri.z, ri.y}, {ri.z, 0, -ri.x}, {-ri.y, ri.x, 0}});
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

        void PBDSimulation::createPositionEstimates(std::vector<umath::Position> const& positions, std::vector<umath::Vec3> const& velocities, umath::Real const deltaTime)
        {
            size_t const nParticles = positions.size();
            m_PositionEstimates.resize(nParticles);

            for(Particle p = 0; p < nParticles; ++p)
            {
                m_PositionEstimates[p] = positions[p] + velocities[p] * deltaTime;
            }
        }


        void PBDSimulation::solveAttachments(std::vector<Attachment> const& attachments)
        {
            for(auto const& attachment : attachments)
            {
                if(std::holds_alternative<Particle>(attachment.destination))
                {
                    auto const& destination = std::get<Particle>(attachment.destination);
                    m_PositionEstimates[attachment.p] = m_PositionEstimates[destination];
                }
                else
                {
                    auto const& destination = std::get<umath::Position>(attachment.destination);
                    m_PositionEstimates[attachment.p] = destination;
                }
            }
        }

        void PBDSimulation::ignoreAttachmentMasses(std::vector<Attachment> const& attachments, std::vector<umath::Real> inverseMasses) const
        {
            for(auto const& attachment : attachments)
            {
                inverseMasses[attachment.p] = 0;
            }
        }

        void PBDSimulation::restoreAttachmentMasses(std::vector<Attachment> const& attachments, std::vector<umath::Real> inverseMasses) const
        {
            for(auto const& attachment : attachments)
            {
                inverseMasses[attachment.p] = attachment.originalInverseMass;
            }   
        }

        void PBDSimulation::returnResultsToWorld(std::vector<umath::Position> & positions, umath::Real const deltaTime, std::vector<umath::Vec3>& velocities) const
        {
            size_t const nParticles = positions.size();
            for(Particle p = 0; p < nParticles; ++p)
            {
                velocities[p] = (m_PositionEstimates[p] - positions[p]) / deltaTime;
            }
            std::copy(m_PositionEstimates.begin(), m_PositionEstimates.end(), positions.begin());
        }

        void PBDSimulation::projectDistanceConstraints(std::vector<DistanceConstraint> const constraints, std::vector<umath::Real> const& inverseMasses, size_t const solverIterations)
        {
            for(auto const& constraint : constraints)
            {
                umath::Position& p1 = m_PositionEstimates[constraint.p1];
                umath::Position& p2 = m_PositionEstimates[constraint.p2];

                umath::Real const w1 = inverseMasses[constraint.p1];
                umath::Real const w2 = inverseMasses[constraint.p2];

                umath::Real const C = umath::length(p1 - p2) - constraint.distance;
                umath::Vec3 const n = umath::normalize(p1 - p2);
                umath::Vec3 const deltaP1 = (-n) * w1 * C / (w1 + w2);
                umath::Vec3 const deltaP2 = (n) * w1 * C / (w1 + w2);

                umath::Real kPrime = 1 - powf(1 - constraint.stiffness, 1.0f / static_cast<umath::Real>(solverIterations));
                p1 += kPrime * deltaP1;
                p2 += kPrime * deltaP2;
            }
        }
    }
}