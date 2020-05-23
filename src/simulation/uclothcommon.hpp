#ifndef UCLOTH__UCLOTH_COMMON_H_
#define UCLOTH__UCLOTH_COMMON_H_

#include <array>
#include <vector>
#include <utility>
#include <umath/umath.hpp>
#include <cstdint>
#include <variant>

namespace ucloth
{
    namespace simulation
    {

        using Particle = int;
        using Face = std::array<Particle, 3>;
        using Edge = std::pair<Particle, Particle>;

        enum class MeshType : uint32_t
        {
            StaticMesh,
            Rigidbody,
            Cloth
        };

        struct Mesh
        {
            std::vector<Face> faces;
            Particle begin; // defines the range in the world vectors
            Particle end;
            umath::Real kVelocity;
            umath::Real kClothThickness;
            MeshType type;
        };

        struct Attachment
        {
            Particle p;
            umath::Real originalInverseMass;
            std::variant<Particle, umath::Position> destination;

        };

    } // namespace simulation
} // namespace ucloth

#endif // !UCLOTH__UCLOTH_COMMON_H_