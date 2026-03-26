add_library(engine_core STATIC)

target_sources(engine_core
    PRIVATE
        src/Logger.cpp
        src/Assert.cpp
)

target_include_directories(engine_core
    PUBLIC
        include
)