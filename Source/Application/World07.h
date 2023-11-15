#pragma once
#include "Framework/World.h"
#include <vector>
#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"

namespace nc
{
	class World07 : public World
	{
	public:
		const uint32_t INVERT_MASK		= (1 << 0);
		const uint32_t GRAYSCALE_MASK	= (1 << 1);
		const uint32_t COLOR_TINT_MASK	= (1 << 2);
		const uint32_t SCANLINE_MASK	= (1 << 3);

	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 1.1f;
		float m_blend = 0;

		uint32_t params{ 0 };
		glm::vec3 tint_color{ 1 };

		float m_refraction{ 1 };
		glm::vec3 lightAmbient = {0.5f, 0.5f, 0.5f};
	};
}