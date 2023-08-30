//Blinn-Phone光照模型
Shader "Custom/MeshEditShader"
{
    Properties
    {
        _Color("Color", Color) = (1,1,1,1)
        _Specular("Specular",Color)=(0.5,0.5,0.5,1)
        _Gloss("Gloss",Range(8.0,256))=20

        [Toggle] _IsCurrent ("Is Current Flag", int) = 0
    }
    SubShader
    {
		Tags {"Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent"}
        ZWrite Off     
        //Cull Off
        //ColorMask 0
        ZTest LEqual

        Pass
        {
            Tags {"LightMode" = "ForwardBase"}

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
 			#pragma target 5.0
			#pragma require randomwrite
            #include "UnityCG.cginc"
            #include "Lighting.cginc"

            #pragma multi_compile __ DISPLAY_BRUSH
            #pragma multi_compile __ HIGHLIGHT_VERTICES
            #pragma multi_compile __ GHOST



            //定义变量
            fixed4 _Color;
            fixed4 _Specular;
            float _Gloss;

            uint _Index;
            int _IsCurrent;

#if DISPLAY_BRUSH
            float BrushRadius;
            float3 BrushPosition;
#endif

#if HIGHLIGHT_VERTICES
            StructuredBuffer<int> _vtxHighlightBuffer;
#endif
            struct a2v
            {
                float4 vertex : POSITION;
                float3 normal:NORMAL; 
                uint vid:SV_VERTEXID;
            };
 
            struct v2f
            {
                float4 pos : SV_POSITION;
                float3 worldNormal:TEXCOORD0;   
                float3 worldPos:TEXCOORD1;   
                float3 localPos:TEXCOORD2;

                #if HIGHLIGHT_VERTICES
                float highlight:TEXCOORD3;
                #endif
            };
 
            v2f vert (a2v v)
            {
                v2f o;
                o.pos =UnityObjectToClipPos(v.vertex); //把顶点位置从模型空间转换到裁剪空间中
                o.worldNormal = normalize(UnityObjectToWorldNormal(v.normal.xyz));//法线方向转换世界空间下
                o.worldPos=mul(unity_ObjectToWorld,v.vertex).xyz;//顶点坐标转换世界空间下
                o.localPos = v.vertex;

                #if HIGHLIGHT_VERTICES
                o.highlight =(float)_vtxHighlightBuffer[v.vid];
                #endif
                return o;
            }

            [earlydepthstencil]
            fixed4 frag(v2f i, uint uCoverage : SV_COVERAGE) : SV_Target
            {
                float4 color = float4(0,0,0,0);

                fixed3 viewDir=normalize(UnityWorldSpaceViewDir(i.worldPos));//世界空间下的视角方向
                fixed3 worldNormal=normalize(i.worldNormal);
                fixed3 ambient=UNITY_LIGHTMODEL_AMBIENT.xyz;//得到环境光的颜色和强度信息

                
                fixed3 worldNormalInv = -worldNormal;
                fixed3 worldLightDir=normalize(UnityWorldSpaceLightDir(i.worldPos));//世界空间下的光照方向
 
                float4 col = _Color;

                float lightAttention = 1.0;

                const float wireScale = 0.3;
                 const float wirewidth= 0.1;
                 if(col.a<0.25){// 显示网线
                    if(frac(i.localPos.z*wireScale)<wirewidth){
                        col.a = 0.25;
                    }
                }
                if(_IsCurrent){

                    fixed3 colorForCurr = 1;
                    fixed3 redLightDir = fixed3(10,-5,0)- i.worldPos.xyz;
                    fixed3 blueLightDir = fixed3(-10,-5,0)-i.worldPos.xyz;
                    fixed3 diffuseRed =_LightColor0.rgb*fixed3(1,0,0)*saturate(dot(worldNormal,redLightDir)* 0.5 + 0.5)*0.2;
                    fixed3 diffuseBlue =_LightColor0.rgb*fixed3(0,0,1)*saturate(dot(worldNormal,blueLightDir)* 0.5 + 0.5)*0.2;
                    float VdN22 =dot(viewDir,worldNormal);
                    color.rgb+= saturate(diffuseRed+diffuseBlue);
                    if(VdN22>0){
                         float lerpW = pow( (1-VdN22),2*(1-col.a)+1);
                        color.rgb = lerp(color.rgb,colorForCurr.rgb,lerpW);
                        col.a = lerp(col.a,0.8,lerpW);
                        //col.a = max(col.a,0.8);
                    }
                }

#if HIGHLIGHT_VERTICES
                if(i.highlight>0){
                    color= lerp(color,1,saturate( i.highlight/0.5));
                    if(frac(i.localPos.x*wireScale)<wirewidth||frac(i.localPos.y*wireScale)<wirewidth||frac(i.localPos.z*wireScale)<wirewidth){
                        ambient+=0.1;
                        color.rgb = 0;
                        color.a = 1;
                    }
                }
#endif

                fixed3 diffuse=_LightColor0.rgb*col.rgb*saturate(max(dot(worldNormal,worldLightDir), dot(worldNormalInv, worldLightDir))* 0.5 + 0.5)*lightAttention;
                fixed3 halfDir=normalize(worldLightDir+viewDir);
                float specularTerm = pow(max(dot(worldNormalInv,halfDir),dot(worldNormal,halfDir)),_Gloss)*lightAttention;
                fixed3 specular=_LightColor0.rgb*_Specular.rgb*specularTerm;
#if GHOST
               float VdN =dot(viewDir,worldNormal);
               if(VdN>0)
                    col.a =pow( (1-VdN),2*(1-col.a)+1);

                if(VdN<0)
                    col.a =pow( (1-abs(VdN)),2*(1-col.a)+1);
#endif


                float viewAttent = 1;
                if(dot(viewDir,worldNormal)<0){
                    viewAttent = 0.5;
                }
                color = fixed4((color.rgb+ ambient + diffuse*viewAttent + specular).xyz, col.a);


#if DISPLAY_BRUSH
                float brushDist = length(i.worldPos - BrushPosition);
                float brushEdgeDist = abs(brushDist- BrushRadius);
                if (brushEdgeDist<0.01) {
                    
                    color = lerp(color, float4(1,0,0,1),1-brushEdgeDist*100)*float4(diffuse,1);
                    color +=0.2*  dot(worldNormal,viewDir) ;
                }
#endif

                //createFragmentEntryWithIndex(color, i.pos.xyz, uCoverage,_Index);

                return color;
            }
            ENDCG
        }
    }
    FallBack "Unlit/Transparent"
}

