
#pragma once

#include "domain/IBodyVisitor.h"
#include "domain/Tools/PointCharge.h"
#include "domain/Tools/ChargedRod.h"
#include <imgui.h>

namespace Flux::Presentation {

    class ImGuiInspectorVisitor : public Domain::IBodyVisitor {
    public:
        void visit(Domain::PointCharge& pc) override {
            float q = pc.getCharge();
            if (ImGui::DragFloat("Charge (q)", &q, 0.1f, -50.0f, 50.0f, "%.2f C")) {
                pc.setCharge(q);
            }
        }

        void visit(Domain::ChargedRod& rod) override {
            float q = rod.getTotalCharge();
            if (ImGui::DragFloat("Total Charge (Q)", &q, 0.1f, -100.0f, 100.0f, "%.2f C")) {
                rod.setTotalCharge(q);
            }

            float len = rod.getLength();
            if (ImGui::DragFloat("Length", &len, 1.0f, 5.0f, 500.0f, "%.1f px")) {
                rod.setLength(len);
            }

            float angle = rod.getAngle();
            if (ImGui::SliderAngle("Rotation", &angle)) {
                rod.setAngle(angle);
            }
        }
    };

}