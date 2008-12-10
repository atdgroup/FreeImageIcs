cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

# The name of our project is "FreeImageAlgorithms".  CMakeLists files in this project can
# refer to the root source directory of the project as ${FreeImageIcs_SOURCE_DIR} and
# to the root binary directory of the project as ${FreeImageIcs_BINARY_DIR}.
PROJECT(FreeImageIcs)

SUBDIRS(src zlib Tests)

SET(CMAKE_DEBUG_POSTFIX "_d")

# Look for the FreeImage library
FIND_PACKAGE(FreeImage REQUIRED) 

FIND_PATH (GCI_DLL_PATH StringUtils.lib FORCE "C://dlls")
FIND_PATH (FREEIMAGEALGORITHMS_DIR_PATH FreeImageAlgorithms ../..)

# Make sure the compiler can find include files from our library.
INCLUDE_DIRECTORIES(include src 
							zlib
							Tests
							libics-1.5.1
							${FREEIMAGE_INCLUDE_PATH}
							${FreeImageAlgorithms_DIR_PATH}
							${FREEIMAGEALGORITHMS_DIR_PATH}/FreeImageAlgorithms/trunk/include
)

SET (LIBRARY_OUTPUT_PATH ${FreeImageIcs_BINARY_DIR}/bin)
SET (EXECUTABLE_OUTPUT_PATH ${FreeImageIcs_BINARY_DIR}/bin)

CONFIGURE_FILE(${FreeImageIcs_SOURCE_DIR}/Tests/Constants.h.template ${FreeImageIcs_SOURCE_DIR}/Tests/Constants.h)

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

IF(WIN32)
SET(CMAKE_CXX_STANDARD_LIBRARIES "user32.lib gdi32.lib")
ENDIF(WIN32)

MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH: " ${EXECUTABLE_OUTPUT_PATH} )

ADD_DEFINITIONS(-DFREEIMAGE_EXPORTS -DICS_ZLIB)

SET(SRCS zlib/adler32.c
	     zlib/compress.c
	     zlib/crc32.c
	     zlib/deflate.c
	     zlib/example.c
	     zlib/gzio.c
	     zlib/infback.c
	     zlib/inffast.c
	     zlib/inflate.c
	     zlib/inftrees.c
	     zlib/trees.c
	     zlib/uncompr.c
	     zlib/zutil.c
		 libics-1.5.1/libics_binary.c
		 libics-1.5.1/libics_data.c
		 libics-1.5.1/libics_gzip.c
		 libics-1.5.1/libics_history.c
		 libics-1.5.1/libics_preview.c
		 libics-1.5.1/libics_read.c
		 libics-1.5.1/libics_sensor.c
		 libics-1.5.1/libics_top.c
		 libics-1.5.1/libics_util.c
		 libics-1.5.1/libics_write.c
		 src/FreeImageIcs_IO.cpp
		 src/FreeImageIcs_MetaData.cpp
		 src/FreeImageIcs_Private.cpp
		 exports.def
		 exports_d.def
)

LINK_DIRECTORIES(${GCI_DLL_PATH})

ADD_LIBRARY (FreeImageIcs SHARED ${SRCS})

IF (UNIX)
  INSTALL(TARGETS freeimageics RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
ENDIF (UNIX)

# Link the executable to the FreeImage library.
TARGET_LINK_LIBRARIES (FreeImageIcs freeimagealgorithms.lib ${FREEIMAGE_LIBRARY})
