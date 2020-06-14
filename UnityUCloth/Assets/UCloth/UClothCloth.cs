using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCloth
{

    [RequireComponent(typeof(MeshFilter))]
    public class UClothCloth : MonoBehaviour
    {
        private Mesh _mesh;

        [SerializeField]
        private Attachment[] _attachments = {};

        private float[] _positionValues = null;
        private int[] _facesValues = null;

        private IntPtr _clothHandle;

        [SerializeField]
        private float _mass = 1.0f;

        [SerializeField]
        private float _elasticity = 0.9f;

        [SerializeField]
        private float _damping = 0.2f;

        void Start()
        {
            _mesh = GetComponent<MeshFilter>().mesh;
            IntPtr worldHandle = UClothWorld.Instance.GetHandle();
            _clothHandle = CreateCloth(worldHandle, _mesh, _mass, _elasticity, _damping);
            
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

            RetrieveCloth(worldHandle);
        }

        private unsafe void RetrieveCloth(IntPtr worldHandle) 
        {
            IntPtr positions = (IntPtr)0;
            int positionsSize = 0;
            IntPtr faces = (IntPtr)0;
            int facesSize = 0;

            //Debug.Log($"Attempt retrieve cloth. ClothHandle = {_clothHandle}");
            UClothImports.ucloth_retrieveClothInfo(_clothHandle, worldHandle, ref positions, ref positionsSize, ref faces, ref facesSize);

            Marshal.Copy(positions, _positionValues, 0, positionsSize * 3);
            Vector3[] vertices = _mesh.vertices;

            for (int i = 0; i < _mesh.vertices.Length; i++)
            {
                vertices[i] = new Vector3(_positionValues[i*3], _positionValues[i*3 + 1], _positionValues[i*3 + 2]);
            }

            _mesh.vertices = vertices;
        }

        private unsafe IntPtr CreateCloth(IntPtr worldHandle, Mesh mesh, float mass, float elasticity, float damping)
        {
            Vector3[] vertices = mesh.vertices;
            int[] faces = mesh.triangles;

            // allocate arrays one time only for performance. We don't wanna allocate each time for marshalling later.
            if (_positionValues == null)
                _positionValues = new float[vertices.Length * 3];
            if (_facesValues == null)
                _facesValues = new int[faces.Length];

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