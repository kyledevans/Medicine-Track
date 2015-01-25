import qbs.base 1.0

QtGuiApplication {
    name: "mtrack"

    Depends {
        name: "Qt"
        submodules: [
            "printsupport",
            "sql",
            "widgets"
        ]
        required: true
    }

    cpp.includePaths: [
        "./",
        "./db",
        "./wizards"
    ]

    files: [
        // DB headers
        "db/alertinterface.h",
        "db/displaytable.h",
        "db/drugdisplay.h",
        "db/druggroupbox.h",
        "db/drugrecord.h",
        "db/globals.h",
        "db/patientdisplay.h",
        "db/patientgroupbox.h",
        "db/patientrecord.h",
        "db/pharmacistdisplay.h",
        "db/pharmacistgroupbox.h",
        "db/pharmacistrecord.h",
        "db/prescriberdisplay.h",
        "db/prescribergroupbox.h",
        "db/prescriberrecord.h",
        "db/prescriptiondisplay.h",
        "db/prescriptiongroupbox.h",
        "db/prescriptionrecord.h",
        "db/shipdeltarecord.h",
        "db/shipmentdisplay.h",
        "db/shipmentgroupbox.h",
        "db/shipmentrecord.h",
        // DB sources
        "db/alertinterface.cpp",
        "db/displaytable.cpp",
        "db/drugdisplay.cpp",
        "db/druggroupbox.cpp",
        "db/drugrecord.cpp",
        "db/globals.cpp",
        "db/patientdisplay.cpp",
        "db/patientgroupbox.cpp",
        "db/patientrecord.cpp",
        "db/pharmacistdisplay.cpp",
        "db/pharmacistgroupbox.cpp",
        "db/pharmacistrecord.cpp",
        "db/prescriberdisplay.cpp",
        "db/prescribergroupbox.cpp",
        "db/prescriberrecord.cpp",
        "db/prescriptiondisplay.cpp",
        "db/prescriptiongroupbox.cpp",
        "db/prescriptionrecord.cpp",
        "db/shipdeltarecord.cpp",
        "db/shipmentdisplay.cpp",
        "db/shipmentgroupbox.cpp",
        "db/shipmentrecord.cpp",
        // DB UI files
        "db/drugdisplay.ui",
        "db/druggroupbox.ui",
        "db/patientdisplay.ui",
        "db/patientgroupbox.ui",
        "db/pharmacistdisplay.ui",
        "db/pharmacistgroupbox.ui",
        "db/prescriberdisplay.ui",
        "db/prescribergroupbox.ui",
        "db/prescriptiondisplay.ui",
        "db/prescriptiongroupbox.ui",
        "db/shipmentdisplay.ui",
        "db/shipmentgroupbox.ui",

        // Wizard headers
        "wizards/medication_00.h",
        "wizards/medication_01.h",
        "wizards/medicationwizard.h",
        "wizards/patient_00.h",
        "wizards/patientwizard.h",
        "wizards/pharmacist_00.h",
        "wizards/pharmacistwizard.h",
        "wizards/prescriber_00.h",
        "wizards/prescriberwizard.h",
        "wizards/prescription_00.h",
        "wizards/prescription_01.h",
        "wizards/prescription_02.h",
        "wizards/prescriptionwizard.h",
        "wizards/shipment_00.h",
        "wizards/shipmentwizard.h",
        // Wizard sources
        "wizards/medication_00.cpp",
        "wizards/medication_01.cpp",
        "wizards/medicationwizard.cpp",
        "wizards/patient_00.cpp",
        "wizards/patientwizard.cpp",
        "wizards/pharmacist_00.cpp",
        "wizards/pharmacistwizard.cpp",
        "wizards/prescriber_00.cpp",
        "wizards/prescriberwizard.cpp",
        "wizards/prescription_00.cpp",
        "wizards/prescription_01.cpp",
        "wizards/prescription_02.cpp",
        "wizards/prescriptionwizard.cpp",
        "wizards/shipment_00.cpp",
        "wizards/shipmentwizard.cpp",
        // Wizard UI files
        "wizards/medication_00.ui",
        "wizards/medication_01.ui",
        "wizards/medicationwizard.ui",
        "wizards/patient_00.ui",
        "wizards/patientwizard.ui",
        "wizards/pharmacist_00.ui",
        "wizards/pharmacistwizard.ui",
        "wizards/prescriber_00.ui",
        "wizards/prescriberwizard.ui",
        "wizards/prescription_00.ui",
        "wizards/prescription_01.ui",
        "wizards/prescription_02.ui",
        "wizards/prescriptionwizard.ui",
        "wizards/shipment_00.ui",
        "wizards/shipmentwizard.ui",

        // Core headers
        "core/barcodelabel.h",
        "core/formularyframe.h",
        "core/inventoryframe.h",
        "core/mainwindow.h",
        "core/mtconfig.h",
        "core/mtsettings.h",
        "core/patientframe.h",
        "core/pharmacistframe.h",
        "core/prescriberframe.h",
        "core/prescriptionframe.h",
        "core/prescriptionlabel.h",
        // Core sources
        "core/main.cpp",
        "core/barcodelabel.cpp",
        "core/formularyframe.cpp",
        "core/inventoryframe.cpp",
        "core/mainwindow.cpp",
        "core/mtconfig.cpp",
        "core/mtsettings.cpp",
        "core/patientframe.cpp",
        "core/pharmacistframe.cpp",
        "core/prescriberframe.cpp",
        "core/prescriptionframe.cpp",
        "core/prescriptionlabel.cpp",
        // Core UI files
        "core/barcodelabel.ui",
        "core/formularyframe.ui",
        "core/inventoryframe.ui",
        "core/mainwindow.ui",
        "core/mtconfig.ui",
        "core/patientframe.ui",
        "core/pharmacistframe.ui",
        "core/prescriberframe.ui",
        "core/prescriptionframe.ui",
        "core/prescriptionlabel.ui"
    ]
}


