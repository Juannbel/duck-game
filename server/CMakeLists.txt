target_sources(taller_server
    PRIVATE
    # .cpp files
    main.cpp
    acceptor.cpp
    receiver.cpp
    sender.cpp
    server.cpp
    ServerClient.cpp
    list_monitor.cpp
    game_loop.cpp
    serverProtocol.cpp
    PUBLIC
    # .h files
    acceptor.h
    receiver.h
    sender.h
    server.h
    ServerClient.h
    list_monitor.h
    game_loop.h
    serverProtocol.h
    )
