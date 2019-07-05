#ifndef CONSTANTS_H
#define CONSTANTS_H

#define ORGANIZATION_NAME                               "harman"

#define ORGANIZATION_DOMAIN                             "harman.com"

#define APPLICATION_NAME                                "translationeditor"

#define DEFAULT_APPLICATION_DISPLAYNAME                 "Translation Editor"

#define APPLICATION_VERSION                             "0.1.0"

#define SUPPORTED_LANGUAGES                             "ar-EG;bg-BG;bn-BN;cs-CZ;da-DK;de-AT;de-CH;de-DE;de-LU;el-GR;en-AU;en-CA;" \
                                                        "en-GB;en-IE;en-NZ;en-US;es-AR;es-ES;es-MX;et-ET;fi-FI;fr-FR;he-IL;hr-HR;" \
                                                        "hu-HU;ic-IC;it-IT;ja-JA;ko-KO;lt-LT;lv-LV;mt-MT;nb-NO;nl-BE;nl-NL;pl-PL;" \
                                                        "pt-BR;pt-PT;ro-RO;ru-RU;sk-SK;sl-SL;sr-SP;sv-SE;tr-TR;uk-UA;zh-CN;zn-HK"

#define DEFAULT_PACKAGE_FILENAME_SUFFIX                 ".json"

#define DEFAULT_PREFERENCES_FILE_NAME                   "tepreferences.ini"

#define DEFAULT_RECENT_FILES_NAME                       "terecents.ini"

#define DEFAULT_SEARCH_HISTORY_NAME                     "tesearchhistory.ini"

#define MAIN_QML_SOURCE                                 "qrc:/resources/qml/mainQML.qml"

#define PREVIEW_QML_SOURCE                              "qrc:/resources/qml/previewQML.qml"

#define PACKAGE_PREFERENCES_QML_SOURCE                  "qrc:/resources/qml/packagePreferencesQML.qml"

#define COLOR_PREVIEW_SOURCE                            "qrc:/resources/qml/colorsQML.qml"

#define SEARCH_RESULTS_SOURCE                           "qrc:/resources/qml/searchResultsQML.qml"

#define CONTEXT_PROPERTY_MAIN_CONTROLLER                "mainController"

#define CONTEXT_PROPERTY_PREFERENCES                    "preferences"

#define CONTEXT_PROPERTY_PACKAGE_CONTROLLER             "packageController"

#define CONTEXT_PROPERTY_SEARCH_CONTROLLER              "searchController"

#define CONTEXT_PROPERTY_SEARCH_RESULT_MODEL            "searchResultModel"

#define DEFAULT_CUSTOM_COMPONENTS                       "customcomponents"

#define CUSTOM_COMPONENT_NAME_PACKAGE_INFO              "PackageInfo"

#define CUSTOM_COMPONENT_NAME_PACKAGE_GROUP             "PackageGroup"

#define CUSTOM_COMPONENT_NAME_TRANSLATION_ITEM          "TranslationItem"

#define CUSTOM_COMPONENT_NAME_TRANSLATION_LAYOUT        "TranslationItemLayout"

#define CUSTOM_COMPONENT_NAME_PACKAGE_FILTER            "PackageGroupFilter"

#define DEFAULT_PLUGIN_IID_PREFIX                       "org.harman.translationeditor"

#define DEFAULT_RECENT_FILES_MAX                        9

#define DEFAULT_STATUS_MESSAGE_TIMEOUT                  4000

#endif // CONSTANTS_H
