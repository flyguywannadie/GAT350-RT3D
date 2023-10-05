#pragma once
#include "Framework/World.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include <vector>
#include "Renderer/Renderer.h"

namespace nc
{
	class World02 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_angle = 0;
		float m_angle_x = 0;
		float m_angle_y = 0;
		float m_angle_z = 0;
		float m_time;
		bool m_timerot = false;
		vec3 m_position;
		float m_scale_x = 1;
		float m_scale_y = 1;
		std::vector<vec3> m_actualPoints = { vec3{-0.5f,-0.5f, 1.0f},vec3{0.0f,0.5f, 2.0f},vec3{0.5f,-0.5, 3.0f} };
		GLuint m_vao;
	};
}