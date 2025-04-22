cmake_minimum_required(VERSION 3.8)

# Set project properties
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(CMAKE_WIN32_EXECUTABLE TRUE)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Project definition
project("Spirytus")

# Define the resources folder
set(RESOURCE_PATH "${CMAKE_SOURCE_DIR}/Resources")

# Collect all source and resource files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
file(GLOB RESOURCE_FILES "${RESOURCE_PATH}/*.rc")  # Include .rc files from Resources

# Add the executable and include resource files
add_executable(Spirytus ${SOURCES} ${RESOURCE_FILES})

# Set the resource path for compilation
add_compile_definitions(RESOURCE_PATH="${RESOURCE_PATH}")

# Include sub-projects if needed
