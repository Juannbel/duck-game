add_subdirectory(pre_lobby)
add_subdirectory(lobby)

file(GLOB CPP_SOURCES
    "*.cpp"
    "renderables/*.cpp"
    )

file(GLOB H_SOURCES
    "*.h"
    "renderables/*.h"
    )

target_sources(duck_game_client
    PRIVATE ${CPP_SOURCES}
    PUBLIC ${H_SOURCES}
    )
