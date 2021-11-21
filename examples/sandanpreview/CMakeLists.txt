project(sandanpreview)

add_executable(${PROJECT_NAME} 
    main.cpp
    openglwindow.cpp
    camera.cpp
    entities/model.cpp
    entities/bullets.cpp)

enable_abcg(${PROJECT_NAME})