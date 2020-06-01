using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCloth 
{ 

    public sealed class UClothWorld
    {
        private readonly static UClothWorld _instance = new UClothWorld();

        public static UClothWorld Instance
        {
            get { return _instance; }
        }

        [SerializeField]
        private Vector3 gravity = new Vector3(0.0f, -9.8f, 0.0f);

        private IntPtr _worldHandle;

        UClothWorld() 
        {
            _worldHandle = UClothImports.ucloth_createWorld();
            UClothImports.ucloth_addAcceleration(_worldHandle, ref gravity);
        }

        ~UClothWorld()
        {
             UClothImports.ucloth_deleteWorld(_worldHandle);
        }

        public IntPtr GetHandle()
        {
            return _worldHandle;
        }

        // Start is called before the first frame update
        void Start()
        {
        
        }

        // Update is called once per frame
        void Update()
        {
        
        }
    }
}