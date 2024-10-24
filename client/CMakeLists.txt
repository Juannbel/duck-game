target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    constant_looper.cpp
    renderables/animation.cpp
    renderables/duck.cpp
    renderables/equipped_gun.cpp
    renderables/collectable.cpp
    renderables/map.cpp
    camera.cpp
    client_protocol.cpp
    client.cpp
    client_receiver.cpp
    client_sender.cpp
    textures_provider.cpp
    PUBLIC
    # .h files
    constant_looper.h
    camera.h
    renderables/animation.h
    renderables/duck.h
    renderables/equipped_gun.h
    renderables/collectable.h
    renderables/map.h
    client_protocol.h
    client.h
    client_receiver.h
    client_sender.h
    textures_provider.h

    )
