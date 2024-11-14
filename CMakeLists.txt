cmake_minimum_required(VERSION 3.10)
project(dictionaryProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含目录
include_directories(include/dictionary)

# 设置输出目录
set(BINARY_DIR bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/${BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/${BINARY_DIR})

# 添加可执行文件 main
add_executable(main src/Dictionary.cpp)
target_link_libraries(main dictionary)

# 设置 main 可执行文件的输出目录
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

# 添加测试目标
file(GLOB TEST_SOURCES "test/*.cpp")
foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} dictionary)
endforeach()

# 设置测试目标的输出目录
foreach(test_name ${TEST_SOURCES})
    get_filename_component(test_name ${test_name} NAME_WE)
    set_target_properties(${test_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    )
endforeach()

# 添加库
add_library(dictionary STATIC
    include/dictionary/Configuration.h
    include/dictionary/DicProducer.h
    include/dictionary/Dictionary.h
    include/dictionary/SplitTool.h
    include/dictionary/SplitToolCppJieba.h
    src/dictionary/DicProducer.cpp
    src/dictionary/SplitTool.cpp
    src/dictionary/SplitToolCppJieba.cpp
)

# 设置库的输出目录
set_target_properties(dictionary PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)

# 安装目标
install(TARGETS main dictionary DESTINATION bin)