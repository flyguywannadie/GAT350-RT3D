#include "World02.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#define INTERLEAVE

namespace nc
{
    bool World02::Initialize()
    {
        // shaders
        const char* vertexShader =
            "#version 430\n"
            "layout (location=0) in vec3 position;"
            "layout (location=1) in vec3 color;"
            "layout (location=0) out vec3 ocolor;"
            "void main() {"
            "  ocolor = color;"
            "  gl_Position = vec4(position, 1.0);"
            "}";

        const char* fragmentShader =
            "#version 430\n"
            "layout (location=0) in vec3 color;"
            "out vec4 ocolor;"
            "void main() {"
            "  ocolor = vec4(color, 1);"
            "}";

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShader, NULL);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShader, NULL);
        glCompileShader(fs);

        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glUseProgram(program);

#ifdef INTERLEAVE
        //vertex data  (position,   color)
        float vertexData[] = {
            -0.25f, -0.25f, 0.0f,   1.0f, 0.0f, 0.0f,
             0.25f, -0.25f, 0.0f,   0.0f, 1.0f, 0.0f,
             0.25f,  0.25f, 0.0f,   0.0f, 0.0f, 1.0f,
            -0.25f,  0.25f, 0.0f,   0.0f, 0.0f, 1.0f
        };

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 6 * sizeof(GLfloat));
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);

#else
        //vertex data
        float positionData[] = {
            -0.25f, -0.25f, 0.0f,
            0.25f, -0.25f, 0.0f,
            0.25f,  0.25f, 0.0f,
            -0.25f,  0.25f, 0.0f
        };

        float colorData[] =
        {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f
        };

        GLuint vbo[2];
        glGenBuffers(2, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(0, vbo[0], 0, 3 * sizeof(GLfloat));
        // color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(1, vbo[1], 0, 3 * sizeof(GLfloat));
#endif // INTERLEAVE

        return true;
    }

    void World02::Shutdown()
    {
    }

    void World02::Update(float dt)
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
            m_actualPoints.push_back(vec3{-m_position.x,-m_position.y, (float)random(1,3)});
        }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_M) && !ENGINE.GetSystem<InputSystem>()->GetPreviousKeyDown(SDL_SCANCODE_M)) { m_actualPoints.pop_back(); }

        m_time += dt;
    }

    void World02::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glBindVertexArray(m_vao);
        glDrawArrays(GL_QUADS, 0, 4);

        // post-render
        renderer.EndFrame();
    }
}
