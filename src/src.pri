QT += core gui sql

SOURCES += src/main.cpp\
	src/mainwindow.cpp \
	src/patientsearch.cpp \
	src/prescriptionsframe.cpp \
	src/medicationsframe.cpp \
	src/inventoryframe.cpp \
	src/globals.cpp \
	src/prescriptionlabel.cpp \
	src/prescriberframe.cpp \
	src/pharmacistframe.cpp \
	src/barcodelabel.cpp \
	src/mtconfig.cpp \
	src/mtsettings.cpp

HEADERS	+= src/mainwindow.h \
	src/patientsearch.h \
	src/prescriptionsframe.h \
	src/medicationsframe.h \
	src/inventoryframe.h \
	src/globals.h \
	src/prescriptionlabel.h \
	src/prescriberframe.h \
	src/pharmacistframe.h \
	src/barcodelabel.h \
	src/mtconfig.h \
	src/mtsettings.h

FORMS	+= src/mainwindow.ui \
	src/patientsearch.ui \
	src/prescriptionsframe.ui \
	src/medicationsframe.ui \
	src/inventoryframe.ui \
	src/prescriptionlabel.ui \
	src/prescriberframe.ui \
	src/pharmacistframe.ui \
	src/barcodelabel.ui \
	src/mtconfig.ui
