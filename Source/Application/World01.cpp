#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World01::Initialize()
    {
        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt)
    {

        m_angle += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_H) ? dt * 50 : 0);
        m_angle += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Y) ? -dt * 50 : 0);

        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_J)) { m_angle_x = 1; }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_U)) { m_angle_x = 0; }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_K)) { m_angle_y = 1; }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_I)) { m_angle_y = 0; }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_L)) { m_angle_z = 1; }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_O)) { m_angle_z = 0; }

        m_position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? dt : 0);
        m_position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0);
        m_position.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt : 0);
        m_position.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? dt : 0);
        m_position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? -dt : 0);
        m_position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? dt : 0);

        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_T) && !ENGINE.GetSystem<InputSystem>()->GetPreviousKeyDown(SDL_SCANCODE_T)) m_timerot = !m_timerot;

        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Z)) { m_scale_x += dt; }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_X)) { m_scale_x += -dt; }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_C)) { m_scale_y += dt; }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_V)) { m_scale_y += -dt; }

        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_N) && !ENGINE.GetSystem<InputSystem>()->GetPreviousKeyDown(SDL_SCANCODE_N)) { 
            m_actualPoints.push_back(glm::vec3{-m_position.x,-m_position.y, (float)random(1,3)});
        }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_M) && !ENGINE.GetSystem<InputSystem>()->GetPreviousKeyDown(SDL_SCANCODE_M)) { m_actualPoints.pop_back(); }

        m_time += dt;
    }

    void World01::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        if (m_actualPoints.size() > 0) {
            glPushMatrix();
            glTranslatef(m_position.x, m_position.y, m_position.z);
            if (!m_timerot) {
                glRotatef(m_angle, m_angle_x, m_angle_y, m_angle_z);
            }
            else {
                glRotatef(m_time * 50, m_angle_x, m_angle_y, m_angle_z);
            }
            glScalef(m_scale_x, m_scale_y, 1);

            glBegin(GL_POLYGON);

            for (int i = 0; i < m_actualPoints.size(); i++) {
                if ((int)m_actualPoints[i].z == 1) {
                    glColor3f(1, 0, 0);
                }
                else if ((int)m_actualPoints[i].z == 2) {

                    glColor3f(0, 1, 0);
                }
                else {
                    glColor3f(0, 0, 1);
                }
                glVertex2f(m_actualPoints[i].x, m_actualPoints[i].y);
            }

            glEnd();

            glPopMatrix();
        }

        // post-render
        renderer.EndFrame();
    }
}
