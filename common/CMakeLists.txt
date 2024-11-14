file(GLOB CPP_SOURCES
    "*.cpp"
    )

file(GLOB H_SOURCES
    "*.h"
    )

target_sources(duck_game_common
    PRIVATE ${CPP_SOURCES}
    PUBLIC ${H_SOURCES}
    )
