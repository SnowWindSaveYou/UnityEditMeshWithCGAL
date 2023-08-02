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

        }

        public void OnDispose()
        {

        }

        public void OnStart()
        {
            var pos = CGALEditMeshManager.Instance.MouseRaycastTargetWorld();
            hitDist = Camera.main.WorldToScreenPoint(pos).z;

            //var dir = Camera.main.transform.forward;

            var mousepos = Input.mousePosition;
            mousepos.z = hitDist-0.01f;
            var worldPos = Camera.main.ScreenToWorldPoint(mousepos);
            var w2l = CGALEditMeshManager.Instance.currentTarget.transform.worldToLocalMatrix;
            var localPos = w2l.MultiplyPoint(worldPos);
            var localDir = w2l.MultiplyVector(Camera.main.transform.forward);
            CPDLL_EDITPROCESS.setRayInfo(localPos, localDir);


            CGALEditMeshManager.Instance.currentTarget.testTrans.localPosition = localPos;

            //CPDLL_EDITPROCESS.setRayInfo(pos, dir);
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