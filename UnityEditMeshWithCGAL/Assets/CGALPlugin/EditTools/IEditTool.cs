using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace CGALPlugin {

public interface IEditTool
{
    public void OnInit();// 切换到该工具需要哪些预处理
    public void OnWithdraw();// 切到其他工具需要释放什么

    public void OnStart();//开始执行
    public void OnEnd();//执行结束
    public void OnUpdate();//执行过程
 }
}