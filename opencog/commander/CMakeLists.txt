ADD_GUILE_MODULE (FILES
	action-selector.scm
	main.scm
	openpsi.scm
	rule.scm
	utilities.scm
	MODULE_DESTINATION "${DATADIR}/scm/opencog/commander"
)


ADD_LIBRARY (commander SHARED
	OpenPsiImplicator.cc
	OpenPsiRules.cc
	OpenPsiSCM.cc
)

TARGET_LINK_LIBRARIES (commander
	${ATOMSPACE_LIBRARIES}
)

INSTALL (TARGETS commander DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")

INSTALL (FILES OpenPsiImplicator.h
	DESTINATION "include/opencog/commander/"
)
