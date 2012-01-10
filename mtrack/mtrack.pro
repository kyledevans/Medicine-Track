#-------------------------------------------------
#
# Project created by QtCreator 2011-12-21T19:36:52
#
#-------------------------------------------------

QT       += core gui sql

TARGET = mtrack
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    patientsearch.cpp \
    prescriptionsframe.cpp \
    medicationsframe.cpp \
    inventoryframe.cpp \
    newpatientwizard.cpp \
    patientrecord.cpp \
    altermedicationwizard.cpp \
    medicationrecord.cpp \
    globals.cpp \
    altershipmentwizard.cpp \
    shipmentrecord.cpp \
    alertinterface.cpp \
    prescriptionrecord.cpp \
    prescriptionlabel.cpp

HEADERS  += mainwindow.h \
    patientsearch.h \
    prescriptionsframe.h \
    medicationsframe.h \
    inventoryframe.h \
    newpatientwizard.h \
    patientrecord.h \
    altermedicationwizard.h \
    medicationrecord.h \
    globals.h \
    altershipmentwizard.h \
    shipmentrecord.h \
    alertinterface.h \
    prescriptionrecord.h \
    prescriptionlabel.h

FORMS    += mainwindow.ui \
    patientsearch.ui \
    prescriptionsframe.ui \
    medicationsframe.ui \
    inventoryframe.ui \
    newpatientwizard.ui \
    altermedicationwizard.ui \
    altershipmentwizard.ui \
    prescriptionlabel.ui

OTHER_FILES += \
    database_info.txt







