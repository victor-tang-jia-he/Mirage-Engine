#pragma once

#include "bullet3_pch.h"

namespace MirageEngine::Core::Physics
{
    class BulletDebugDrawer : public btIDebugDraw
    {
        int m_DebugMode = DBG_DrawWireframe;

    public:
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

        void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB,
            btScalar distance, int lifeTime, const btVector3& color) override {
        }
        void reportErrorWarning(const char* warningString) override {}
        void draw3dText(const btVector3& location, const char* textString) override {}
        void setDebugMode(int debugMode) override { m_DebugMode = debugMode; }
        int  getDebugMode() const override { return m_DebugMode; }
    };
}