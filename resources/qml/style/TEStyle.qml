pragma Singleton

import QtQuick 2.0

import customcomponents 1.0

QtObject {
    id: styleRoot

    readonly property int defaultMargin:                    8

    readonly property int defaultSpacing:                   4

    readonly property int defaultRadius:                    4

    readonly property real disabledOpacity:                 0.5

    readonly property int buttonWidth:                      92
    readonly property int buttonHeight:                     36

    readonly property int checkBoxSize:                     24

    readonly property int defaultHeaderHeight:              36

    readonly property int listItemHeight:                   preferences.listRowHeight

    readonly property int defaultCellHeight:                listItemHeight

    readonly property int defaultPopupItemWidth:            128
    readonly property int popupItemHeight:                  24

    readonly property int scrollIndicatorWidth:             10

    readonly property int grabAreaSize:                     4

    readonly property int defaultDurationShort:             100
    readonly property int defaultDurationNormal:            defaultDurationShort * 2
    readonly property int defaultDurationLong:              defaultDurationShort * 5


    readonly property string defaultColorGradient1:         themeColorGradient1[preferences.guiTheme]               //"#FF111111"
    readonly property string defaultColorGradient2:         themeColorGradient2[preferences.guiTheme]               //"#FF222222"

    readonly property string defaultColorBackground:        themeColorBackground[preferences.guiTheme]              //"#AA000000"

    readonly property string defaultColorText:              themeColorText[preferences.guiTheme]                    //"Snow"

    readonly property string defaultColorBorder:            themeColorBorder[preferences.guiTheme]                  //"Gray"

    readonly property string defaultColorHeader:            themeColorHeader[preferences.guiTheme]                  //"#FF333333"

    readonly property string defaultColorSection:           themeColorSection[preferences.guiTheme]                 //"transparent"

    readonly property string defaultColorButton:            themeColorButton[preferences.guiTheme]                  //"#22FFFFFF"
    readonly property string defaultColorButtonPressed:     themeColorButtonPressed[preferences.guiTheme]           //"#77FFFFFF"
    readonly property string defaultColorButtonChecked:     themeColorButtonChecked[preferences.guiTheme]           //"#44FFFFFF"

    readonly property string defaultColorRowAlternate:      themeColorRowAlternate[preferences.guiTheme]            //"#07FFA500"

    readonly property string defaultColorRowSelected:       themeColorRowSelected[preferences.guiTheme]             //"#77FFA500"

    readonly property string defaultColorRowWarning:        themeColorRowWarning[preferences.guiTheme]              //"#44770000"

    readonly property string defaultColorListHighlight:     themeColorListHighlight[preferences.guiTheme]           //"#44FFFFFF"

    readonly property string defaultColorGlow:              themeColorGlow[preferences.guiTheme]                    //"#FFAAAAAA"

    readonly property string defaultColorSelect:            themeColorSelected[preferences.guiTheme]

    readonly property int defaultFontSizeS:                 12
    readonly property int defaultFontSizeM:                 16
    readonly property int defaultFontSizeL:                 20
    readonly property int defaultFontSizeXL:                24
    readonly property int defaultFontSizeXXL:               34

//    property QtObject fontLoader: FontLoader {
//        source: "qrc:/resources/fonts/Instrument103.ttf"
//    }

//    property Connections preferencesConnections: Connections {
//        target: preferences

//        onGuiThemeChanged: {
//            //console.log("#### TEStyle.preferencesConnections.onGuiThemeChanged - aTheme: " + aTheme);

//            // ...
//        }
//    }

    // Theme Colors
    readonly property var themeColorGradient1:              ["White"        , "#FF070707"   , preferences.colorBackground               ]
    readonly property var themeColorGradient2:              ["White"        , "#FF111111"   , Qt.lighter(preferences.colorBackground, 1.15) ]
    readonly property var themeColorBackground:             ["Snow"         , "#BB010101"   , preferences.colorBackground               ]
    readonly property var themeColorText:                   ["Black"        , "Snow"        , preferences.colorText                     ]
    readonly property var themeColorBorder:                 ["Gray"         , "Gray"        , preferences.colorBorder                   ]
    readonly property var themeColorHeader:                 ["#FFBBBBBB"    , "#FF333333"   , preferences.colorHeader                   ]
    readonly property var themeColorSection:                ["transparent"  , "transparent" , "transparent"                             ]
    readonly property var themeColorButton:                 ["#22777777"    , "#22FFFFFF"   , "#22FFFFFF"                               ]
    readonly property var themeColorButtonPressed:          ["#77777777"    , "#77FFFFFF"   , "#77FFFFFF"                               ]
    readonly property var themeColorButtonChecked:          ["#44888888"    , "#44FFFFFF"   , "#44FFFFFF"                               ]
    readonly property var themeColorRowAlternate:           ["#0F777777"    , "#0F777777"   , preferences.colorRowAlt                   ]
    readonly property var themeColorRowSelected:            ["#77FFA500"    , "#77FFA500"   , "#77FFA500"                               ]
    readonly property var themeColorRowWarning:             ["#22770000"    , "#44770000"   , "#44770000"                               ]
    readonly property var themeColorListHighlight:          ["#44777777"    , "#22EEEEEE"   , preferences.colorHighlight                ]
    readonly property var themeColorGlow:                   ["#FFFFFFAA"    , "#FFAAAAAA"   , "#FFAAAAAA"                               ]
    readonly property var themeColorSelected:               ["Orange"       , "Orange"      , "Orange"                                  ]

    Component.onCompleted: {
        console.log("TEStyle.onCompleted - guiTheme: " + preferences.guiTheme);

        // ...
    }

    // ...
}
