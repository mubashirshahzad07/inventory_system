# Generated Makefile - do not edit!
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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}
OBJECTFILES= \
	${OBJECTDIR}/fileHandler.o \
	${OBJECTDIR}/inventoryReports.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/productManagement.o \
	${OBJECTDIR}/productNode.o \
	${OBJECTDIR}/transactionManager.o \
	${OBJECTDIR}/transactionNode.o \
	${OBJECTDIR}/transactionQueue.o

CCFLAGS=-std=c++11
CXXFLAGS=-std=c++11
LDLIBSOPTIONS=

.build-conf:
	"${MAKE}" -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inventorysystem

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inventorysystem: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inventorysystem ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/fileHandler.o: fileHandler.cpp
	${MKDIR} -p ${OBJECTDIR}
	${COMPILE.cc} -g -o ${OBJECTDIR}/fileHandler.o fileHandler.cpp

${OBJECTDIR}/inventoryReports.o: inventoryReports.cpp
	${MKDIR} -p ${OBJECTDIR}
	${COMPILE.cc} -g -o ${OBJECTDIR}/inventoryReports.o inventoryReports.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${COMPILE.cc} -g -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/productManagement.o: productManagement.cpp
	${MKDIR} -p ${OBJECTDIR}
	${COMPILE.cc} -g -o ${OBJECTDIR}/productManagement.o productManagement.cpp

${OBJECTDIR}/productNode.o: productNode.cpp
	${MKDIR} -p ${OBJECTDIR}
	${COMPILE.cc} -g -o ${OBJECTDIR}/productNode.o productNode.cpp

${OBJECTDIR}/transactionManager.o: transactionManager.cpp
	${MKDIR} -p ${OBJECTDIR}
	${COMPILE.cc} -g -o ${OBJECTDIR}/transactionManager.o transactionManager.cpp

${OBJECTDIR}/transactionNode.o: transactionNode.cpp
	${MKDIR} -p ${OBJECTDIR}
	${COMPILE.cc} -g -o ${OBJECTDIR}/transactionNode.o transactionNode.cpp

${OBJECTDIR}/transactionQueue.o: transactionQueue.cpp
	${MKDIR} -p ${OBJECTDIR}
	${COMPILE.cc} -g -o ${OBJECTDIR}/transactionQueue.o transactionQueue.cpp

.clean-conf:
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inventorysystem
