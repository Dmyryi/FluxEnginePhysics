add_library(application STATIC
    "include/application/UseCase/SpawnBodyUseCase.h"
    "src/UseCase/SpawnBodyUseCase.cpp"
 "include/application/UseCase/StepSimulationUseCase.h" "src/UseCase/StepSimulationUseCase.cpp")

target_include_directories(application PUBLIC include)
target_compile_features(application PUBLIC cxx_std_20)
target_link_libraries(application PUBLIC domain)