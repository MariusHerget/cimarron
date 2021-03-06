cmake_minimum_required(VERSION 2.8.9)

project (cimarron)
include_directories(".")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_CURRENT_LIST_DIR}/cmake
                      ${CMAKE_SOURCE_DIR}/cmake)


find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
set(LIBS
          ${OpenGL_LIBS}
	        ${OPENGL_gl_LIBRARY}
	        ${OPENGL_glu_LIBRARY}
          ${QT_LIBRARIES})
find_package(Boost 1.55.0 REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})



link_directories(${THIRDPARTY}/lib ${Boost_LIBRARY_DIRS})

add_definitions(-std=c++1y -g)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native -fopenmp -DNDEBUG -lboost_system")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1 -march=native")


add_executable(cimarron cimarron-main.cc)
# add_executable(opencvdemo cimarron-main-opencvdemo.cc)
target_link_libraries(cimarron ${OpenCV_LIBS} ${LIBS} ${Boost_LIBRARIES})
# target_link_libraries(opencvdemo ${OpenCV_LIBS} ${LIBS} ${Boost_LIBRARIES})
