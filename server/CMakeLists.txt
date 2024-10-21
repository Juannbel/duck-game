target_sources(taller_server
    PRIVATE
    # .cpp files
    main.cpp
    acceptor.cpp
    receiver.cpp
    sender.cpp
    server.cpp
    serverClient.cpp
    list_monitor.cpp
    game_loop.cpp
    PUBLIC
    acceptor.h
    receiver.h
    sender.h
    server.h
    serverClient.h
    # .h files
    list_monitor.h
    game_loop.h
    )
