cmake_minimum_required(VERSION 3.10)
project(SearchEngine)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置路径
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)
set(BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${BINARY_DIR})

# 包含目录
include_directories(${INCLUDE_DIR})

# 自动收集 include 目录的头文件
file(GLOB_RECURSE HEADER_FILES "${INCLUDE_DIR}/*.h" "${INCLUDE_DIR}/*.hpp")

# 自动收集 src 目录的源文件（排除子文件夹）
file(GLOB MAIN_SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE ALL_SOURCES "${SRC_DIR}/*.cpp")
list(REMOVE_ITEM ALL_SOURCES ${MAIN_SOURCES})

# 自动收集 test 目录的测试文件
file(GLOB TEST_SOURCES "${TEST_DIR}/*.cpp")

# 创建静态库
add_library(dictionary STATIC
        ${HEADER_FILES}
        ${ALL_SOURCES}
)

# 设置静态库的输出路径
set_target_properties(dictionary PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)

# 添加主程序
foreach(src_file ${MAIN_SOURCES})
    get_filename_component(target_name ${src_file} NAME_WE) # 获取文件名（无扩展名）
    add_executable(${target_name} ${src_file})             # 创建可执行文件
    target_link_libraries(${target_name} dictionary)       # 链接静态库
    set_target_properties(${target_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    )
endforeach()

# 添加测试程序
foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE) # 获取文件名（无扩展名）
    add_executable(${test_name} ${test_file})             # 创建可执行文件
    target_link_libraries(${test_name} dictionary)        # 链接静态库
    set_target_properties(${test_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    )
endforeach()

# 安装主程序和库
install(TARGETS dictionary DESTINATION lib)

# 安装主程序可执行文件
foreach(src_file ${MAIN_SOURCES})
    get_filename_component(target_name ${src_file} NAME_WE)
    install(TARGETS ${target_name} DESTINATION bin)
endforeach()

# 安装测试程序可执行文件
foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    install(TARGETS ${test_name} DESTINATION bin)
endforeach()
