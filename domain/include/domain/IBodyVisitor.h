#pragma once

namespace Flux::Domain {
    class PointCharge;
    class ChargedRod;
   

    class IBodyVisitor {
    public:
        virtual ~IBodyVisitor() = default;
        virtual void visit(const PointCharge& body) {}
        virtual void visit(const ChargedRod& body) {}

        virtual void visit(PointCharge& body) {}
        virtual void visit(ChargedRod& body) {}
    };
}