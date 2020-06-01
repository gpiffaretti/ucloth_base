using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace UCloth
{
    public class UClothSimulation : MonoBehaviour
    {
        private IntPtr _simulationHandle;

        [SerializeField]
        private int solverIterations = 100;

        // Start is called before the first frame update
        void Awake()
        {
            _simulationHandle = UClothImports.ucloth_createPBDSimulation();
            Debug.Log("Created PBD simulation");
        }

        void FixedUpdate()
        {
            UClothImports.ucloth_simulate(_simulationHandle, UClothWorld.Instance.GetHandle(), 5, Time.fixedDeltaTime);    
        }

        // Update is called once per frame
        void OnDestroy()
        {
            UClothImports.ucloth_deletePBDSimulation(_simulationHandle);
            Debug.Log("Destroyed PBD simulation");
        }
    }

}