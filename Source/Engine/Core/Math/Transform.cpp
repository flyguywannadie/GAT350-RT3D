#include "Transform.h"
#include "Renderer/Gui.h"

namespace nc
{
	void Transform::Read(const json_t& value)
	{
		READ_DATA(value, position);
		READ_DATA(value, scale);

		READ_DATA(value, eulerAngles);
		rotation = EulerToQuaternion(eulerAngles);
	}
	void Transform::ProcessGui()
	{
		ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
		if (ImGui::DragFloat3("Rotation", glm::value_ptr(eulerAngles), 0.1f)) {
			rotation = EulerToQuaternion(eulerAngles);
		}
		ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
	}
}
