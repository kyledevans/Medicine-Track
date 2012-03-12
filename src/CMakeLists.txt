CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/db")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/wizards")

SET(SOURCES main.cpp)
SET(SOURCES ${SOURCES}; mainwindow.cpp)
SET(SOURCES ${SOURCES}; formularyframe.cpp)
SET(SOURCES ${SOURCES}; inventoryframe.cpp)
SET(SOURCES ${SOURCES}; patientframe.cpp)
SET(SOURCES ${SOURCES}; pharmacistframe.cpp)
SET(SOURCES ${SOURCES}; prescriberframe.cpp)
SET(SOURCES ${SOURCES}; prescriptionframe.cpp)
SET(SOURCES ${SOURCES}; barcodelabel.cpp)
SET(SOURCES ${SOURCES}; prescriptionlabel.cpp)
SET(SOURCES ${SOURCES}; mtconfig.cpp)
SET(SOURCES ${SOURCES}; mtsettings.cpp)

SET(HEADERS ${HEADERS}; mtsettings.h)

SET(MOCS mainwindow.h)
SET(MOCS ${MOCS}; formularyframe.h)
SET(MOCS ${MOCS}; inventoryframe.h)
SET(MOCS ${MOCS}; patientframe.h)
SET(MOCS ${MOCS}; pharmacistframe.h)
SET(MOCS ${MOCS}; prescriberframe.h)
SET(MOCS ${MOCS}; prescriptionframe.h)
SET(MOCS ${MOCS}; barcodelabel.h)
SET(MOCS ${MOCS}; prescriptionlabel.h)
SET(MOCS ${MOCS}; mtconfig.h)

SET(FORMS mainwindow.ui)
SET(FORMS ${FORMS}; formularyframe.ui)
SET(FORMS ${FORMS}; inventoryframe.ui)
SET(FORMS ${FORMS}; patientframe.ui)
SET(FORMS ${FORMS}; pharmacistframe.ui)
SET(FORMS ${FORMS}; prescriberframe.ui)
SET(FORMS ${FORMS}; prescriptionframe.ui)
SET(FORMS ${FORMS}; barcodelabel.ui)
SET(FORMS ${FORMS}; prescriptionlabel.ui)
SET(FORMS ${FORMS}; mtconfig.ui)

QT4_WRAP_CPP(MOC_HEADERS ${MOCS})
QT4_WRAP_UI(FORM_HEADERS ${FORMS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(mtrack WIN32 ${SOURCES} ${MOC_HEADERS} ${FORM_HEADERS})
TARGET_LINK_LIBRARIES(mtrack db wizards ${QT_LIBRARIES} ${MYSQL_LIBRARIES})

ADD_CUSTOM_TARGET(translations_update ALL
	COMMAND lupdate -no-obsolete "${PROJECT_SOURCE_DIR}/${DB_DIR}" "${PROJECT_SOURCE_DIR}/${WIZARDS_DIR}" "${PROJECT_SOURCE_DIR}/${EXE_DIR}" -ts ${PROJECT_SOURCE_DIR}/${TRANSLATIONS_TS}
)

ADD_CUSTOM_TARGET(translations ALL
	COMMAND lrelease ${PROJECT_SOURCE_DIR}/${TRANSLATIONS_TS} -qm ${CMAKE_CURRENT_BINARY_DIR}/${TRANSLATIONS_QM}
)

IF(WIN32)
	FILE(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}\\${IMAGES_DIR}" NATIVE_IMAGES_DIR)
	FILE(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}\\${IMAGES_DIR}" NATIVE_CMAKE_IMAGES_DIR)
	ADD_CUSTOM_TARGET(images ALL
		COMMAND xcopy ${NATIVE_IMAGES_DIR} ${NATIVE_CMAKE_IMAGES_DIR} /I /Y
	)
ENDIF()
