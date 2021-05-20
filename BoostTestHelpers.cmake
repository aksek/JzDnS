function(add_boost_test SOURCE_FILE_NAME IMPLEMENTATION)

    get_filename_component(TEST_EXECUTABLE_NAME ${SOURCE_FILE_NAME} NAME_WE)

    add_executable(${TEST_EXECUTABLE_NAME} ${SOURCE_FILE_NAME} ${IMPLEMENTATION})
    target_link_libraries(${TEST_EXECUTABLE_NAME} 
                        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    link_directories(/home/aniela/software/cryptopp/)
    target_link_libraries(${TEST_EXECUTABLE_NAME} cryptopp)

    file(READ "${SOURCE_FILE_NAME}" SOURCE_FILE_CONTENTS)
    string(REGEX MATCHALL "BOOST_AUTO_TEST_CASE\\( *([A-Za-z_0-9]+) *\\)" 
    FOUND_TESTS ${SOURCE_FILE_CONTENTS})

    foreach(HIT ${FOUND_TESTS})
        string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1" TEST_NAME ${HIT})

        add_test(NAME "${TEST_EXECUTABLE_NAME}.${TEST_NAME}" 
                COMMAND ${TEST_EXECUTABLE_NAME}
                --run_test=${TEST_NAME} --catch_system_error=yes )
    endforeach()

endfunction()