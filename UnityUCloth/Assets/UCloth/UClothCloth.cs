using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCloth
{

    [RequireComponent(typeof(MeshFilter))]
    public class UClothCloth : MonoBehaviour
    {
        private Attachment[] _attachments = {};

        private IntPtr _clothHandle;

        [SerializeField]
        private float _mass = 1.0f;

        [SerializeField]
        private float _elasticity = 0.9f;

        [SerializeField]
        private float _damping = 0.2f;

        void Start()
        {
            Mesh mesh = GetComponent<MeshFilter>().mesh;
            IntPtr worldHandle = UClothWorld.Instance.GetHandle();
            _clothHandle = CreateCloth(worldHandle, mesh, _mass, _elasticity, _damping);

            foreach (Attachment attachment in _attachments)
            {
                if (attachment.enabled)
                {
                    AttachIndexToPosition(worldHandle, _clothHandle, attachment.particle, attachment.position);
                }
            }

        }

        void OnDestroy()
        {

        }

        // Update is called once per frame
        void FixedUpdate()
        {
            IntPtr worldHandle = UClothWorld.Instance.GetHandle();

        }

        private unsafe void RetrieveCloth(IntPtr worldHandle) 
        {
            IntPtr positions = (IntPtr)0;
            int positionsSize = 0;
            IntPtr faces = (IntPtr)0;
            int facesSize = 0;

            //UClothImports.ucloth_retrieveClothInfo(_clothHandle, worldHandle, ref positions, ref positionsSize, ref faces, ref facesSize);

            //float[] a = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
            //Vector3[] positionVectors = new Vector3[positionsSize];

            //GCHandle handleVs = GCHandle.Alloc(positionVectors, GCHandleType.Pinned);
            //IntPtr positionsPtr = handleVs.AddrOfPinnedObject();

            //Marshal.Copy(positions, 0, positionsPtr, sizeof(float) * positionsSize * 3);

        }

        private unsafe IntPtr CreateCloth(IntPtr worldHandle, Mesh mesh, float mass, float elasticity, float damping)
        {
            Vector3[] vertices = mesh.vertices;
            int[] faces = mesh.triangles;

            fixed (Vector3* v = vertices)
            {
                fixed (int* f = faces)
                {
                    return UClothImports.ucloth_addCloth(worldHandle, v, mesh.vertexCount, f, mesh.triangles.Length, mass, elasticity, damping);
                }
            }
        }

        private void AttachIndexToPosition(IntPtr worldHandle, IntPtr clothHandle, uint index, Vector3 position)
        {
            UClothImports.ucloth_attachParticleToPosition(worldHandle, clothHandle, index, position);
        }

        [System.Serializable]
        public class Attachment 
        {
            public uint particle;
            public Vector3 position;
            public bool enabled = true;
        }
    }

}