using CGALPlugin;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace Utility
{
    public class KeyMouseActions : MonoBehaviour
    {

        private static KeyMouseActions _Instance;
        public static KeyMouseActions Instance
        {
            get
            {
                if (_Instance == null)
                {
                    _Instance = GameObject.FindObjectOfType<KeyMouseActions>();
                    if (_Instance == null)
                    {
                        var go = new GameObject("KeyMouseActions");
                        _Instance = go.AddComponent<KeyMouseActions>();
                    }
                }

                return _Instance;
            }
        }



        public event Action OnMouseLeftDown;
        public event Action OnMouseLeftUp;
        public event Action OnMouseLeft;


        // Start is called before the first frame update
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {
            handleMouseLeftActions();
        }



        bool isMouseLeftDown = false;
        void handleMouseLeftActions()
        {
            if (Input.GetMouseButtonDown(0))
            {
                isMouseLeftDown = true;
                OnMouseLeftDown?.Invoke();
            }
            if (Input.GetMouseButtonUp(0))
            {
                isMouseLeftDown = false;
                OnMouseLeftUp?.Invoke();
            }
            if (isMouseLeftDown)
            {
                OnMouseLeft?.Invoke();
            }
        }
    }
}