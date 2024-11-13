file(GLOB CPP_SOURCES
    "*.cpp"
    "game/*.cpp"
    "game/guns/*.cpp"
    )

file(GLOB H_SOURCES
    "*.h"
    "game/*.h"
    "game/guns/*.h"
    )

target_sources(taller_server
    PRIVATE ${CPP_SOURCES}
    PUBLIC ${H_SOURCES}
    )
