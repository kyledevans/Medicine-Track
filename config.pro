QT += core gui sql

TARGET = mtrack
TEMPLATE = app
CONFIG += embed_manifests_exe

INCLUDEPATH = src/db \
	src/wizards \
	src

DESTDIR = build

OBJECTS_DIR = build

MOC_DIR = build

OTHER_FILES = barcodes.txt \
	COPYING.txt \
	copyright_template.txt \
	database_info.txt \
	HACKING.txt \
	license_template.txt \
	README.txt \
	todo.txt

include(src/db/db.pri)
include(src/wizards/wizards.pri)
include(src/src.pri)
include(doc/doc.pri)
