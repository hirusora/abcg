project(sandan)

add_executable(${PROJECT_NAME} 
    main.cpp
    openglwindow.cpp
    camera.cpp
    entities/model.cpp
    entities/ship.cpp
    entities/shipcore.cpp
    entities/ufo.cpp
    entities/bullets.cpp
    entities/patterns/waveparticle.cpp)

enable_abcg(${PROJECT_NAME})