#
# Generated Makefile - do not edit!
#
PROJECTNAME=InventorySystem
DEFAULTCONF=Debug
CONF=${DEFAULTCONF}
ALLCONFS=Debug Release

.build-impl: .build-pre
	"${MAKE}" -f nbproject/Makefile-${CONF}.mk .build-conf

.clean-impl: .clean-pre
	"${MAKE}" -f nbproject/Makefile-${CONF}.mk .clean-conf

.clobber-impl: .clobber-pre
	@for CONF in ${ALLCONFS}; do \
	    "${MAKE}" -f nbproject/Makefile-$${CONF}.mk .clean-conf; \
	done

.all-impl: .all-pre .build-impl

.build-tests-impl: .build-impl

.test-impl: .build-tests-impl

.help-impl: .help-pre
	@echo "Targets: build clean clobber all"
