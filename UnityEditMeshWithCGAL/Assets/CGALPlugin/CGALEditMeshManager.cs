using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Utility;

namespace CGALPlugin
{
    public enum EditToolType
    {
        None = 0,
        Drag,
        Raise,
        Lower,
        Draw,
        Rubber,
        Scaler,
        Smooth
    }



    public class CGALEditMeshManager : MonoBehaviour
    {
        private static CGALEditMeshManager _Instance;
        public static CGALEditMeshManager Instance
        {
            get
            {
                if (_Instance == null)
                {
                    _Instance = GameObject.FindObjectOfType<CGALEditMeshManager>();
                    if (_Instance == null)
                    {
                        var go = new GameObject("CGALEditMeshManager");
                        _Instance = go.AddComponent<CGALEditMeshManager>();
                    }
                }

                return _Instance;
            }
        }


        public SurfaceMesh currentTarget;
        public IEditTool currentEditTool;

        float brushSize = 0.3f;

        private bool isEditProcessing = false;
        public bool GetEditAllowState { 
            get { 
                return 
                    currentTarget!=null
                    && currentEditTool!=null
                    ;
            } 
        }


        #region [Interaction help]


        public Vector3 MouseRaycastTargetWorld()
        {

            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

            Vector3 localStart = this.transform.worldToLocalMatrix.MultiplyPoint(ray.GetPoint(0));
            Vector3 localStartDir = this.transform.worldToLocalMatrix.MultiplyVector(ray.direction);
            Vector3 localPos = CGALMeshEditPluginDLL.rayInteract(localStart, localStartDir);

            Vector3 pos = currentTarget.transform.localToWorldMatrix.MultiplyPoint(localPos);
            return pos;
        }
        #endregion [Interaction help]

        #region [Edit Actions]

        void handleOnEditBegin()
        {
            if (!GetEditAllowState) return;
            isEditProcessing = true;
            currentEditTool.OnStart();
        }

        void handleOnEditEnd()
        {
            if (!GetEditAllowState) return;
            isEditProcessing = false;
            currentEditTool.OnEnd();
        }

        void handleOnEditProcess()
        {
            if (!GetEditAllowState) return;
            
            currentEditTool.OnUpdate();
        }

        #endregion  [Edit Actions]

        // Start is called before the first frame update
        void Start()
        {
            KeyMouseActions.Instance.OnMouseLeftDown += handleOnEditBegin;
            KeyMouseActions.Instance.OnMouseLeftUp += handleOnEditEnd;
            KeyMouseActions.Instance.OnMouseLeft += handleOnEditProcess;

            CGALMeshEditPluginDLL.initPlugin();

            currentEditTool = new GrabBrushTool();
        }
        private void OnDestroy()
        {
            CGALMeshEditPluginDLL.disposePlugin();
        }

        // Update is called once per frame
        void Update()
        {

        }
    }
}