target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    constant_looper.cpp
    renderables/animation.cpp
    camera.cpp
    clientProtocol.cpp
    client.cpp
    client_receiver.cpp
    client_sender.cpp
    PUBLIC
    # .h files
    constant_looper.h
    camera.h
    renderables/animation.h
    renderables/duck.h
    clientProtocol.h
    client.h
    client_receiver.h
    client_sender.h

    )
