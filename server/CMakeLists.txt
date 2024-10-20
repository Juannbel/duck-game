target_sources(taller_server
    PRIVATE
    # .cpp files
    main.cpp
    acceptor.cpp
    receiver.cpp
    sender.cpp
    server.cpp
    serverClient.cpp
    PUBLIC
    acceptor.h
    receiver.h
    sender.h
    server.h
    serverClient.h
    # .h files
    )
