target_sources(taller_common
    PRIVATE
    # .cpp files
    socket.cpp
    resolver.cpp
    resolvererror.cpp
    liberror.cpp
    map_loader.cpp
    PUBLIC
    # .h files
    commands.h
    snapshot.h
    socket.h
    resolver.h
    resolvererror.h
    liberror.h
    map_dto.h
    map.h
    map_loader.h
    )
