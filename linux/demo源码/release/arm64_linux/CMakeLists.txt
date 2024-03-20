set(LINUXVERSION "arm64_linux")
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)

SET(CMAKE_SYSTEM_NAME ${LINUXVERSION})
set(OUTFILENAME "readCardDemo_${LINUXVERSION}")
set(INCLUDEPATH ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})

MESSAGE( STATUS "this var = ${LINUXVERSION}.")
MESSAGE( STATUS "The ${LINUXVERSION} in.")
set(LIBDLLPATH ${CMAKE_SOURCE_DIR}/lib/${LINUXVERSION}/)
set(LIDSOPATH ${CMAKE_SOURCE_DIR}/release/${LINUXVERSION}/)
set(LIBSUFFIX ".so")

MESSAGE( STATUS "set end.")

set(ENV{LD_LIBRARY_PATH} "${LIBDLLPATH}:${LIDSOPATH}:./")
MESSAGE(STAUS "LD_LIBRAAY_PATH:$ENV{LD_LIBRARY_PATH}")

aux_source_directory(./ DIR_SRCS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb -ffunction-sections -O0 -fvisibility=hidden -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -ggdb -ffunction-sections -O0 -fvisibility=hidden -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -ggdb -ffunction-sections -O0 -fvisibility=hidden -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -ggdb -ffunction-sections -O0 -fvisibility=hidden -fPIC")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c++11 -ggdb -ffunction-sections -O0 -fvisibility=hidden -fPIC")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=c++11 -ggdb -ffunction-sections -O0 -fvisibility=hidden -fPIC")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_SOURCE_DIR}/-Wl,-gc-sections,--no-as-needed ${CMAKE_SOURCE_DIR}/-ldl ${CMAKE_SOURCE_DIR}/-fPIC)
include_directories(${CMAKE_SOURCE_DIR}/)
include_directories(${INCLUDEPATH})
link_directories(${LIBDLLPATH})
add_executable( ${OUTFILENAME} ${CMAKE_SOURCE_DIR}/readCardDemo.cpp)

link_directories(${LIBDLLPATH})

add_library( zyreadcard SHARED  IMPORTED )
set_target_properties( zyreadcard PROPERTIES IMPORTED_LOCATION  ${LIBDLLPATH}libreadCardInfo_${LINUXVERSION}.so )
target_link_libraries(  ${OUTFILENAME}
			-Wl,--start-group
			zyreadcard
			dl
			 -Wl,--end-group
			)

SET(CMAKE_INSTALL_PREFIX ${LIDSOPATH})
INSTALL(TARGETS ${OUTFILENAME} RUNTIME DESTINATION ${LIDSOPATH})
INSTALL(FILES ${LIDSOPATH}${OUTFILENAME} DESTINATION ${LIBDLLPATH} PERMISSIONS OWNER_EXECUTE RENAME readCardDemo)