INCLUDE_DIRECTORIES(
    ${SMEDIA_INC_DIRS}
    )

SET(RISC2_TEST_DEVICE_SRC
    main.c
    )

ADD_EXECUTABLE(risc2TestDevice.plugin
    ${RISC2_TEST_DEVICE_SRC}
)

ADD_CUSTOM_COMMAND(
    TARGET risc2TestDevice.plugin
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
            #-O binary ${CMAKE_CURRENT_BINARY_DIR}/risc2TestDevice.plugin ${LIBRARY_OUTPUT_PATH}/risc2TestDevice.raw
            -O binary -j .risc2_header -j .text -j .data ${CMAKE_CURRENT_BINARY_DIR}/risc2TestDevice.plugin ${LIBRARY_OUTPUT_PATH}/risc2TestDevice.raw
    #COMMAND ${CMAKE_OBJCOPY}
            #-O binary ${CMAKE_CURRENT_BINARY_DIR}/risc2TestDevice.plugin ${LIBRARY_OUTPUT_PATH}/risc2TestDevice.raw
            #-O binary -j .reset ${CMAKE_CURRENT_BINARY_DIR}/risc2TestDevice.plugin ${LIBRARY_OUTPUT_PATH}/riscStartupCode.raw        
    #POST_BUILD
    COMMAND ${CMAKE_OBJDUMP}
            -D ${CMAKE_CURRENT_BINARY_DIR}/risc2TestDevice.plugin > ${LIBRARY_OUTPUT_PATH}/risc2TestDevice.dbg
    COMMAND ${READELF}
            -a ${CMAKE_CURRENT_BINARY_DIR}/risc2TestDevice.plugin > ${CMAKE_CURRENT_BINARY_DIR}/risc2TestDevice.symbol
    COMMAND ${DATACONV} -x ${LIBRARY_OUTPUT_PATH}/risc2TestDevice.raw
    #COMMAND ${DATACONV} -x ${LIBRARY_OUTPUT_PATH}/riscStartupCode.raw
    COMMAND mkrom
    ARGS -z -b 512K ${PROJECT_SOURCE_DIR}/sdk/target/ram/${CFG_RAM_INIT_SCRIPT} ${LIBRARY_OUTPUT_PATH}/risc2TestDevice.raw ${LIBRARY_OUTPUT_PATH}/risc2TestDevice.rom
)