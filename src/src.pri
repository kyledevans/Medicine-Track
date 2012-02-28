QT += core gui sql

SOURCES += src/globals.cpp \
	src/main.cpp \
	src/mainwindow.cpp \
	src/formularyframe.cpp \
	src/inventoryframe.cpp \
	src/patientframe.cpp \
	src/pharmacistframe.cpp \
	src/prescriberframe.cpp \
	src/prescriptionframe.cpp \
	src/barcodelabel.cpp \
	src/prescriptionlabel.cpp \
	src/mtconfig.cpp \
	src/mtsettings.cpp

HEADERS	+= src/globals.h \
	src/mainwindow.h \
	src/formularyframe.h \
	src/inventoryframe.h \
	src/patientframe.h \
	src/pharmacistframe.h \
	src/prescriberframe.h \
	src/prescriptionframe.h \
	src/barcodelabel.h \
	src/prescriptionlabel.h \
	src/mtconfig.h \
	src/mtsettings.h

FORMS	+= src/mainwindow.ui \
	src/formularyframe.ui \
	src/inventoryframe.ui \
	src/patientframe.ui \
	src/pharmacistframe.ui \
	src/prescriberframe.ui \
	src/prescriptionframe.ui \
	src/barcodelabel.ui \
	src/prescriptionlabel.ui \
	src/mtconfig.ui
