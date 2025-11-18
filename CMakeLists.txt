# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(U233GammaNU232 LANGUAGES CXX)

# Find Geant4 with UI and visualization components
find_package(Geant4 REQUIRED ui_all vis_all)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Convenience include (matches Geant4 examples: sets compile defs/include dirs)
include(${Geant4_USE_FILE})

file(GLOB sources CONFIGURE_DEPENDS src/*.cc)
file(GLOB headers CONFIGURE_DEPENDS include/*.hh)

add_executable(u233_gamma_n_u232 ${sources} ${headers})

# Public include dir so MSVC finds headers
target_include_directories(u233_gamma_n_u232 PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Prefer MT if available
if(Geant4_multithreaded_FOUND)
  target_compile_definitions(u233_gamma_n_u232 PRIVATE G4MULTITHREADED)
endif()

# Warnings
if(MSVC)
  target_compile_options(u233_gamma_n_u232 PRIVATE /W3)
else()
  target_compile_options(u233_gamma_n_u232 PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# Link Geant4
target_link_libraries(u233_gamma_n_u232 PRIVATE ${Geant4_LIBRARIES})

# Copy macros next to the built executable for each config (Debug/Release)
add_custom_command(TARGET u233_gamma_n_u232 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:u233_gamma_n_u232>/macros"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/macros" "$<TARGET_FILE_DIR:u233_gamma_n_u232>/macros"
  VERBATIM)

# (Optional) Install targets and macros
install(TARGETS u233_gamma_n_u232 RUNTIME DESTINATION bin)
install(DIRECTORY macros DESTINATION share)