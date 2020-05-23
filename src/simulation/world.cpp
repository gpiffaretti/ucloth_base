#include "world.hpp"

namespace ucloth
{
    namespace simulation
    {
        void World::addCloth(std::vector<umath::Position> const& pos, 
                             Mesh const& mesh,
                             umath::Real mass)
        {
            size_t const nNewParticles = pos.size();
            size_t const currentSize = positions.size();
            reserveForNParticles(nNewParticles + currentSize);
            positions.insert(positions.end(), pos.begin(), pos.end());
            velocities.resize(currentSize + nNewParticles, umath::Vec3(0.0f,0.0f,0.0f));
            umath::Real const invMassParticle = static_cast<umath::Real>(currentSize + nNewParticles) / mass;
            inverseMasses.resize(currentSize + nNewParticles, invMassParticle);

            // Current size == latest index available
            size_t const indexShift = currentSize;
            Mesh copy = mesh;
            copy.begin += indexShift;
            copy.end += indexShift;
            for(auto& face: copy.faces)
            {
                face[0] += indexShift;
                face[1] += indexShift;
                face[2] += indexShift;
            }
            copy.type = MeshType::Cloth;
            meshes.push_back(std::move(copy));

            // TODO: add constraints
        }

        void World::attachParticle(Mesh const& mesh, Particle particle, umath::Position const& position)
        {
            Particle pInWorld = mesh.begin + particle;
            attachments.push_back(std::move(Attachment{pInWorld, inverseMasses[pInWorld], position}));
        }

        void World::reserveForNParticles(size_t const nParticles)
        {
            positions.reserve(nParticles);
            velocities.reserve(nParticles);
            inverseMasses.reserve(nParticles);
        }

    }
}