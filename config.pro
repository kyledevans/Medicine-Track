QT += core gui sql

TARGET = mtrack
TEMPLATE = app
CONFIG += embed_manifests_exe

INCLUDEPATH = src \
	src/db

DESTDIR = build

OBJECTS_DIR = build

MOC_DIR = build

OTHER_FILES = database_info.txt \
	barcodes.txt \
	build_instructions.txt

include(src/db/db.pri)
include(src/src.pri)
include(doc/doc.pri)
