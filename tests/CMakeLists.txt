target_sources(taller_client_tests
    PRIVATE
    # .cpp files
    client_protocol_tests.cpp
    ../client/client_protocol.cpp
    PUBLIC
    ../client/client_protocol.h
    # .h files
    )

target_sources(taller_server_tests
    PRIVATE
    # .cpp files
    server_protocol_tests.cpp
    ../server/server_protocol.cpp
    PUBLIC
    ../server/server_protocol.h
    # .h files
    )
