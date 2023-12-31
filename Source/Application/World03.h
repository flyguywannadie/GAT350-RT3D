#pragma once
#include "Framework/World.h"
#include <vector>
#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"

namespace nc
{
	class World03 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 1.1f;

		Transform m_transform;

		res_t<VertexBuffer> m_vertexBuffer;
		res_t<Material> m_material;
		res_t<Program> m_program;
		res_t<Texture> m_texture;
	};
}