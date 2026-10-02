add_library(domain STATIC
    # Headers
    "include/domain/Vector2D.h"
    "include/domain/Entity.h"
    "include/domain/PhysicsBody.h"
    "include/domain/Tools/PointCharge.h"
    "include/domain/Tools/ChargedRod.h"
    "include/domain/BodyCreator.h"
    "include/domain/Creator/PointChargeCreator.h"
    "include/domain/Creator/ChargedRodCreator.h"

    # Sources
    "src/Vector2D.cpp"
    "src/Tools/PointCharge.cpp"
    "src/Tools/ChargedRod.cpp"
    "src/Creator/PointChargeCreator.cpp"
    "src/Creator/ChargedRodCreator.cpp"
 "include/domain/IBodyVisitor.h" "include/domain/PhysicSolver.h")

target_include_directories(domain PUBLIC include)
target_compile_features(domain PUBLIC cxx_std_20)