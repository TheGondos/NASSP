set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Orbitersdk/samples/ProjectApollo)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fpermissive")



####### Template #######
#add_library(ASTP SHARED
#    ${SRC_DIR}/
#)

#target_include_directories(ASTP PUBLIC
#	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
#)

#set_target_properties(ASTP
#	PROPERTIES
#	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_PLUGIN_DIR}
#	OUTPUT_NAME ASTP
#)

#install(TARGETS ASTP
#	DESTINATION ${ORBITER_INSTALL_PLUGIN_DIR}
#)


####### ApolloRTCCMFD #######

add_library(ApolloRTCCMFD SHARED
    ${SRC_DIR}/src_launch/rtcc.cpp
    ${SRC_DIR}/src_rtccmfd/ApollomfdButtons.cpp
    ${SRC_DIR}/src_rtccmfd/ApolloRTCCMFD.cpp
    ${SRC_DIR}/src_rtccmfd/ApolloRTCCMFD_Display.cpp
    ${SRC_DIR}/src_rtccmfd/ARCore.cpp
    ${SRC_DIR}/src_rtccmfd/ARoapiModule.cpp
    ${SRC_DIR}/src_rtccmfd/CoastNumericalIntegrator.cpp
    ${SRC_DIR}/src_rtccmfd/CSMLMGuidanceSim.cpp
    ${SRC_DIR}/src_rtccmfd/EnckeIntegrator.cpp
    ${SRC_DIR}/src_rtccmfd/EntryCalculations.cpp
    ${SRC_DIR}/src_rtccmfd/EphemProg.cpp
    ${SRC_DIR}/src_rtccmfd/GeneralizedIterator.cpp
    ${SRC_DIR}/src_rtccmfd/GeneralPurposeManeuver.cpp
    ${SRC_DIR}/src_rtccmfd/LDPP.cpp
    ${SRC_DIR}/src_rtccmfd/LMGuidanceSim.cpp
    ${SRC_DIR}/src_rtccmfd/LOITargeting.cpp
    ${SRC_DIR}/src_rtccmfd/LunarTargetingProgram.cpp
    ${SRC_DIR}/src_rtccmfd/LWP.cpp
    ${SRC_DIR}/src_rtccmfd/OrbMech.cpp
    ${SRC_DIR}/src_rtccmfd/ReentryNumericalIntegrator.cpp
    ${SRC_DIR}/src_rtccmfd/RTCCModule.cpp
    ${SRC_DIR}/src_rtccmfd/RTCC_EMSMISS.cpp
    ${SRC_DIR}/src_rtccmfd/rtcc_intermediate_library_programs.cpp
    ${SRC_DIR}/src_rtccmfd/rtcc_library_programs.cpp
    ${SRC_DIR}/src_rtccmfd/TLIGuidanceSim.cpp
    ${SRC_DIR}/src_rtccmfd/TLMCC.cpp
)

target_include_directories(ApolloRTCCMFD PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_lm
    ${SRC_DIR}/src_launch
)

set_target_properties(ApolloRTCCMFD
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_PLUGIN_DIR}
	OUTPUT_NAME ApolloRTCCMFD
)

install(TARGETS ApolloRTCCMFD
#	RUNTIME
	DESTINATION ${ORBITER_INSTALL_PLUGIN_DIR}
)

####### ASTP #######
add_library(ASTP SHARED
    ${SRC_DIR}/src_saturn/ASTP.cpp
    ${SRC_DIR}/src_sys/payload.cpp
    ${SRC_DIR}/src_sys/connector.cpp
)

target_include_directories(ASTP PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
)

set_target_properties(ASTP
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_PLUGIN_DIR}
	OUTPUT_NAME ASTP
)

install(TARGETS ASTP
#	RUNTIME
	DESTINATION ${ORBITER_INSTALL_PLUGIN_DIR}
)

####### CMChute #######
add_library(CMChute SHARED
${SRC_DIR}/src_landing/CMChute.cpp
${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(CMChute PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_sys
)

set_target_properties(CMChute
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_PLUGIN_DIR}
	OUTPUT_NAME CMChute
)

install(TARGETS CMChute
	DESTINATION ${ORBITER_INSTALL_PLUGIN_DIR}
)

####### Crawler #######
add_library(Crawler SHARED
    ${SRC_DIR}/src_launch/Crawler.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(Crawler PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
)

set_target_properties(Crawler
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_PLUGIN_DIR}
	OUTPUT_NAME Crawler
)

install(TARGETS Crawler
	DESTINATION ${ORBITER_INSTALL_PLUGIN_DIR}
)

####### EVA #######
add_library(EVA SHARED
${SRC_DIR}/src_csm/eva.cpp
${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(EVA PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_csm
)

set_target_properties(EVA
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_PLUGIN_DIR}
	OUTPUT_NAME EVA
)

install(TARGETS EVA
	DESTINATION ${ORBITER_INSTALL_PLUGIN_DIR}
)

####### FloatBag #######
add_library(FloatBag SHARED
    ${SRC_DIR}/src_landing/FloatBag.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(FloatBag PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_sys
)

set_target_properties(FloatBag
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_PLUGIN_DIR}
	OUTPUT_NAME FloatBag
)

install(TARGETS FloatBag
	DESTINATION ${ORBITER_INSTALL_PLUGIN_DIR}
)
