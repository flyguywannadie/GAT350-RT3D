#pragma once
#include "Framework/World.h"
#include <vector>
#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"

namespace nc
{
	struct light_t
	{
		enum lightType
		{
			Point, Directional, Spot
		};

		lightType type;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		float cutoff;
	};

	class World04 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 1.1f;
		
		light_t m_light;

		Transform m_transform;
		res_t<Model> m_model;

		glm::vec3 lightAmbient = {0.5f, 0.5f, 0.5f};
	};
}