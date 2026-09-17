#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <vector>
#include <memory>
#include <algorithm>
#include <string>

#include "presentation/Camera2D.h"
#include "domain/Creator/PointChargeCreator.h"
#include "domain/Creator/ChargedRodCreator.h"
#include "domain/Tools/PointCharge.h"
#include "domain/Tools/ChargedRod.h"
#include "../application/include/application/UseCase/SpawnBodyUseCase.h"
#include "../application/include/application/UseCase/StepSimulationUseCase.h"
#include "../presentation/include/presentation/Renderer/SfmlBodyRenderer.h"

enum class ToolType {
    PointCharge,
    ChargedRod,
    ConductorPlate,
    Measure
};

int main() {
    sf::RenderWindow window(sf::VideoMode({ 1600u, 900u }), "ELECTRO_DYNAMICS_2D - Physics Engine");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window, false)) {
        return -1;
    }

    ImGui::GetIO().Fonts->AddFontDefault();
    if (!ImGui::SFML::UpdateFontTexture()) {
        return -1;
    }

    Flux::Presentation::Camera2D camera(1600.0f, 900.0f);
    sf::Clock deltaClock;

    std::vector<std::unique_ptr<Flux::Domain::PhysicsBody>> bodies;
    Flux::Application::SpawnBodyUseCase spawnUseCase(bodies);
    Flux::Application::StepSimulationUseCase stepSimulation;

    Flux::Domain::PointChargeCreator pointCreator;
    Flux::Domain::ChargedRodCreator rodCreator;

    // Состояние инструментов и симуляции
    ToolType activeTool = ToolType::PointCharge;
    bool isRunning = true;
    float timeStep = 0.016f;
    float totalSimTime = 0.0f;
    float coulombK = 50000.0f;
    float dragCoeff = 0.5f;

    // Параметры создания
    float spawnCharge = 1.0f;
    float spawnMass = 1.0f;

    // ID выбранного объекта для инспектора
    std::uint64_t selectedId = 0;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (!ImGui::GetIO().WantCaptureMouse) {
                camera.handleEvent(*event, window);

                if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseBtn->button == sf::Mouse::Button::Left) {
                        camera.setViewTo(window);
                        sf::Vector2f worldPos = window.mapPixelToCoords(mouseBtn->position);
                        Flux::Domain::Vector2D spawnPos(worldPos.x, worldPos.y);

                        if (activeTool == ToolType::PointCharge) {
                            pointCreator.setCharge(spawnCharge);
                            spawnUseCase.execute(pointCreator, spawnPos);
                            if (!bodies.empty()) {
                                bodies.back()->setMass(spawnMass);
                                selectedId = bodies.back()->getId();
                            }
                        }
                        else if (activeTool == ToolType::ChargedRod) {
                            spawnUseCase.execute(rodCreator, spawnPos);
                            if (!bodies.empty()) {
                                selectedId = bodies.back()->getId();
                            }
                        }
                    }
                }
            }

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                camera.resetSize(static_cast<float>(resized->size.x), static_cast<float>(resized->size.y));
            }
        }

        sf::Time elapsed = deltaClock.restart();
        float dt = elapsed.asSeconds();
        if (dt > 0.05f) dt = 0.05f;

        ImGui::SFML::Update(window, elapsed);

        // --- Физика ---
        if (isRunning) {
            stepSimulation.execute(bodies, dt, coulombK, dragCoeff);
            totalSimTime += dt;
        }

        // ================= UI ПАНЕЛИ =================

        // 1. Верхний Toolbar
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window.getSize().x), 55.0f), ImGuiCond_Always);
        ImGui::Begin("TopBar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        {
            if (ImGui::Button(isRunning ? "Pause ||" : "Play >")) {
                isRunning = !isRunning;
            }
            ImGui::SameLine();
            if (ImGui::Button("Step >|")) {
                stepSimulation.execute(bodies, timeStep, coulombK, dragCoeff);
                totalSimTime += timeStep;
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset")) {
                bodies.clear();
                totalSimTime = 0.0f;
                selectedId = 0;
            }
            ImGui::SameLine();
            ImGui::Text("Time: %.2f s", totalSimTime);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(90.0f);
            ImGui::DragFloat("TimeStep", &timeStep, 0.001f, 0.001f, 0.1f, "%.3f s");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(120.0f);
            ImGui::SliderFloat("Coulomb K", &coulombK, 1000.0f, 200000.0f, "%.0f");
        }
        ImGui::End();

        // 2. Левая верхняя панель: Иерархия сцены
        ImGui::SetNextWindowPos(ImVec2(10, 65), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(240, 360), ImGuiCond_FirstUseEver);
        ImGui::Begin("Hierarchy / Assets");
        {
            if (ImGui::TreeNodeEx("Scene Nodes", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (ImGui::TreeNodeEx("Charges", ImGuiTreeNodeFlags_DefaultOpen)) {
                    for (const auto& b : bodies) {
                        std::string label = b->getName() + " ##" + std::to_string(b->getId());
                        bool isSelected = (selectedId == b->getId());
                        if (ImGui::Selectable(label.c_str(), isSelected)) {
                            selectedId = b->getId();
                        }
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();

        // 3. Левая нижняя панель: Свойства выбранного объекта (Inspector)
        ImGui::SetNextWindowPos(ImVec2(10, 435), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(240, 440), ImGuiCond_FirstUseEver);
        ImGui::Begin("Properties");
        {
            Flux::Domain::PhysicsBody* selectedBody = nullptr;
            for (auto& b : bodies) {
                if (b->getId() == selectedId) {
                    selectedBody = b.get();
                    break;
                }
            }

            if (selectedBody) {
                ImGui::Text("Selected: %s (ID: %llu)", selectedBody->getName().c_str(), selectedBody->getId());
                ImGui::Separator();

                // Масса
                float mass = selectedBody->getMass();
                if (ImGui::DragFloat("Mass (kg)", &mass, 0.1f, 0.01f, 1000.0f)) {
                    selectedBody->setMass(mass);
                }

                // Позиция
                auto pos = selectedBody->getPosition();
                float p[2] = { pos.x, pos.y };
                if (ImGui::DragFloat2("Position", p, 1.0f)) {
                    selectedBody->setPosition({ p[0], p[1] });
                }

                // Скорость
                auto vel = selectedBody->getVelocity();
                float v[2] = { vel.x, vel.y };
                if (ImGui::DragFloat2("Velocity", v, 1.0f)) {
                    selectedBody->setVelocity({ v[0], v[1] });
                }

                // Пин (гвоздь)
                bool pinned = selectedBody->isPinned();
                if (ImGui::Checkbox("Pinned (Fixed)", &pinned)) {
                    selectedBody->setPinned(pinned);
                }

                // Специфичные поля для точечного заряда
                if (auto* pc = dynamic_cast<Flux::Domain::PointCharge*>(selectedBody)) {
                    float q = pc->getCharge();
                    if (ImGui::DragFloat("Charge (q)", &q, 0.1f, -50.0f, 50.0f)) {
                        pc->setCharge(q);
                    }
                }
            }
            else {
                ImGui::TextDisabled("Select an object from Hierarchy");
            }
        }
        ImGui::End();

        // 4. Правая верхняя панель: Инструменты (Tools)
        float screenW = static_cast<float>(window.getSize().x);
        ImGui::SetNextWindowPos(ImVec2(screenW - 250, 65), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(240, 360), ImGuiCond_FirstUseEver);
        ImGui::Begin("Field Editor / Tools");
        {
            if (ImGui::RadioButton("Charge Tool", activeTool == ToolType::PointCharge)) activeTool = ToolType::PointCharge;
            if (ImGui::RadioButton("Charged Rod", activeTool == ToolType::ChargedRod)) activeTool = ToolType::ChargedRod;
            if (ImGui::RadioButton("Conductor Plate", activeTool == ToolType::ConductorPlate)) activeTool = ToolType::ConductorPlate;
            if (ImGui::RadioButton("Measure", activeTool == ToolType::Measure)) activeTool = ToolType::Measure;

            ImGui::Separator();
            ImGui::Text("Spawn Settings:");
            ImGui::SliderFloat("Charge (q)", &spawnCharge, -10.0f, 10.0f, "%.1f C");
            ImGui::SliderFloat("Mass (m)", &spawnMass, 0.1f, 20.0f, "%.1f kg");
        }
        ImGui::End();

        // 5. Правая нижняя панель: Графики (Заглушка под ImPlot)
        ImGui::SetNextWindowPos(ImVec2(screenW - 250, 435), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(240, 440), ImGuiCond_FirstUseEver);
        ImGui::Begin("Graphing");
        {
            ImGui::Text("Current vs. Time");
            static float dummyValues[20] = { 0.1f, 0.2f, 0.15f, 0.4f, 0.6f, 0.5f, 0.8f, 0.7f, 0.9f, 0.6f, 0.4f, 0.2f, 0.1f, 0.05f, 0.0f, -0.1f, 0.0f, 0.2f, 0.5f, 0.3f };
            ImGui::PlotLines("##CurrentPlot", dummyValues, 20, 0, nullptr, -0.5f, 1.0f, ImVec2(200, 100));

            ImGui::Separator();
            ImGui::Text("Field Intensity E_x vs. X");
            static float dummyField[20] = { 0.0f, 0.05f, 0.1f, 0.2f, 0.4f, 0.8f, 1.0f, 0.8f, 0.4f, 0.2f, 0.1f, 0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
            ImGui::PlotLines("##FieldPlot", dummyField, 20, 0, nullptr, 0.0f, 1.2f, ImVec2(200, 100));
        }
        ImGui::End();

        // ================= РЕНДЕР СЦЕНЫ =================
        window.clear(sf::Color(22, 24, 30));

        camera.setViewTo(window);
        camera.renderGrid(window);

        Flux::Presentation::SfmlBodyRenderer bodyRenderer(window);
        for (const auto& body : bodies) {
            body->accept(bodyRenderer);
        }

        window.setView(window.getDefaultView());
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}