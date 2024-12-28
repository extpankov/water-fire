cmake_minimum_required(VERSION 3.14)
project(WaterFireGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SFML_DIR "/Library/Frameworks/SFML.framework/Resources/CMake")
set(SFML_INCLUDE_DIR "/Library/Frameworks/SFML.framework/Headers")
set(SFML_LIBRARY_DIR "/Library/Frameworks")

find_package(SFML 2.6 COMPONENTS system window graphics audio REQUIRED)

add_executable(${PROJECT_NAME}
    game/main.cpp
    game/GameEngine.cpp
    game/MenuState.cpp
    game/PlayState.cpp
    game/LevelSelectState.cpp
    game/Level.cpp
    game/Fire.cpp
    game/Water.cpp
    game/Door.cpp
    game/Lever.cpp
    game/Diamond.cpp
    game/Hint.cpp
    game/Animation.cpp
    game/MovingPlatform.cpp
    game/PressureButton.cpp
    game/ButtonPlatform.cpp
    game/PushableBlock.cpp
)

# Добавляем демонстрационное приложение для рычага
# add_executable(LeverDemo
#     game/LeverDemo.cpp
#     game/Lever.cpp
# )

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/game
    ${SFML_INCLUDE_DIR}
)

# target_include_directories(LeverDemo PRIVATE
#     ${CMAKE_SOURCE_DIR}/game
#     ${SFML_INCLUDE_DIR}
# )

# Линкуем SFML к основному приложению
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# Линкуем SFML к демо-приложению
# target_link_libraries(LeverDemo PRIVATE
#     sfml-graphics
#     sfml-window
#     sfml-system
# )

set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "@executable_path/../Frameworks;/Library/Frameworks"
    MACOSX_RPATH TRUE
)

if(APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -F /Library/Frameworks")
endif()

# Копируем ресурсы для основной игры
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/game/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/game/data
    ${CMAKE_BINARY_DIR}/data
)

# Добавляем тестовый исполняемый файл
# add_executable(WaterTest
#     game/WaterTest.cpp
#     game/Water.cpp
#     game/Animation.cpp
# )
# target_include_directories(WaterTest PRIVATE
#     game
#     ${SFML_INCLUDE_DIR}
# )
# target_link_libraries(WaterTest sfml-graphics sfml-window sfml-system)

# Добавляем тестовое приложение для двери
# add_executable(door_test
#     game/DoorTestApp.cpp
#     game/Door.cpp
# )
# target_link_libraries(door_test sfml-graphics sfml-window sfml-system)

# Копируем ресурсы для тестового приложения
# add_custom_command(TARGET door_test POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/game/data $<TARGET_FILE_DIR:door_test>/data
# )