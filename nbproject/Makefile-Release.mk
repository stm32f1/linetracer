#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/capture.o \
	${OBJECTDIR}/common/PID.o \
	${OBJECTDIR}/common/action.o \
	${OBJECTDIR}/common/comutil.o \
	${OBJECTDIR}/common/delay.o \
	${OBJECTDIR}/common/linetrace.o \
	${OBJECTDIR}/common/motor.o \
	${OBJECTDIR}/common/music/music.o \
	${OBJECTDIR}/common/music/sub.o \
	${OBJECTDIR}/common/position.o \
	${OBJECTDIR}/common/sensor.o \
	${OBJECTDIR}/common/servo.o \
	${OBJECTDIR}/common/stm32f10x_it.o \
	${OBJECTDIR}/common/syscalls.o \
	${OBJECTDIR}/common/system_stm32f10x.o \
	${OBJECTDIR}/common/tof/VL53L0X.o \
	${OBJECTDIR}/common/utility.o \
	${OBJECTDIR}/common/xbee.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/mode.o \
	${OBJECTDIR}/setup.o \
	${OBJECTDIR}/subroutine.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hakutaka

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hakutaka: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hakutaka ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/capture.o: capture.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/capture.o capture.c

${OBJECTDIR}/common/PID.o: common/PID.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/PID.o common/PID.c

${OBJECTDIR}/common/action.o: common/action.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/action.o common/action.c

${OBJECTDIR}/common/comutil.o: common/comutil.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/comutil.o common/comutil.c

${OBJECTDIR}/common/delay.o: common/delay.s
	${MKDIR} -p ${OBJECTDIR}/common
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/common/delay.o common/delay.s

${OBJECTDIR}/common/linetrace.o: common/linetrace.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/linetrace.o common/linetrace.c

${OBJECTDIR}/common/motor.o: common/motor.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/motor.o common/motor.c

${OBJECTDIR}/common/music/music.o: common/music/music.c
	${MKDIR} -p ${OBJECTDIR}/common/music
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/music/music.o common/music/music.c

${OBJECTDIR}/common/music/sub.o: common/music/sub.c
	${MKDIR} -p ${OBJECTDIR}/common/music
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/music/sub.o common/music/sub.c

${OBJECTDIR}/common/position.o: common/position.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/position.o common/position.c

${OBJECTDIR}/common/sensor.o: common/sensor.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/sensor.o common/sensor.c

${OBJECTDIR}/common/servo.o: common/servo.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/servo.o common/servo.c

${OBJECTDIR}/common/stm32f10x_it.o: common/stm32f10x_it.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/stm32f10x_it.o common/stm32f10x_it.c

${OBJECTDIR}/common/syscalls.o: common/syscalls.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/syscalls.o common/syscalls.c

${OBJECTDIR}/common/system_stm32f10x.o: common/system_stm32f10x.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/system_stm32f10x.o common/system_stm32f10x.c

${OBJECTDIR}/common/tof/VL53L0X.o: common/tof/VL53L0X.c
	${MKDIR} -p ${OBJECTDIR}/common/tof
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/tof/VL53L0X.o common/tof/VL53L0X.c

${OBJECTDIR}/common/utility.o: common/utility.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/utility.o common/utility.c

${OBJECTDIR}/common/xbee.o: common/xbee.c
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/xbee.o common/xbee.c

${OBJECTDIR}/main.o: main.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/mode.o: mode.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mode.o mode.c

${OBJECTDIR}/setup.o: setup.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/setup.o setup.c

${OBJECTDIR}/subroutine.o: subroutine.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/subroutine.o subroutine.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
