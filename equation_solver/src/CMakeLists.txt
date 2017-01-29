find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(/usr/local/lib)
include_directories(${equation_solver_SOURCE_DIR}/include)

set(SOURCES CharacterDetector.cpp MnistReader.cpp MathEngine.cpp Ocr.cpp DataExtractor.cpp)

add_executable(demo demo.cpp ${SOURCES})
target_link_libraries(demo ${OpenCV_LIBS})
