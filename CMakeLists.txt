cmake_minimum_required(VERSION 3.6)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIR})


add_executable(AtHomeObjectRecognitionScharr AtHomeObjectRecognitionScharr.cpp)
add_executable(CapturaVideo CapturaVideo.cpp)
#add_executable(SegmentacaoFirefly SegmentacaoFirefly.cpp)

target_link_libraries(AtHomeObjectRecognitionScharr ${OpenCV_LIBRARIES})
target_link_libraries(CapturaVideo ${OpenCV_LIBRARIES})
#target_link_libraries(SegmentacaoFirefly ${OpenCV_LIBRARIES})
