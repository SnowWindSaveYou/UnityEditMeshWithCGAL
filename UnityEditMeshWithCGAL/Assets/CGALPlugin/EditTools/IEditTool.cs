using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace CGALPlugin {

public interface IEditTool
{
    public void OnInit();// �л����ù�����Ҫ��ЩԤ����
    public void OnWithdraw();// �е�����������Ҫ�ͷ�ʲô

    public void OnStart();//��ʼִ��
    public void OnEnd();//ִ�н���
    public void OnUpdate();//ִ�й���
 }
}