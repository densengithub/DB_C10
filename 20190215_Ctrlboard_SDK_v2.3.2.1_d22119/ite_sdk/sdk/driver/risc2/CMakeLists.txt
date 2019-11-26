if (DEFINED CFG_RISC2_ENABLE)
    if (DEFINED CFG_RISC2_TEST_DEVICE)
        add_subdirectory(risc2TestDevice)
    endif()

    if (DEFINED CFG_RISC2_OPUS_CODEC)
        add_subdirectory(opuscodec)
    endif()
endif()
