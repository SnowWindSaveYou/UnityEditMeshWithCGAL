using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Unity.Collections;
using UnityEngine;
using Utility;

namespace CGALPlugin
{
    public enum EditToolType
    {
        None = 0,
        Drag = 1,
        Deform = 2,
        Raise,
        Lower,
        Draw,
        Rubber,
        Scaler,
        Smooth,
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
        [HideInInspector]
        public EditToolType currentEditToolType = EditToolType.None;
        public Dictionary<EditToolType, IEditTool> editTools;

        NativeArray<int> m_selectedList;
        ComputeBuffer m_selectedVerticesBuffer;

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


        public void SetEditToolType(EditToolType editToolType)
        {
            if (editToolType == currentEditToolType) return;
            currentEditTool?.OnWithdraw();

            currentEditToolType = editToolType;

            if (editTools.ContainsKey(editToolType)){ 

                currentEditTool = editTools[editToolType];
                CPDLL_EDITPROCESS.setCurrentTool((int)editToolType);

                currentEditTool.OnInit();
                //todo
            }
            else
            {
                currentEditTool = null;
            }
        }


        public void SetCurrentTarget(SurfaceMesh target)
        {
            if (currentTarget != null)
            {
                currentTarget.OnWithDraw();
                currentTarget.m_material.DisableKeyword("HIGHLIGHT_VERTICES");
            }
            currentTarget = target;
            currentTarget.OnSelected();


            if(m_selectedList==null|| m_selectedList.Count()< target.m_work_vertices.Count())
            {
                if (m_selectedList != null) {
                    m_selectedVerticesBuffer.Release();
                    m_selectedList.Dispose(); 
                }
                m_selectedList = new NativeArray<int>(target.m_work_vertices.Count(), Allocator.Persistent);

                m_selectedVerticesBuffer = new ComputeBuffer(target.m_work_vertices.Count(), sizeof(int));
            }

            m_selectedVerticesBuffer.SetData(m_selectedList,0,0,target.m_work_vertices.Count());
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


            editTools = new Dictionary<EditToolType, IEditTool>();
            editTools.Add(EditToolType.Drag, new GrabBrushTool());
            editTools.Add(EditToolType.Deform, new DeformTool());

            //test
            SetEditToolType(EditToolType.Drag);

            if (currentTarget != null)
            {
                SetCurrentTarget(currentTarget);
            }
        }
        private void OnDestroy()
        {
            //CGALMeshEditPluginDLL.disposePlugin();
        }

        // Update is called once per frame
        void Update()
        {

        }
    }
}