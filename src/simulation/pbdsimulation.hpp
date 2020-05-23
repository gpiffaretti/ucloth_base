
#ifndef UCLOCH_PDB_SIMULATION_H_
#define UCLOCH_PDB_SIMULATION_H_

#include <umath.hpp>
#include <vector>
#include "world.hpp"

namespace ucloth
{
    namespace simulation
    {
        class PBDSimulation
        {
        public:
            void simulate(umath::Real const deltaTime, size_t const solverIterations, World& world);

        private:
            void applyExternalAccelerations(std::vector<umath::Vec3> const& externalAccelerations, umath::Real const deltaTime, std::vector<umath::Vec3>& velocities);
            void dampVelocities(std::vector<Mesh> const& meshes, std::vector<umath::Position> const& positions, std::vector<umath::Real> const& inverseMasses, std::vector<umath::Vec3> & velocities);

        private:
            std::vector<umath::Position> m_PositionEstimates;

        };

    } // namespace simulation

} // namespace ucloth


#endif // UCLOCH_PDB_SIMULATION_H_