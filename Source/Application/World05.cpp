#include "World05.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtx/color_space.hpp>

namespace nc
{
    bool World05::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene.json");
        m_scene->Initialize();

        //{
        //    auto actor = CREATE_CLASS(Actor);
        //    actor->name = "actor1";
        //    actor->transform.position = glm::vec3{ 0, 0, 0 };
        //    auto modelComponent = CREATE_CLASS(ModelComponent);
        //    modelComponent->model = std::make_shared<Model>();
        //    modelComponent->model->SetMaterial(GET_RESOURCE(Material, "materials/squirrel.mtrl"));
        //    modelComponent->model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
        //    actor->AddComponent(std::move(modelComponent));
        //    m_scene->Add(std::move(actor));
        //}

        //{
        //    auto actor = CREATE_CLASS(Actor);
        //    actor->name = "actor2";
        //    actor->transform.position = glm::vec3{ 0, 0, 5 };
        //    auto modelComponent = CREATE_CLASS(ModelComponent);
        //    modelComponent->model = std::make_shared<Model>();
        //    modelComponent->model->SetMaterial(GET_RESOURCE(Material, "materials/squirrel.mtrl"));
        //    modelComponent->model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
        //    actor->AddComponent(std::move(modelComponent));
        //    m_scene->Add(std::move(actor));
        //}

        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "light1";
            actor->transform.position = glm::vec3{ 3, 3, 3 };
            auto lightComponent = CREATE_CLASS(LightComponent);
            lightComponent->type = LightComponent::eType::Point;
            lightComponent->color = glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
            lightComponent->intensity = 1;
            lightComponent->range = 20;
            lightComponent->innerAngle = 10.0f;
            lightComponent->outerAngle = 30.0f;
            actor->AddComponent(std::move(lightComponent));
            m_scene->Add(std::move(actor));
        }
        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "camera1";
            actor->transform.position = glm::vec3{ 0, 0, 0 };
            actor->transform.rotation = glm::vec3{ 0, 180, 0 };

            auto cameraComponent = CREATE_CLASS(CameraComponent);
            cameraComponent->SetPerspective(70.0f, ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.1f, 100.0f);
            actor->AddComponent(std::move(cameraComponent));

            m_scene->Add(std::move(actor));
        }

        return true;
    }

    void World05::Shutdown()
    {
    }

    void World05::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        auto actor = m_scene->GetActorByName<Actor>("actor1");

        //actor->transform.rotation.z += (180 * dt);
        //m_time += dt;

        actor->transform.position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? -dt * m_speed : 0);
        actor->transform.position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? dt * m_speed : 0);
        actor->transform.position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? -dt * m_speed : 0);
        actor->transform.position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? dt * m_speed : 0);

       // m_transform.rotation.x += dt * 100;
        //m_transform.rotation.y += dt * 100;
        //m_transform.rotation.z += dt * 100;

        // texture tiling and offset
        auto material = actor->GetComponent<ModelComponent>()->model->GetMaterial();
        ///material->ProcessGui();
        material->Bind();

        material->GetProgram()->SetUniform("ambientColor", lightAmbient);

        material = GET_RESOURCE(Material, "materials/refraction.mtrl");
        if (material) {
            ImGui::Begin("Refraction");
            ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1, 3);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("ior", m_refraction);

            ImGui::End();
        }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World05::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_scene->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
