add_subdirectory(unit_tests)

function(add_unit_test TARGET)
        add_executable(${TARGET}
        ${ARGN})
        target_link_libraries(${TARGET}
        contrib_catch_main)
        target_include_directories(${TARGET}
        PUBLIC ./)
endfunction()

add_unit_test(test_bitarray unit_tests/test_bitarray.cpp)
add_unit_test(test_bitread unit_tests/test_bitread.cpp bitread.cpp)
add_unit_test(test_bitwrite unit_tests/test_bitwrite.cpp bitwrite.cpp)
add_unit_test(test_encoder unit_tests/test_encoder.cpp encoder.cpp)
add_unit_test(test_decoder unit_tests/test_decoder.cpp encoder.cpp decoder.cpp)
add_unit_test(test_priority_queue unit_tests/test_priority_queue.cpp)

add_executable(
        archiver
        archiver.cpp bitread.cpp bitwrite.cpp cli_parser.cpp encoder.cpp decoder.cpp
)