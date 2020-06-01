using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCloth
{
    public static class UClothImports
    {
        public const string UClothLibName = "ucloth_c_api";

        [DllImport(UClothLibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ucloth_createWorld();

        [DllImport(UClothLibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ucloth_deleteWorld(IntPtr world);

        [DllImport(UClothLibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ucloth_addAcceleration(IntPtr world, ref Vector3 acceleration);

        [DllImport(UClothLibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ucloth_createPBDSimulation();

        [DllImport(UClothLibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ucloth_deletePBDSimulation(IntPtr simulation);

        [DllImport(UClothLibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ucloth_simulate(IntPtr simulation, IntPtr world, int solverIteration, float deltaTime);

        [DllImport(UClothLibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern unsafe IntPtr ucloth_addCloth(IntPtr world, Vector3* positions, int posSize, int* faces, int facesSize, float mass, float elasticity, float damping);

        [DllImport(UClothLibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ucloth_retrieveCloth(IntPtr clothHandle, IntPtr worldHandle, ref IntPtr positions, int posSize, ref IntPtr faces, int facesSize);

        [DllImport(UClothLibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ucloth_attachParticleToPosition(IntPtr world, IntPtr cloth, uint index, Vector3 position);
    }


}