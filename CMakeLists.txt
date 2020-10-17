cmake_minimum_required(VERSION 3.10)

project(serial_tool CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(THREADS_HAVE_PTHREAD_ARG 1)
find_package(Threads REQUIRED)
find_package(PkgConfig)
pkg_check_modules(SERIAL libserial)

set(BOOST_ROOT "/usr")
set(Boost_USE_STATIC_LIBS       OFF) # only find static libs
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost COMPONENTS program_options REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif()


add_executable(
    ${PROJECT_NAME}

    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/serial.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SERIAL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SERIAL_LDFLAGS} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION "bin")
