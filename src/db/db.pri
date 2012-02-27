QT += core gui sql

SOURCES += src/db/alertinterface.cpp \
	src/db/patientrecord.cpp \
	src/db/prescriptionrecord.cpp \
	src/db/medicationrecord.cpp \
	src/db/prescriberrecord.cpp \
	src/db/pharmacistrecord.cpp \
	src/db/shipmentrecord.cpp

HEADERS += src/db/alertinterface.h \
	src/db/patientrecord.h \
	src/db/prescriptionrecord.h \
	src/db/medicationrecord.h \
	src/db/prescriberrecord.h \
	src/db/pharmacistrecord.h \
	src/db/shipmentrecord.h
