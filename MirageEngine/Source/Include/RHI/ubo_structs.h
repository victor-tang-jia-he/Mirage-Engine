    #pragma once
#include <matrix4.h>
using namespace MirageEngine::Core::Math;
// Struct send to the GPU
#define MAX_DIR_LIGHTS 4
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 8

namespace MirageRHI
{

    struct ALIGN16 PerFrameData
    {
            Matrix4 view;
            Matrix4 projection;
            Vector3 camPos;
            float padding;
    };

    struct ALIGN16 PerObjectData
    {
            Matrix4 model;
            Matrix4 normalMatrix;
    };

    // Lights
    struct ALIGN16 DirectionalLightData
    {
            Vector3 direction;
            float _pad0;
            Vector4 color;
            float intensity;
            float _pad1[3];
    };

    struct ALIGN16 PointLightData
    {
            Vector3 position;
            float _pad0;
            Vector4 color;
            float intensity;
            float _pad1[3];
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
