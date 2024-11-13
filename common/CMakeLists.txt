file(GLOB CPP_SOURCES
    "*.cpp"
    )

file(GLOB H_SOURCES
    "*.h"
    )

target_sources(taller_common
    PRIVATE ${CPP_SOURCES}
    PUBLIC ${H_SOURCES}
    )
