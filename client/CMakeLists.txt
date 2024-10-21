target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    constant_looper.cpp
    renderables/animation.cpp
    camera.cpp
    clientProtocol.cpp
    PUBLIC
    # .h files
    constant_looper.h
    camera.h
    renderables/animation.h
    renderables/renderable_duck.h
    clientProtocol.h
    )
