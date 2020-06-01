using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCloth
{

    public class UClothCloth : MonoBehaviour
    {
        private Attachment[] _attachments;

        private IntPtr _clothHandle;

        [SerializeField]
        private float _mass;

        [SerializeField]
        private float _elasticity;

        [SerializeField]
        private float _damping;

        void Awake()
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

        private unsafe void AttachIndexToPosition(IntPtr worldHandle, IntPtr clothHandle, uint index, Vector3 position)
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