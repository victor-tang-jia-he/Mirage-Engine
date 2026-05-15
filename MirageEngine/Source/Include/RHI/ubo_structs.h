#pragma once
#include <matrix4.h>

// Struct send to the GPU
#define MAX_DIR_LIGHTS 4
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 8

using namespace MirageEngine::Core::Math;

namespace MirageRHI
{
    struct ALIGN16 PerFrameData
    {
        Matrix4 View;
        Matrix4 Projection;
        Vector3 CamPos;
        float Padding;
    };

    struct ALIGN16 PerObjectData
    {
        Matrix4 Model;
        Matrix4 NormalMatrix;
    };

    // Lights
    struct ALIGN16 DirectionalLightData
    {
        Vector3 Direction;
        float _Pad0;
        Vector4 Color;
        float Intensity;
        float _Pad1[3];
    };

    struct ALIGN16 PointLightData
    {
        Vector3 Position;
        float _Pad0;
        Vector4 Color;
        float Intensity;
        float _Pad1[3];
    };

    struct ALIGN16 SpotLightData
    {
        Vector3 position;
        float _pad0;
        Vector3 direction;
        float _pad1;
        Vector4 color;
        float intensity;
        float innerCutoff;
        float outerCutoff;
        float _pad2;
    };

    struct ALIGN16 LightUBOData
    {
            DirectionalLightData dirLights[MAX_DIR_LIGHTS];
            PointLightData pointLights[MAX_POINT_LIGHTS];
            SpotLightData spotLights[MAX_SPOT_LIGHTS];
            int dirCount;
            int pointCount;
            int spotCount;
            float padding;
    };

    // Mat
    struct ALIGN8 MaterialData
    {
            Vector3 albedo;
            float metallic;
            float roughness;
            float ao;
            float _pad[2];
    };
} // namespace MirageRHI
