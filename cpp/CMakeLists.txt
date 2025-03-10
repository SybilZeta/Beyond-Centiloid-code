# CMakeList.txt: CentiloidCalculator 的 CMake 项目，适用于跨平台构建
#
cmake_minimum_required(VERSION 3.8)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 项目名称
project(CentiloidCalculator)

# 查找 ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# 查找 SimpleITK
find_package(SimpleITK REQUIRED)
include(${SimpleITK_USE_FILE})

# 查找 ONNX Runtime
# Windows 使用 vcpkg，macOS/Linux 使用预编译二进制
find_package(onnxruntime QUIET)
if(NOT onnxruntime_FOUND)
    # 如果未找到，假设通过 CMAKE_PREFIX_PATH 或 vcpkg 提供
    message(STATUS "onnxruntime not found via find_package, relying on CMAKE_PREFIX_PATH or vcpkg")
endif()

# argparse 头文件目录（假设作为子目录或外部提供）
# 若使用 git submodule，可添加如下代码：
# add_subdirectory(external/argparse)
# 否则需通过参数传入
if(NOT DEFINED ARGPARSE_INCLUDE_DIR)
    set(ARGPARSE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/argparse/include")
endif()
include_directories(${ARGPARSE_INCLUDE_DIR})

# 添加源代码到可执行文件
add_executable(CentiloidCalculator 
    "main.cpp" 
    "Calculator.cpp" 
    "Calculator.h" 
    "Rigid.h" 
    "Rigid.cpp" 
    "Decoupler.cpp" 
    "Decoupler.h" 
    "common.h" 
    "common.cpp"
)

# 打印 ITK 库信息
message(STATUS "ITK_LIBRARIES: ${ITK_LIBRARIES}")

# 链接库
target_link_libraries(CentiloidCalculator PRIVATE 
    ${ITK_LIBRARIES} 
    ${SimpleITK_LIBRARIES} 
    onnxruntime
)

# 确保目标包含 ONNX Runtime 的头文件目录
if(onnxruntime_FOUND)
    target_include_directories(CentiloidCalculator PRIVATE ${onnxruntime_INCLUDE_DIRS})
endif()

# TODO: 如有需要，请添加测试并安装目标。