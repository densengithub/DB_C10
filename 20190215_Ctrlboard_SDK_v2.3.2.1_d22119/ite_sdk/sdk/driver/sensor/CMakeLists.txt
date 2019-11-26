include_directories(
	.
	..
    ${SMEDIA_INC_DIRS}
    ${PROJECT_SOURCE_DIR}/sdk/include
    )

add_library(sensor STATIC
	  mmp_sensor.c 
    )
if (SENSOR_FW_NAME STREQUAL OMNIVISION_OV7725)  
    add_library(sensor STATIC
		omnivision/omnivision_interface.c
    	omnivision/omnivision_driver.c
	  	omnivision/omnivision_io.c	 
    )
elseif (SENSOR_FW_NAME STREQUAL PIXELPLUS_PO3100)
    add_library(sensor STATIC
		pixelplus/pixelplus_interface.c
    	pixelplus/pixelplus_driver.c
	  	pixelplus/pixelplus_io.c
    )
elseif (SENSOR_FW_NAME STREQUAL NOVATEK_NT99141)
    add_library(sensor STATIC
		novatek_nt99141/novatek_nt99141_interface.c
    	novatek_nt99141/novatek_nt99141_driver.c
	  	novatek_nt99141/novatek_nt99141_io.c
    )    
elseif (SENSOR_FW_NAME STREQUAL AR0130)
    add_library(sensor STATIC
		ar0130/ar0130_interface.c
    	ar0130/ar0130_driver.c
    )
endif()  