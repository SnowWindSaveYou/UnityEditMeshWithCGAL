using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace CGALPlugin
{
    public class GrabBrushTool : IEditTool
    {


        float hitDist;

        ~GrabBrushTool()
        {

        }


        public void OnInit()
        {
            CPDLL_EDITPROCESS.onToolSelected();
        }

        public void OnWithdraw()
        {
            CPDLL_EDITPROCESS.onToolWithdraw();
        }

        public void OnStart()
        {
            var trans = CGALEditMeshManager.Instance.currentTarget.transform;
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

            Vector3 localStart = trans.worldToLocalMatrix.MultiplyPoint(ray.GetPoint(0));
            Vector3 localStartDir = trans.worldToLocalMatrix.MultiplyVector(ray.direction);
            Vector3 localPos = CGALMeshEditPluginDLL.rayInteract(localStart, localStartDir);
            Vector3 worldPos = trans.localToWorldMatrix.MultiplyPoint(localPos);
            hitDist = Camera.main.WorldToScreenPoint(worldPos).z;
            CPDLL_EDITPROCESS.setRayInfo(localStart, localStartDir);
            CGALEditMeshManager.Instance.currentTarget.testTrans.localPosition = localPos;
            CPDLL_EDITPROCESS.processToolBegin();
        }

        public void OnEnd()
        {
            CPDLL_EDITPROCESS.processToolEnd();
        }

        public void OnUpdate()
        {

            var mousePos = Input.mousePosition;
            mousePos.z = hitDist;

            var worldPos = Camera.main.ScreenToWorldPoint(mousePos);

            var w2l = CGALEditMeshManager.Instance.currentTarget.transform.worldToLocalMatrix;
            var localPos = w2l.MultiplyPoint(worldPos);
            var localDir = w2l.MultiplyVector( Camera.main.transform.forward);

            CPDLL_EDITPROCESS.setRayInfo(localPos, localDir);
            CPDLL_EDITPROCESS.processToolUpdate();

            CGALEditMeshManager.Instance.currentTarget.testTrans.localPosition = localPos;

            CGALEditMeshManager.Instance.currentTarget.RefreshMeshVertices();
           
        }

        void SelectVertices(bool isSelectMode = false)
        {

        }

    }
}