#include "RotateComponent.h"
#include "imgui/imgui.h"
#include "Framework/Actor.h"

namespace nc {
	CLASS_DEFINITION(RotateComponent)

	bool RotateComponent::Initialize()
	{
		return true;
	}

	void RotateComponent::Update(float dt)
	{
		m_owner->transform.eulerAngles = euler * dt;

		glm::quat rotation = EulerToQuaternion(euler * dt);
		m_owner->transform.rotation = m_owner->transform.rotation * rotation;
	}

	void RotateComponent::ProcessGui()
	{
		ImGui::DragFloat3("Rotate", glm::value_ptr(euler));
	}

	void RotateComponent::Read(const nc::json_t& value)
	{
		// read rotate component in
		READ_NAME_DATA(value, "rotation", euler);
	}
}