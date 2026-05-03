#include <GameObject/game_object.h>
#include <Components/component.h>
#include <Components/transform.h>
#include <atomic>
#include <Components/rigidbody.h>
#include <Scripting/script_manager.h>

DEFINE_LOG_CATEGORY(LogGameObject)

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Math;
using namespace MirageEngine::Core::Scripting;

static std::atomic<uint64_t> s_NextID = 1;

uint64_t GenerateUniqueID() {
	return s_NextID++;
}

MirageEngine::GameObject::GameObject(std::shared_ptr<GameObject> _parent, std::string _name, bool _isStatic, bool _isActive)
	: m_Parent(_parent),
	m_Name(_name),
	m_bIsStatic(_isStatic),
	m_bIsActive(_isActive)
{
	m_ID = GenerateUniqueID();

	ScriptManager::Instance().SubscribePreReload(this, [this]() { OnPreReload(); });
    ScriptManager::Instance().SubscribePostReload(this, [this]() { OnPostReload(); });
}

MirageEngine::GameObject::~GameObject() {
	ScriptManager::Instance().Unsubscribe(this);
	m_Components.clear();
}

void MirageEngine::GameObject::AddChild(std::shared_ptr<GameObject> _child, bool _isStatic, bool _isActive)
{
	//if (_child->GetParent() != nullptr)
	//	_child->GetParent()->RemoveChild(_child);

	//_child->SetParent(this);
	m_Children.push_back(_child);

	_child->m_bIsStatic = _isStatic;
	_child->m_bIsActive = _isActive;
}

//MirageEngine::GameObject::~GameObject()
//{
//	for (auto& component : m_Components)
//	{
//		delete component;
//	}
//	m_Components.clear();
//	for (auto& child : m_Children)
//	{
//		delete child;
//	}
//	m_Children.clear();
//}

void MirageEngine::GameObject::SetActive(bool _isActive)
{
	if (_isActive)
	{
		m_bIsActive = true;
		std::cout << "GameObject is now active." << std::endl;
	}
	else
	{
		m_bIsActive = false;
		std::cout << "GameObject is now inactive." << std::endl;
	}
}
void MirageEngine::GameObject::AddComponent(std::shared_ptr<MirageEngine::Core::Components::Component> _component)
{
	_component->m_Owner = this;
	if (auto* script = dynamic_cast<Core::Scripting::IScript*>(_component.get())) {
		m_ScriptNames.push_back(script->GetName());
	}
	m_Components.push_back(_component);
}
void MirageEngine::GameObject::RemoveComponent(std::shared_ptr<MirageEngine::Core::Components::Component> _component)
{
	m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), _component), m_Components.end());
}

void MirageEngine::GameObject::RemoveChild(std::shared_ptr<GameObject> _child)
{
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), _child), m_Children.end());
}

void MirageEngine::GameObject::SetRotation(MirageEngine::Core::Math::Quaternion _rotation)
{
	m_bIsDirty = true;
	Transform.SetRotation(_rotation);
	if (auto rb = GetComponent<MirageEngine::Core::Components::Rigidbody>()) rb->OnTransformChanged(Transform);
}

void MirageEngine::GameObject::SetPosition(MirageEngine::Core::Math::Vector3 _pos)
{
	m_bIsDirty = true;
	Transform.SetPosition(_pos);
	if (auto rb = GetComponent<MirageEngine::Core::Components::Rigidbody>()) rb->OnTransformChanged(Transform);
}

void MirageEngine::GameObject::SetScale(MirageEngine::Core::Math::Vector3 _scale)
{
	m_bIsDirty = true;
	Transform.SetScale(_scale);
	if (auto rb = GetComponent<MirageEngine::Core::Components::Rigidbody>()) rb->OnTransformChanged(Transform);
}

void MirageEngine::GameObject::SetPosition(float _x, float _y, float _z)
{
	SetPosition(MirageEngine::Core::Math::Vector3(_x, _y, _z));
}

void MirageEngine::GameObject::SetScale(float _x, float _y, float _z)
{
	SetScale(MirageEngine::Core::Math::Vector3(_x, _y, _z));
}

void MirageEngine::GameObject::SetDirty(bool _isDirty)
{
	m_bIsDirty = _isDirty;
	if (_isDirty)
	{
		Transform.SetDirty(true);
	}
}

void MirageEngine::GameObject::OnPreReload()
{
    m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(), [](const std::shared_ptr<Component>& c) { return dynamic_cast<IScript*>(c.get()) != nullptr; }), m_Components.end());
}

void MirageEngine::GameObject::OnPostReload()
{
    auto& sm = ScriptManager::Instance();

    std::vector<std::string> copy = m_ScriptNames;
    m_ScriptNames.clear();
    for (auto& name : copy)
    {
        auto raw = sm.CreateScriptByName(name.c_str());
        if (raw) AddComponent(std::move(raw));
        else LOG(logWarning, LogGameObject, "Script ", name, " not found after reload");
    }
}
