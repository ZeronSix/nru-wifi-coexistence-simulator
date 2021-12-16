add_executable(bianchi bianchi.cpp ${SIMULATOR_SRC})
set_target_properties(bianchi
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(scenario1 scenario1.cpp ${SIMULATOR_SRC})
set_target_properties(scenario1
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(scenario2 scenario2.cpp ${SIMULATOR_SRC})
set_target_properties(scenario2
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(scenario3 scenario3.cpp ${SIMULATOR_SRC})
set_target_properties(scenario3
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(scenario4 scenario4.cpp ${SIMULATOR_SRC})
set_target_properties(scenario4
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
