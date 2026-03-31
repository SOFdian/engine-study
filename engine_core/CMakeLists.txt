add_library(engine_core STATIC)

target_sources(engine_core
    PRIVATE
        src/Logger.cpp
        src/Assert.cpp
        include/core/MemoryBuffer.hpp
)

target_include_directories(engine_core
    PUBLIC
        include
)