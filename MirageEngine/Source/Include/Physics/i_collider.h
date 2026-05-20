// i_collider.h
#pragma once

#include <Physics/common_enum.h>
#include <Components/component.h>
#include <Components/transform.h>

namespace MirageEngine::Core::Physics
{
    class IRigidBody;
    using Vector3 = Math::Vector3;
    using Quaternion = Math::Quaternion;

    //=========================================================================
    // Collider-specific interfaces
    //=========================================================================
    class ISphereCollider
    {
    public:
        virtual ~ISphereCollider() = default;
        virtual void SetRadius(float _radius) = 0;
        virtual float GetRadius() const = 0;
    };

    class IBoxCollider
    {
    public:
        virtual ~IBoxCollider() = default;
        virtual void SetHalfExtents(const Vector3& _halfExtents) = 0;
        virtual Vector3 GetHalfExtents() const = 0;
    };

    class ICapsuleCollider
    {
    public:
        virtual ~ICapsuleCollider() = default;
        virtual void SetRadius(float _radius) = 0;
        virtual void SetHeight(float _height) = 0;
        virtual float GetRadius() const = 0;
        virtual float GetHeight() const = 0;
    };

	//=========================================================================
	// ICollider - Base collider interface
	//=========================================================================
    class ICollider : public std::enable_shared_from_this<ICollider>
    {
    protected:
        Vector3 p_Scale = Vector3(1.f);
        Vector3 p_Offset = Vector3(0.f);
        Quaternion p_Rot = Quaternion(0.f, 0.f, 0.f, 1.f);
        ECollisionShapeType p_Type;

        std::weak_ptr<IRigidBody> p_RigidBody;

        bool p_bIsDirty = true;

    public:
        virtual ~ICollider() = default;

        virtual void Create() = 0;

        virtual void SetOffset(Vector3 _offset, Quaternion _rotation) = 0;
        bool SetRigidbody(std::shared_ptr<IRigidBody> _rigidbody);

        void SetScale(Vector3 _scale)
        {
            p_Scale = _scale;
            p_bIsDirty = true;
        }

        void SetTranslate(Vector3 _offset)
        {
            p_Offset = _offset;
            p_bIsDirty = true;
        }

        void SetRotation(Quaternion _rotation)
        {
            p_Rot = _rotation;
            p_bIsDirty = true;
        }

        void SetIsDirty(bool _value) { p_bIsDirty = _value; }

        Vector3 GetScale() const { return p_Scale; }
        Vector3 GetTranslate() const { return p_Offset; }
        Quaternion GetRotation() const { return p_Rot; }
        bool GetIsDirty() const { return p_bIsDirty; }
        ECollisionShapeType GetColliderType() const { return p_Type; }

        template <typename T> T* As() { return dynamic_cast<T*>(this); }
    };
} // namespace MirageEngine::Core::Physics
