add_executable(app app.cpp)
target_compile_features(app PRIVATE cxx_std_20)
target_link_libraries(app PRIVATE computer_graphics)

add_custom_command(TARGET app PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:app>/resources)
