using AOT;
using System;
using System.Runtime.InteropServices;
using UnityEngine;
using UCloth;

public class DebugCPP : MonoBehaviour
{
    void Awake() 
    {
        //UClothImports.ucloth_registerDebugCallback(OnDebugCallback);
    }

    // Use this for initialization
    void OnEnable()
    {
        
    }

    void OnDestroy() 
    {
    
    }

    [MonoPInvokeCallback(typeof(UClothImports.DebugCallback))]
    static void OnDebugCallback(IntPtr request, int strLength)
    {
        //Ptr to string
        string debug_string = Marshal.PtrToStringAnsi(request, strLength);

        //Add Specified Color
        //debug_string =
        //    String.Format("{0}{1}{2}{3}{4}",
        //    "<color=",
        //    ((Color)color).ToString(),
        //    ">",
        //    debug_string,
        //    "</color>"
        //    );

        UnityEngine.Debug.Log(debug_string);
    }
}