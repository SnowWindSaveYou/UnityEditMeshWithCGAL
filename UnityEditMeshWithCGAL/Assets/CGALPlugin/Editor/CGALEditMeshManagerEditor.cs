using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using CGALPlugin;

[CustomEditor(typeof(CGALEditMeshManager))]
public class CGALEditMeshManagerEditor : Editor
{
    private CGALEditMeshManager that;
    private EditToolType CurrentToolType;
    private float brushSize = 0.3f;


   
    private void OnEnable()
    {
        that = (CGALEditMeshManager)target;
        CurrentToolType = that.currentEditToolType;
        //brushSize = that
    }
    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();
        CurrentToolType = (EditToolType)EditorGUILayout.EnumPopup("Tool:", that.currentEditToolType);
        if (CurrentToolType != that.currentEditToolType)
        {
            that.SetEditToolType(CurrentToolType);
        }

        brushSize = EditorGUILayout.Slider("Brush Size:", brushSize, 0.01f, 1f);


        //EditorGUILayout.DropdownButton()




    }
}
