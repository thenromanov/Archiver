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

add_unit_test(test_output_file unit_tests/test_output_file.cpp)


add_executable(
        archiver
        archiver.cpp bitread.cpp bitwrite.cpp cli_parser.cpp encoder.cpp
)