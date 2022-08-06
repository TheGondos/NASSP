set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Orbitersdk/samples/ProjectApollo)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fpermissive -Wno-write-strings -Wno-format-overflow -Wno-format-zero-length")

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
    ${SRC_DIR}/src_csm/saturnmesh.cpp
    ${SRC_DIR}/src_csm/saturnvc.cpp
    ${SRC_DIR}/src_csm/saturnpanel.cpp
    ${SRC_DIR}/src_csm/ecs.cpp
    ${SRC_DIR}/src_sys/dsky.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Internals/Esystems.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Internals/Hsystems.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Internals/esysparse.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Internals/Thermal.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Internals/Hsysparse.cpp
    ${SRC_DIR}/src_csm/csmcautionwarning.cpp
    ${SRC_DIR}/src_sys/cautionwarning.cpp
    ${SRC_DIR}/src_launch/mccvc.cpp
    ${SRC_DIR}/src_sys/powersource.cpp
    ${SRC_DIR}/src_csm/csmrcs.cpp
    ${SRC_DIR}/src_csm/satswitches.cpp
    ${SRC_DIR}/src_sys/toggleswitch.cpp
    ${SRC_DIR}/src_sys/connector.cpp
    ${SRC_DIR}/src_csm/satsystems.cpp
    ${SRC_DIR}/src_csm/csmsensors.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_sys/pyro.cpp
)

target_include_directories(ApolloRTCCMFD PUBLIC
    ${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_lm
    ${SRC_DIR}/src_launch
)

target_link_libraries(ApolloRTCCMFD
	${ORBITER_SDK_LIB}
    XRSound_dll
)

set_target_properties(ApolloRTCCMFD
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_PLUGIN_DIR}
	OUTPUT_NAME ApolloRTCCMFD
)

add_dependencies(ApolloRTCCMFD
	Orbitersdk
    XRSound_dll
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

target_link_libraries(ASTP
	${ORBITER_SDK_LIB}
)

set_target_properties(ASTP
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME ASTP
)

install(TARGETS ASTP
#	RUNTIME
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
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

target_link_libraries(CMChute
	${ORBITER_SDK_LIB}
)

set_target_properties(CMChute
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME CMChute
)

install(TARGETS CMChute
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Crawler #######
add_library(Crawler SHARED
    ${SRC_DIR}/src_launch/Crawler.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(Crawler PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
)

target_link_libraries(Crawler
	${ORBITER_SDK_LIB}
)

set_target_properties(Crawler
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME Crawler
)

install(TARGETS Crawler
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
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

target_link_libraries(EVA
	${ORBITER_SDK_LIB}
)

set_target_properties(EVA
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME EVA
)

install(TARGETS EVA
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### FloatBag #######
add_library(FloatBag SHARED
    ${SRC_DIR}/src_landing/FloatBag.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(FloatBag PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_sys
)

target_link_libraries(FloatBag
	${ORBITER_SDK_LIB}
)

set_target_properties(FloatBag
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME FloatBag
)

install(TARGETS FloatBag
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Floodlight #######
add_library(Floodlight SHARED
    ${SRC_DIR}/src_launch/Floodlight.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(Floodlight PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_sys
)

target_link_libraries(Floodlight
	${ORBITER_SDK_LIB}
)

set_target_properties(Floodlight
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME Floodlight
)

install(TARGETS Floodlight
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### LC34 #######
add_library(LC34 SHARED
    ${SRC_DIR}/src_launch/IUUmbilical.cpp
    ${SRC_DIR}/src_launch/IUUmbilicalInterface.cpp
    ${SRC_DIR}/src_launch/IU_ESE.cpp
    ${SRC_DIR}/src_launch/LC34.cpp
    ${SRC_DIR}/src_launch/RCA110A.cpp
    ${SRC_DIR}/src_launch/SCMUmbilical.cpp
    ${SRC_DIR}/src_launch/SIB_ESE.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(LC34 PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_mfd
)

target_link_libraries(LC34
	${ORBITER_SDK_LIB}
)

set_target_properties(LC34
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME LC34
)

install(TARGETS LC34
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### LC37 #######
add_library(LC37 SHARED
    ${SRC_DIR}/src_launch/IUUmbilical.cpp
    ${SRC_DIR}/src_launch/IUUmbilicalInterface.cpp
    ${SRC_DIR}/src_launch/IU_ESE.cpp
    ${SRC_DIR}/src_launch/LC37.cpp
    ${SRC_DIR}/src_launch/RCA110A.cpp
    ${SRC_DIR}/src_launch/SCMUmbilical.cpp
    ${SRC_DIR}/src_launch/SIB_ESE.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
)

target_include_directories(LC37 PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_lm
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_csm
)

target_link_libraries(LC37
	${ORBITER_SDK_LIB}
)

set_target_properties(LC37
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME LC37
)

install(TARGETS LC37
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### LCC #######
add_library(LCC SHARED
    ${SRC_DIR}/src_launch/LCC.cpp
    ${SRC_DIR}/src_launch/LCCMFDButtons.cpp
    ${SRC_DIR}/src_launch/LCC_MFD.cpp
    ${SRC_DIR}/src_launch/RCA110A.cpp
)

target_include_directories(LCC PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_rtccmfd
    ${SRC_DIR}/src_sys
)

target_link_libraries(LCC
	${ORBITER_SDK_LIB}
)

set_target_properties(LCC
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME LCC
)

install(TARGETS LCC
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### LEM #######
add_library(LEM SHARED
    ${SRC_DIR}/src_aux/animations.cpp
    ${SRC_DIR}/src_aux/CDK.cpp
    ${SRC_DIR}/src_aux/Mission.cpp
    ${SRC_DIR}/src_aux/OrbiterMath.cpp
    ${SRC_DIR}/src_aux/vesim.cpp
    ${SRC_DIR}/src_lm/CWEA_FlipFlop.cpp
    ${SRC_DIR}/src_lm/lemconnector.cpp
    ${SRC_DIR}/src_lm/lm_aca.cpp
    ${SRC_DIR}/src_lm/lm_aeaa.cpp
    ${SRC_DIR}/src_lm/lm_aps.cpp
    ${SRC_DIR}/src_lm/lm_cwea.cpp
    ${SRC_DIR}/src_lm/lm_dps.cpp
    ${SRC_DIR}/src_lm/lm_ecs.cpp
    ${SRC_DIR}/src_lm/lm_eds.cpp
    ${SRC_DIR}/src_lm/lm_eps.cpp
    ${SRC_DIR}/src_lm/lm_programer.cpp
    ${SRC_DIR}/src_lm/lm_rcs.cpp
    ${SRC_DIR}/src_lm/lm_rr.cpp
    ${SRC_DIR}/src_lm/lm_scea.cpp
    ${SRC_DIR}/src_lm/lm_ttca.cpp
    ${SRC_DIR}/src_rtccmfd/OrbMech.cpp
    ${SRC_DIR}/src_sys/apolloguidance.cpp
    ${SRC_DIR}/src_aux/BasicExcelVC6.cpp
    ${SRC_DIR}/src_sys/cautionwarning.cpp
    ${SRC_DIR}/src_sys/cdu.cpp
    ${SRC_DIR}/src_sys/checklistController.cpp
    ${SRC_DIR}/src_sys/checklistControllerHelpers.cpp
    ${SRC_DIR}/src_sys/connector.cpp
    ${SRC_DIR}/src_sys/DelayTimer.cpp
    ${SRC_DIR}/src_sys/dsky.cpp
    ${SRC_DIR}/src_sys/dskyinterface.cpp
    ${SRC_DIR}/src_sys/FDAI.cpp
    ${SRC_DIR}/src_sys/imu.cpp
    ${SRC_DIR}/src_sys/imulog.cpp
    ${SRC_DIR}/src_sys/imumath.cpp
    ${SRC_DIR}/src_lm/LEM.cpp
    ${SRC_DIR}/src_lm/LEMcomputer.cpp
    ${SRC_DIR}/src_lm/lemmesh.cpp
    ${SRC_DIR}/src_lm/lempanel.cpp
    ${SRC_DIR}/src_lm/lemswitches.cpp
    ${SRC_DIR}/src_lm/lemsystems.cpp
    ${SRC_DIR}/src_lm/lemvc.cpp
    ${SRC_DIR}/src_lm/lm_ags.cpp
    ${SRC_DIR}/src_lm/lm_telecom.cpp
    ${SRC_DIR}/src_lm/lmscs.cpp
    ${SRC_DIR}/src_mfd/MFDconnector.cpp
    ${SRC_DIR}/src_sys/MechanicalAccelerometer.cpp
    ${SRC_DIR}/src_sys/missiontimer.cpp
    ${SRC_DIR}/src_sys/ORDEAL.cpp
    ${SRC_DIR}/src_sys/payload.cpp
    ${SRC_DIR}/src_sys/powersource.cpp
    ${SRC_DIR}/src_sys/pyro.cpp
    ${SRC_DIR}/src_sys/soundevents.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_sys/thread.cpp
    ${SRC_DIR}/src_sys/toggleswitch.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
    ${SRC_DIR}/src_sys/yaAGC/agc_engine.c
    ${SRC_DIR}/src_sys/yaAGC/agc_engine_init.c
    ${SRC_DIR}/src_sys/yaAGC/agc_utilities.c
    ${SRC_DIR}/src_sys/yaAGC/Backtrace.c
    ${SRC_DIR}/src_sys/yaAGC/random.c
    ${SRC_DIR}/src_sys/yaAGC/rfopen.c
    ${SRC_DIR}/src_lm/yaAGS/aea_engine.c
    ${SRC_DIR}/src_lm/yaAGS/aea_engine_init.c
    ${SRC_DIR}/src_lm/yaAGS/OutputAPI_AGS.c
)


target_include_directories(LEM PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_lm
    ${SRC_DIR}/src_launch
    ${SRC_DIR}/src_moon
)

target_link_libraries(LEM
	${ORBITER_SDK_LIB}
    PanelSDK
)

set_target_properties(LEM
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME LEM
)

install(TARGETS LEM
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### LES #######
add_library(LES SHARED
    ${SRC_DIR}/src_saturn/LES.cpp
)

target_include_directories(LES PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
)

target_link_libraries(LES
	${ORBITER_SDK_LIB}
)

set_target_properties(LES
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME LES
)

install(TARGETS LES
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### LEVA #######
add_library(LEVA SHARED
    ${SRC_DIR}/src_aux/CDK.cpp
    ${SRC_DIR}/src_moon/leva.cpp
    ${SRC_DIR}/src_aux/OrbiterMath.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(LEVA PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_lm
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_mfd
)

target_link_libraries(LEVA
	${ORBITER_SDK_LIB}
)

set_target_properties(LEVA
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME LEVA
)

install(TARGETS LEVA
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)


####### LRV #######
add_library(LRV SHARED
    ${SRC_DIR}/src_aux/CDK.cpp
    ${SRC_DIR}/src_moon/LRV.cpp
    ${SRC_DIR}/src_aux/OrbiterMath.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(LRV PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_lm
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_moon
)

target_link_libraries(LRV
	${ORBITER_SDK_LIB}
)

set_target_properties(LRV
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME LRV
)

add_dependencies(LRV
	Orbitersdk
    XRSound_dll
)

install(TARGETS LRV
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### MCC #######
add_library(MCC SHARED
    ${SRC_DIR}/src_launch/mccvessel.cpp
    ${SRC_DIR}/src_launch/MCC_Mission_B.cpp
    ${SRC_DIR}/src_launch/MCC_Mission_C.cpp
    ${SRC_DIR}/src_launch/MCC_Mission_C_PRIME.cpp
    ${SRC_DIR}/src_launch/MCC_Mission_D.cpp
    ${SRC_DIR}/src_launch/MCC_Mission_F.cpp
    ${SRC_DIR}/src_launch/MCC_Mission_G.cpp
    ${SRC_DIR}/src_launch/rtcc.cpp
    ${SRC_DIR}/src_launch/RTCC_Mission_B.cpp
    ${SRC_DIR}/src_launch/RTCC_Mission_C.cpp
    ${SRC_DIR}/src_launch/RTCC_Mission_Calculations.cpp
    ${SRC_DIR}/src_launch/RTCC_Mission_C_PRIME.cpp
    ${SRC_DIR}/src_launch/RTCC_Mission_D.cpp
    ${SRC_DIR}/src_launch/RTCC_Mission_F.cpp
    ${SRC_DIR}/src_launch/RTCC_Mission_G.cpp
    ${SRC_DIR}/src_rtccmfd/CoastNumericalIntegrator.cpp
    ${SRC_DIR}/src_rtccmfd/CSMLMGuidanceSim.cpp
    ${SRC_DIR}/src_rtccmfd/EnckeIntegrator.cpp
    ${SRC_DIR}/src_rtccmfd/EntryCalculations.cpp
    ${SRC_DIR}/src_rtccmfd/GeneralizedIterator.cpp
    ${SRC_DIR}/src_rtccmfd/GeneralPurposeManeuver.cpp
    ${SRC_DIR}/src_rtccmfd/LDPP.cpp
    ${SRC_DIR}/src_rtccmfd/LMGuidanceSim.cpp
    ${SRC_DIR}/src_rtccmfd/LOITargeting.cpp
    ${SRC_DIR}/src_rtccmfd/LunarTargetingProgram.cpp
    ${SRC_DIR}/src_rtccmfd/LWP.cpp
    ${SRC_DIR}/src_rtccmfd/OrbMech.cpp
    ${SRC_DIR}/src_launch/mcc.cpp
    ${SRC_DIR}/src_rtccmfd/ReentryNumericalIntegrator.cpp
    ${SRC_DIR}/src_rtccmfd/RTCCModule.cpp
    ${SRC_DIR}/src_rtccmfd/RTCC_EMSMISS.cpp
    ${SRC_DIR}/src_rtccmfd/rtcc_intermediate_library_programs.cpp
    ${SRC_DIR}/src_rtccmfd/rtcc_library_programs.cpp
    ${SRC_DIR}/src_rtccmfd/TLIGuidanceSim.cpp
    ${SRC_DIR}/src_rtccmfd/TLMCC.cpp
    ${SRC_DIR}/src_sys/thread.cpp
    ${SRC_DIR}/src_launch/mccvc.cpp
)

target_include_directories(MCC PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_lm
    ${SRC_DIR}/src_launch
)

target_link_libraries(MCC
	${ORBITER_SDK_LIB}
    PanelSDK
    lua::lib
    XRSound_dll
)

set_target_properties(MCC
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME MCC
)

install(TARGETS MCC
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### ML #######
add_library(ML SHARED
    ${SRC_DIR}/src_launch/IUUmbilical.cpp
    ${SRC_DIR}/src_launch/IUUmbilicalInterface.cpp
    ${SRC_DIR}/src_launch/IU_ESE.cpp
    ${SRC_DIR}/src_launch/ML.cpp
    ${SRC_DIR}/src_launch/RCA110A.cpp
    ${SRC_DIR}/src_launch/SIC_ESE.cpp
    ${SRC_DIR}/src_launch/TSMUmbilical.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(ML PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_mfd
)

target_link_libraries(ML
	${ORBITER_SDK_LIB}
)

set_target_properties(ML
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME ML
)

install(TARGETS ML
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### MSS #######
add_library(MSS SHARED
    ${SRC_DIR}/src_launch/MSS.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
)

target_include_directories(MSS PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
)

target_link_libraries(MSS
	${ORBITER_SDK_LIB}
)

set_target_properties(MSS
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME MSS
)

install(TARGETS MSS
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### PanelSDK #######
add_library(PanelSDK SHARED
    ${SRC_DIR}/src_sys/PanelSDK/BUILD.CPP
    ${SRC_DIR}/src_sys/PanelSDK/intruments.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Panel.cpp
    ${SRC_DIR}/src_sys/PanelSDK/PanelSDK.cpp
    ${SRC_DIR}/src_sys/PanelSDK/VSMGMT.CPP
    ${SRC_DIR}/src_sys/PanelSDK/Matrix.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Vectors.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Internals/esysparse.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Internals/Esystems.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Internals/Hsysparse.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Internals/Hsystems.cpp
    ${SRC_DIR}/src_sys/PanelSDK/Internals/Thermal.cpp
)

target_include_directories(PanelSDK PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
)

target_link_libraries(PanelSDK
	${ORBITER_SDK_LIB}
)

set_target_properties(PanelSDK
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME PanelSDK
)

install(TARGETS PanelSDK
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Template #######
add_library(s1b SHARED
    ${SRC_DIR}/src_saturn/s1b.cpp
    ${SRC_DIR}/src_saturn/s1bsystems.cpp
    ${SRC_DIR}/src_sys/connector.cpp
    ${SRC_DIR}/src_sys/pyro.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
)

target_include_directories(s1b PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_launch
)

target_link_libraries(s1b
	${ORBITER_SDK_LIB}
    PanelSDK
)

set_target_properties(s1b
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME s1b
)

install(TARGETS s1b
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### s1c #######
add_library(s1c SHARED
    ${SRC_DIR}/src_saturn/s1c.cpp
)

target_include_directories(s1c PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
)

target_link_libraries(s1c
	${ORBITER_SDK_LIB}
)

set_target_properties(s1c
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME s1c
)

install(TARGETS s1c
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Sat1bAbort1 #######
add_library(Sat1bAbort1 SHARED
    ${SRC_DIR}/src_saturn/Saturn1Abort.cpp
)

target_include_directories(Sat1bAbort1 PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
)

target_link_libraries(Sat1bAbort1
	${ORBITER_SDK_LIB}
)

set_target_properties(Sat1bAbort1
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME Sat1bAbort1
)

install(TARGETS Sat1bAbort1
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Sat1bAbort2 #######
add_library(Sat1bAbort2 SHARED
    ${SRC_DIR}/src_saturn/Saturn1Abort2.cpp
)

target_include_directories(Sat1bAbort2 PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
)

target_link_libraries(Sat1bAbort2
	${ORBITER_SDK_LIB}
)

set_target_properties(Sat1bAbort2
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME Sat1bAbort2
)

install(TARGETS Sat1bAbort2
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Template #######
add_library(Sat5Abort1 SHARED
    ${SRC_DIR}/src_saturn/Sat5Abort1.cpp
)

target_include_directories(Sat5Abort1 PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
)

target_link_libraries(Sat5Abort1
	${ORBITER_SDK_LIB}
)

set_target_properties(Sat5Abort1
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME Sat5Abort1
)

install(TARGETS Sat5Abort1
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Sat5Abort2 #######
add_library(Sat5Abort2 SHARED
    ${SRC_DIR}/src_saturn/Sat5Abort2.cpp
)

target_include_directories(Sat5Abort2 PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
)

target_link_libraries(Sat5Abort2
	${ORBITER_SDK_LIB}
)

set_target_properties(Sat5Abort2
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME Sat5Abort2
)

install(TARGETS Sat5Abort2
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Sat5Abort3 #######
add_library(Sat5Abort3 SHARED
    ${SRC_DIR}/src_csm/smjc.cpp
    ${SRC_DIR}/src_saturn/Sat5Abort3.cpp
    ${SRC_DIR}/src_sys/DelayTimer.cpp
)

target_include_directories(Sat5Abort3 PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_csm
)

target_link_libraries(Sat5Abort3
	${ORBITER_SDK_LIB}
)

set_target_properties(Sat5Abort3
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME Sat5Abort3
)

install(TARGETS Sat5Abort3
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Sat5LMDSC #######
add_library(Sat5LMDSC SHARED
    ${SRC_DIR}/src_lm/Sat5LMDSC.cpp
)

target_include_directories(Sat5LMDSC PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(Sat5LMDSC
	${ORBITER_SDK_LIB}
)

set_target_properties(Sat5LMDSC
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME Sat5LMDSC
)

install(TARGETS Sat5LMDSC
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Saturn1b #######
add_library(Saturn1b SHARED
    ${SRC_DIR}/src_aux/animations.cpp
    ${SRC_DIR}/src_aux/Mission.cpp
    ${SRC_DIR}/src_aux/vesim.cpp
    ${SRC_DIR}/src_csm/csmsensors.cpp
    ${SRC_DIR}/src_csm/rhc.cpp
    ${SRC_DIR}/src_csm/sce.cpp
    ${SRC_DIR}/src_csm/smjc.cpp
    ${SRC_DIR}/src_rtccmfd/OrbMech.cpp
    ${SRC_DIR}/src_saturn/canard.cpp
    ${SRC_DIR}/src_saturn/dcs.cpp
    ${SRC_DIR}/src_saturn/eds.cpp
    ${SRC_DIR}/src_saturn/FCC.cpp
    ${SRC_DIR}/src_saturn/IUAuxiliaryPowerDistributor.cpp
    ${SRC_DIR}/src_saturn/IUControlDistributor.cpp
    ${SRC_DIR}/src_saturn/IUControlSignalProcessor.cpp
    ${SRC_DIR}/src_saturn/j2engine.cpp
    ${SRC_DIR}/src_saturn/LVDA.cpp
    ${SRC_DIR}/src_saturn/LVDC.cpp
    ${SRC_DIR}/src_saturn/qball.cpp
    ${SRC_DIR}/src_saturn/s1bsystems.cpp
    ${SRC_DIR}/src_saturn/siisystems.cpp
    ${SRC_DIR}/src_saturn/sivbsystems.cpp
    ${SRC_DIR}/src_sys/apolloguidance.cpp
    ${SRC_DIR}/src_aux/BasicExcelVC6.cpp
    ${SRC_DIR}/src_sys/cautionwarning.cpp
    ${SRC_DIR}/src_sys/cdu.cpp
    ${SRC_DIR}/src_sys/checklistController.cpp
    ${SRC_DIR}/src_sys/checklistControllerHelpers.cpp
    ${SRC_DIR}/src_sys/connector.cpp
    ${SRC_DIR}/src_csm/csm_telecom.cpp
    ${SRC_DIR}/src_csm/csmcautionwarning.cpp
    ${SRC_DIR}/src_csm/CSMcomputer.cpp
    ${SRC_DIR}/src_csm/csmconnector.cpp
    ${SRC_DIR}/src_csm/csmrcs.cpp
    ${SRC_DIR}/src_csm/dockingprobe.cpp
    ${SRC_DIR}/src_sys/DelayTimer.cpp
    ${SRC_DIR}/src_sys/dsky.cpp
    ${SRC_DIR}/src_sys/dskyinterface.cpp
    ${SRC_DIR}/src_csm/ecs.cpp
    ${SRC_DIR}/src_sys/FDAI.cpp
    ${SRC_DIR}/src_sys/imu.cpp
    ${SRC_DIR}/src_sys/imulog.cpp
    ${SRC_DIR}/src_sys/imumath.cpp
    ${SRC_DIR}/src_saturn/iu.cpp
    ${SRC_DIR}/src_saturn/lvimu.cpp
    ${SRC_DIR}/src_mfd/MFDconnector.cpp
    ${SRC_DIR}/src_sys/MechanicalAccelerometer.cpp
    ${SRC_DIR}/src_sys/missiontimer.cpp
    ${SRC_DIR}/src_sys/ORDEAL.cpp
    ${SRC_DIR}/src_sys/powersource.cpp
    ${SRC_DIR}/src_sys/pyro.cpp
    ${SRC_DIR}/src_saturn/sat1bmesh.cpp
    ${SRC_DIR}/src_csm/satswitches.cpp
    ${SRC_DIR}/src_csm/satsystems.cpp
    ${SRC_DIR}/src_csm/saturn.cpp
    ${SRC_DIR}/src_saturn/Saturn1b.cpp
    ${SRC_DIR}/src_csm/saturnmesh.cpp
    ${SRC_DIR}/src_csm/saturnpanel.cpp
    ${SRC_DIR}/src_csm/saturnvc.cpp
    ${SRC_DIR}/src_csm/scs.cpp
    ${SRC_DIR}/src_csm/secs.cpp
    ${SRC_DIR}/src_sys/soundevents.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_csm/sps.cpp
    ${SRC_DIR}/src_sys/thread.cpp
    ${SRC_DIR}/src_sys/toggleswitch.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
    ${SRC_DIR}/src_sys/yaAGC/agc_engine.c
    ${SRC_DIR}/src_sys/yaAGC/agc_engine_init.c
    ${SRC_DIR}/src_sys/yaAGC/Backtrace.c
    ${SRC_DIR}/src_sys/yaAGC/random.c
    ${SRC_DIR}/src_sys/yaAGC/rfopen.c
    ${SRC_DIR}/src_aux/IMFD/IMFD_Client.cpp
)

target_include_directories(Saturn1b PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_launch
    ${SRC_DIR}/src_lm
    ${SRC_DIR}/src_landing
)

target_link_libraries(Saturn1b
	${ORBITER_SDK_LIB}
	lua::lib
    PanelSDK
)

set_target_properties(Saturn1b
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME Saturn1b
)

install(TARGETS Saturn1b
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### Saturn5 #######
add_library(Saturn5 SHARED
    ${SRC_DIR}/src_aux/animations.cpp
    ${SRC_DIR}/src_aux/Mission.cpp
    ${SRC_DIR}/src_aux/vesim.cpp
    ${SRC_DIR}/src_csm/csmsensors.cpp
    ${SRC_DIR}/src_csm/rhc.cpp
    ${SRC_DIR}/src_csm/sce.cpp
    ${SRC_DIR}/src_csm/smjc.cpp
    ${SRC_DIR}/src_rtccmfd/OrbMech.cpp
    ${SRC_DIR}/src_saturn/canard.cpp
    ${SRC_DIR}/src_saturn/dcs.cpp
    ${SRC_DIR}/src_saturn/eds.cpp
    ${SRC_DIR}/src_saturn/FCC.cpp
    ${SRC_DIR}/src_saturn/IUAuxiliaryPowerDistributor.cpp
    ${SRC_DIR}/src_saturn/IUControlDistributor.cpp
    ${SRC_DIR}/src_saturn/IUControlSignalProcessor.cpp
    ${SRC_DIR}/src_saturn/j2engine.cpp
    ${SRC_DIR}/src_saturn/LVDA.cpp
    ${SRC_DIR}/src_saturn/LVDC.cpp
    ${SRC_DIR}/src_saturn/qball.cpp
    ${SRC_DIR}/src_saturn/s1csystems.cpp
    ${SRC_DIR}/src_saturn/siisystems.cpp
    ${SRC_DIR}/src_saturn/sivbsystems.cpp
    ${SRC_DIR}/src_sys/apolloguidance.cpp
    ${SRC_DIR}/src_aux/BasicExcelVC6.cpp
    ${SRC_DIR}/src_sys/cautionwarning.cpp
    ${SRC_DIR}/src_sys/cdu.cpp
    ${SRC_DIR}/src_sys/checklistController.cpp
    ${SRC_DIR}/src_sys/checklistControllerHelpers.cpp
    ${SRC_DIR}/src_sys/connector.cpp
    ${SRC_DIR}/src_csm/csm_telecom.cpp
    ${SRC_DIR}/src_csm/csmcautionwarning.cpp
    ${SRC_DIR}/src_csm/CSMcomputer.cpp
    ${SRC_DIR}/src_csm/csmconnector.cpp
    ${SRC_DIR}/src_csm/csmrcs.cpp
    ${SRC_DIR}/src_csm/dockingprobe.cpp
    ${SRC_DIR}/src_sys/DelayTimer.cpp
    ${SRC_DIR}/src_sys/dsky.cpp
    ${SRC_DIR}/src_sys/dskyinterface.cpp
    ${SRC_DIR}/src_csm/ecs.cpp
    ${SRC_DIR}/src_sys/FDAI.cpp
    ${SRC_DIR}/src_sys/imu.cpp
    ${SRC_DIR}/src_sys/imulog.cpp
    ${SRC_DIR}/src_sys/imumath.cpp
    ${SRC_DIR}/src_saturn/iu.cpp
    ${SRC_DIR}/src_saturn/lvimu.cpp
    ${SRC_DIR}/src_mfd/MFDconnector.cpp
    ${SRC_DIR}/src_sys/MechanicalAccelerometer.cpp
    ${SRC_DIR}/src_sys/missiontimer.cpp
    ${SRC_DIR}/src_sys/ORDEAL.cpp
    ${SRC_DIR}/src_sys/powersource.cpp
    ${SRC_DIR}/src_sys/pyro.cpp
    ${SRC_DIR}/src_saturn/sat5mesh.cpp
    ${SRC_DIR}/src_csm/satswitches.cpp
    ${SRC_DIR}/src_csm/satsystems.cpp
    ${SRC_DIR}/src_csm/saturn.cpp
    ${SRC_DIR}/src_saturn/Saturn5.cpp
    ${SRC_DIR}/src_csm/saturnmesh.cpp
    ${SRC_DIR}/src_csm/saturnpanel.cpp
    ${SRC_DIR}/src_csm/saturnvc.cpp
    ${SRC_DIR}/src_csm/scs.cpp
    ${SRC_DIR}/src_csm/secs.cpp
    ${SRC_DIR}/src_sys/soundevents.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_csm/sps.cpp
    ${SRC_DIR}/src_sys/thread.cpp
    ${SRC_DIR}/src_sys/toggleswitch.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
    ${SRC_DIR}/src_sys/yaAGC/agc_engine.c
    ${SRC_DIR}/src_sys/yaAGC/agc_engine_init.c
    ${SRC_DIR}/src_sys/yaAGC/Backtrace.c
    ${SRC_DIR}/src_sys/yaAGC/random.c
    ${SRC_DIR}/src_sys/yaAGC/rfopen.c
    ${SRC_DIR}/src_aux/IMFD/IMFD_Client.cpp
)

target_include_directories(Saturn5 PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_launch
    ${SRC_DIR}/src_lm
    ${SRC_DIR}/src_landing
)

target_link_libraries(Saturn5
	${ORBITER_SDK_LIB}
	lua::lib
    PanelSDK
)

set_target_properties(Saturn5
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME Saturn5
)

install(TARGETS Saturn5
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### sii #######
add_library(sii SHARED
    ${SRC_DIR}/src_saturn/sii.cpp
)

target_include_directories(sii PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/src_sys
)

target_link_libraries(sii
	${ORBITER_SDK_LIB}
)

set_target_properties(sii
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME sii
)

install(TARGETS sii
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### SIVb #######
add_library(SIVb SHARED
    ${SRC_DIR}/src_rtccmfd/OrbMech.cpp
    ${SRC_DIR}/src_saturn/dcs.cpp
    ${SRC_DIR}/src_saturn/eds.cpp
    ${SRC_DIR}/src_saturn/FCC.cpp
    ${SRC_DIR}/src_saturn/IUAuxiliaryPowerDistributor.cpp
    ${SRC_DIR}/src_saturn/IUControlDistributor.cpp
    ${SRC_DIR}/src_saturn/IUControlSignalProcessor.cpp
    ${SRC_DIR}/src_saturn/LVDA.cpp
    ${SRC_DIR}/src_saturn/LVDC.cpp
    ${SRC_DIR}/src_saturn/lvimu.cpp
    ${SRC_DIR}/src_saturn/sivbsystems.cpp
    ${SRC_DIR}/src_sys/connector.cpp
    ${SRC_DIR}/src_saturn/iu.cpp
    ${SRC_DIR}/src_sys/DelayTimer.cpp
    ${SRC_DIR}/src_sys/powersource.cpp
    ${SRC_DIR}/src_saturn/sivb.cpp
    ${SRC_DIR}/src_sys/pyro.cpp
)

target_include_directories(SIVb PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_launch
    ${SRC_DIR}/src_lm
)

target_link_libraries(SIVb
	${ORBITER_SDK_LIB}
    lua::lib
    PanelSDK
)

set_target_properties(SIVb
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME SIVb
)

install(TARGETS SIVb
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### SM #######
add_library(SM SHARED
    ${SRC_DIR}/src_csm/sm.cpp
    ${SRC_DIR}/src_csm/smjc.cpp
    ${SRC_DIR}/src_sys/DelayTimer.cpp
    ${SRC_DIR}/src_sys/soundlib.cpp
)

target_include_directories(SM PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
)

target_link_libraries(SM
	${ORBITER_SDK_LIB}
)

set_target_properties(SM
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME SM
)

install(TARGETS SM
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### VAB #######
add_library(VAB SHARED
    ${SRC_DIR}/src_sys/soundlib.cpp
    ${SRC_DIR}/src_aux/tracer.cpp
    ${SRC_DIR}/src_launch/VAB.cpp
    ${SRC_DIR}/src_launch/VABAnimations.cpp
)

target_include_directories(VAB PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_saturn
)

target_link_libraries(VAB
	${ORBITER_SDK_LIB}
)

set_target_properties(VAB
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_MODULE_DIR}/ProjectApollo
	OUTPUT_NAME VAB
)

install(TARGETS VAB
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}/ProjectApollo
)

####### ProjectApolloMFD #######
add_library(ProjectApolloMFD SHARED
    ${SRC_DIR}/src_mfd/ProjectApolloMFDButtons.cpp
    ${SRC_DIR}/src_rtccmfd/OrbMech.cpp
    ${SRC_DIR}/src_sys/connector.cpp
    ${SRC_DIR}/src_mfd/MFDconnector.cpp
    ${SRC_DIR}/src_mfd/ProjectApolloChecklistMFD.cpp
    ${SRC_DIR}/src_mfd/ProjectApolloMFD.cpp
    ${SRC_DIR}/src_mfd/ProjectApolloPlugin.cpp
    ${SRC_DIR}/src_csm/csmconnector.cpp
    ${SRC_DIR}/src_csm/saturn.cpp
    ${SRC_DIR}/src_csm/saturnpanel.cpp
)

target_include_directories(ProjectApolloMFD PUBLIC
	${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${ORBITER_BINARY_SDK_DIR}/XRSound
    ${SRC_DIR}/src_rtccmfd
    ${SRC_DIR}/src_sys
    ${SRC_DIR}/src_aux
    ${SRC_DIR}/src_mfd
    ${SRC_DIR}/src_csm
    ${SRC_DIR}/src_lm
    ${SRC_DIR}/src_saturn
    ${SRC_DIR}/src_launch
)

target_link_libraries(ProjectApolloMFD
	${ORBITER_SDK_LIB}
    lua::lib
)

set_target_properties(ProjectApolloMFD
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_PLUGIN_DIR}
	OUTPUT_NAME ProjectApolloMFD
)

install(TARGETS ProjectApolloMFD
	DESTINATION ${ORBITER_INSTALL_PLUGIN_DIR}
)

####### Resources #######

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Config
	DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Config/ProjectApollo/Saturn1b.cfg
    DESTINATION ${ORBITER_INSTALL_ROOT_DIR}/Config/Vessels/ProjectApollo/
    RENAME Saturn1B.cfg
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Config/ProjectApollo/SIVBSystems.cfg
    DESTINATION ${ORBITER_INSTALL_ROOT_DIR}/Config/Vessels/ProjectApollo/
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Config/ProjectApollo/nsat1stg2.cfg
    DESTINATION ${ORBITER_INSTALL_ROOT_DIR}/Config/Vessels/ProjectApollo/
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Meshes
	DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Missions
	DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Scenarios
	DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Doc
	DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Script
	DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Sound
	DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Textures
	DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)

file(GLOB NASSP_BITMAPS ${SRC_DIR}/Bitmaps/*.bmp)

install(FILES ${NASSP_BITMAPS}
	DESTINATION ${ORBITER_INSTALL_ROOT_DIR}/Bitmaps/ProjectApollo
)
