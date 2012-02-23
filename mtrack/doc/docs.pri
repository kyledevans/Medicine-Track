QTHELP_FILES = doc/mtrack-collection.qhcp

qthelp_gen.input = QTHELP_FILES
qthelp_gen.output = $(QMAKE_FILE_BASE).qhc
qthelp_gen.commands = qcollectiongenerator ${QMAKE_FILE_NAME}
qthelp_gen.CONFIG = no_link target_predeps

QMAKE_EXTRA_COMPILERS += qthelp_gen

OTHER_FILES += \
	doc/doc.qhp \
	doc/mtrack-collection.qhcp \
	doc/new_patient.html \
	doc/new_prescription.html \
	doc/new_medication.html \
	doc/new_prescriber.html \
	doc/new_pharmacist.html \
	doc/new_inventory.html \
	doc/printing_labels.html \
	doc/search_patients.html \
	doc/search_prescriptions.html \
	doc/search_formulary.html \
	doc/search_prescribers.html \
	doc/search_pharmacists.html \
	doc/search_inventory.html \
	doc/toggle_active.html \
	doc/write_off_inventory.html
