QT += core gui sql

SOURCES += src/db/alertinterface.cpp \
	src/db/patientrecord.cpp \
	src/db/prescriptionrecord.cpp \
	src/db/prescriberrecord.cpp \
	src/db/pharmacistrecord.cpp \
	src/db/shipmentrecord.cpp \
	src/db/drugrecord.cpp \
	src/db/writeoffrecord.cpp \
	src/db/patientdisplay.cpp \
	src/db/drugdisplay.cpp \
    src/db/pharmacistdisplay.cpp \
    src/db/prescriberdisplay.cpp \
    src/db/shipmentdisplay.cpp \
    src/db/prescriptiondisplay.cpp \
    src/db/patientgroupbox.cpp \
    src/db/druggroupbox.cpp \
    src/db/pharmacistgroupbox.cpp \
    src/db/prescribergroupbox.cpp \
    src/db/prescriptiongroupbox.cpp \
    src/db/shipmentgroupbox.cpp

HEADERS += src/db/alertinterface.h \
	src/db/patientrecord.h \
	src/db/prescriptionrecord.h \
	src/db/prescriberrecord.h \
	src/db/pharmacistrecord.h \
	src/db/shipmentrecord.h \
	src/db/drugrecord.h \
	src/db/writeoffrecord.h \
	src/db/patientdisplay.h \
	src/db/drugdisplay.h \
    src/db/pharmacistdisplay.h \
    src/db/prescriberdisplay.h \
    src/db/shipmentdisplay.h \
    src/db/prescriptiondisplay.h \
    src/db/patientgroupbox.h \
    src/db/druggroupbox.h \
    src/db/pharmacistgroupbox.h \
    src/db/prescribergroupbox.h \
    src/db/prescriptiongroupbox.h \
    src/db/shipmentgroupbox.h

FORMS += src/db/patientdisplay.ui \
	src/db/drugdisplay.ui \
    src/db/pharmacistdisplay.ui \
    src/db/prescriberdisplay.ui \
    src/db/shipmentdisplay.ui \
    src/db/prescriptiondisplay.ui \
    src/db/patientgroupbox.ui \
    src/db/druggroupbox.ui \
    src/db/pharmacistgroupbox.ui \
    src/db/prescribergroupbox.ui \
    src/db/prescriptiongroupbox.ui \
    src/db/shipmentgroupbox.ui
