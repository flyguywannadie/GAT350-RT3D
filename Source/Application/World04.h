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
		float intensity;
		float range;
		float innerAngle;
		float outerAngle;
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
		
		//light_t m_light;
		light_t m_lights[3];
		int m_selected = 0;

		Transform m_transform;
		res_t<Model> m_model;
		res_t<Material> m_material;

		glm::vec3 lightAmbient = {0.5f, 0.5f, 0.5f};
	};
}