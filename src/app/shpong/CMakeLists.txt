# Same project name as root project
#project(shpong)

set(${PROJECT_NAME}_HEADERS
    game.h
    paragame.h

    BrickDrawer.h
    GenericDrawer.h
    Intersection.h
)

set(${PROJECT_NAME}_SOURCES
    main.cpp

    BrickDrawer.cpp
    game.cpp
    GenericDrawer.cpp
)

find_package(glfw3 3.3 CONFIG REQUIRED)
find_package(2d CONFIG REQUIRED COMPONENTS engine)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        ad::engine
)
