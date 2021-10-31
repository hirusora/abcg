project(danmaku)

add_executable(${PROJECT_NAME}
    main.cpp
    openglwindow.cpp
    entities/ship.cpp
    entities/shots.cpp
    entities/enemy.cpp
    entities/bullets.cpp
    entities/healthbar.cpp
    entities/stars.cpp)

enable_abcg(${PROJECT_NAME})