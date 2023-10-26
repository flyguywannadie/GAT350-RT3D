#pragma once
#include "Framework/World.h"
#include <vector>
#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"

namespace nc
{
	class World05 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 1.1f;

		glm::vec3 lightAmbient = {0.5f, 0.5f, 0.5f};
	};
}