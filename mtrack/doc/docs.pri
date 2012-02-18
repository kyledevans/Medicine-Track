QTHELP_FILES = doc/mtrack-collection.qhcp

qthelp_gen.input = QTHELP_FILES
qthelp_gen.output = $(QMAKE_FILE_BASE).qhc
qthelp_gen.commands = qcollectiongenerator ${QMAKE_FILE_NAME}
qthelp_gen.CONFIG = no_link target_predeps

QMAKE_EXTRA_COMPILERS += qthelp_gen

OTHER_FILES += \
	doc/new_patient.html \
	doc/new_inventory.html \
	doc/new_formulary.html \
	doc/doc.qhp \
	doc/mtrack-collection.qhcp
