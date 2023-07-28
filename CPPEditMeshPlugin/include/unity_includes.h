#ifndef UNITY_INCLUDES_H
#define UNITY_INCLUDES_H

struct Unity_Matrix {
    // mat in unity is col pri 
    float m00;
    float m10;
    float m20;
    float m30;

    float m01;
    float m11;
    float m21;
    float m31;

    float m02;
    float m12;
    float m22;
    float m32;

    float m03;
    float m13;
    float m23;
    float m33;

    // Eigen::Matrix4d ToMatrix4d() {
    //     Eigen::Matrix4d mat;
    //     mat.setIdentity();
    //     mat << m00, m01, m02, m03,
    //         m10, m11, m12, m13,
    //         m20, m21, m22, m23,
    //         m30, m31, m32, m33;
    //     return mat;
    // }

};

struct Unity_Vector3 {
    float x;
    float y;
    float z;
};





#endif