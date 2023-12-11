add_executable(demo WIN32
        main.cpp
        shader_compiler.cpp
)

set_property(TARGET demo PROPERTY DEBUG_POSTFIX _d)

target_link_libraries(demo PRIVATE ${COMMON_LIBS})