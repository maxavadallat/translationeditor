#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlEngine>

#include "packageinfo.h"
#include "translationitemlayout.h"
#include "constants.h"
#include "shared/translation_keys.h"
#include "preferences.h"

//==============================================================================
// Static Constructor - From JSON Object
//==============================================================================
TranslationItemLayout* TranslationItemLayout::fromJSON(const QJsonObject& aObject, PackageInfo* aPackage)
{
    // Init New Translation Item Layout
    TranslationItemLayout* newLayout = nullptr;

    // Check JSON Object
    if (!aObject.isEmpty()) {
        // Create New Layout
        newLayout = new TranslationItemLayout(aPackage);

        // Set Layout Values
        newLayout->mName = aObject.value(PACKAGE_KEY_TEXT_LAYOUT_NAME).toString();

        newLayout->mFontName = aObject.value(PACKAGE_KEY_TEXT_FONT_NAME).toString(aPackage ? aPackage->defaultFont() : "");
        newLayout->mFontSize = aObject.value(PACKAGE_KEY_TEXT_FONT_SIZE).toInt(aPackage ? aPackage->defaultFontSize() : DEFAULT_TEXT_FONT_SIZE);

        newLayout->mMaxWidth = aObject.value(PACKAGE_KEY_TEXT_MAX_WIDTH).toInt(DEFAULT_MAX_WIDTH);
        newLayout->mMaxHeight = aObject.value(PACKAGE_KEY_TEXT_MAX_HEIGHT).toInt(DEFAULT_MAX_HEIGHT);

        newLayout->mMaxLength = aObject.value(PACKAGE_KEY_TEXT_MAX_LENGTH).toInt(DEFAULT_MAX_LENGTH);

        newLayout->mMaxLines = aObject.value(PACKAGE_KEY_TEXT_MAX_LINES).toInt(DEFAULT_MAX_LINES);

        newLayout->mWrapType = aObject.value(PACKAGE_KEY_TEXT_WRAP).toInt(DEFAULT_WRAP_TYPE);
        newLayout->mElideType = aObject.value(PACKAGE_KEY_TEXT_ELIDE).toInt(DEFAULT_ELIDE_TYPE);

        newLayout->mShrinkable = aObject.value(PACKAGE_KEY_TEXT_CAN_SHRINK).toBool(false);

        newLayout->mAlternativeFonts = aObject.value(PACKAGE_KEY_TEXT_ALTERNATIVE_FONTS).toString().split(";", QString::SkipEmptyParts);

        // Get Alternative Font Indexes Array
        QJsonArray altFontsIndexesArray = aObject.value(PACKAGE_KEY_TEXT_ALT_FONT_INDEXES).toArray();

        // Iterate Through Alternative Font Indexes
        for (QJsonValue layoutValue : altFontsIndexesArray) {
            // Get Values
            QStringList altFontIndex = layoutValue.toString().split(";");
            // Append Alternative Font Index
            newLayout->mAltFontIndexes[altFontIndex[0]] = altFontIndex[1].toInt();
        }

        // ...
    }

    return newLayout;
}

//==============================================================================
// Constructor
//==============================================================================
TranslationItemLayout::TranslationItemLayout(PackageInfo* aPackage, QObject* aParent)
    : QObject(aParent)
    , mPackage(aPackage)
    , mName("")
    , mFontName(mPackage ? mPackage->defaultFont() : "")
    , mFontSize(mPackage ? mPackage->defaultFontSize() : DEFAULT_TEXT_FONT_SIZE)
    , mMaxWidth(DEFAULT_MAX_WIDTH)
    , mMaxHeight(DEFAULT_MAX_HEIGHT)
    , mMaxLines(DEFAULT_MAX_LINES)
    , mWrapType(DEFAULT_WRAP_TYPE)
    , mElideType(DEFAULT_ELIDE_TYPE)
    , mMaxLength(DEFAULT_MAX_LENGTH)
    , mShrinkable(false)
    , mAlternativeFonts(QStringList())
{
    // Set Ownership
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // ...
}

//==============================================================================
// Get Layout Name
//==============================================================================
QString TranslationItemLayout::name()
{
    return mName;
}

//==============================================================================
// Set Layout Name
//==============================================================================
void TranslationItemLayout::setName(const QString& aNewName)
{
    // Check Name
    if (mName != aNewName) {
        // Set Name
        mName = aNewName;
        // Emit Name Changed Signal
        emit nameChanged(mName);

        // Set Package Dirty
        setPackageDirty();
    }
}

//==============================================================================
// Get Font Name
//==============================================================================
QString TranslationItemLayout::fontName()
{
    return mFontName;
}

//==============================================================================
// Set Font Name
//==============================================================================
void TranslationItemLayout::setFontName(const QString& aFontName)
{
    // Check Font Name
    if (mFontName != aFontName) {
        // Set Font Name
        mFontName = aFontName;
        // Emit Font Name Changed Signal
        emit fontNameChanged(mFontName);

        // Emit Layout Changed
        emit layoutChanged();

        // Set Package Dirty
        setPackageDirty();
    }
}

//==============================================================================
// Get Font Size
//==============================================================================
int TranslationItemLayout::fontSize()
{
    return mFontSize;
}

//==============================================================================
// Set Font Size
//==============================================================================
void TranslationItemLayout::setFontSize(const int& aFontSize)
{
    // Check Font Size
    if (mFontSize != aFontSize) {
        // Set Font Size
        mFontSize = aFontSize;
        // Emit Font Size Changed Signal
        emit fontSizeChanged(mFontSize);

        // Emit Layout Changed
        emit layoutChanged();

        // Set Package Dirty
        setPackageDirty();
    }
}

//==============================================================================
// Get Max Width
//==============================================================================
int TranslationItemLayout::maxWidth()
{
    return mMaxWidth;
}

//==============================================================================
// Set Max Width
//==============================================================================
void TranslationItemLayout::setMaxWidth(const int& aMaxWidth)
{
    // Check Max Width
    if (mMaxWidth != aMaxWidth) {
        // Set Max Width
        mMaxWidth = aMaxWidth;
        // Emit Max Width Changed Signal
        emit maxWidthChanged(mMaxWidth);

        // Emit Layout Changed
        emit layoutChanged();

        // Set Package Dirty
        setPackageDirty();
    }
}

//==============================================================================
// Get Max Height
//==============================================================================
int TranslationItemLayout::maxHeight()
{
    return mMaxHeight;
}

//==============================================================================
// Set Max Height
//==============================================================================
void TranslationItemLayout::setMaxHeight(const int& aMaxHeight)
{
    // Check Max Height
    if (mMaxHeight != aMaxHeight) {
        // Set Max Height
        mMaxHeight = aMaxHeight;
        // Emit Max Height Changed Signal
        emit maxHeightChanged(mMaxHeight);

        // Emit Layout Changed
        emit layoutChanged();

        // Set Package Dirty
        setPackageDirty();
    }
}

//==============================================================================
// Get Max Lines
//==============================================================================
int TranslationItemLayout::maxLines()
{
    return mMaxLines;
}

//==============================================================================
// Set Max Lines
//==============================================================================
void TranslationItemLayout::setMaxLines(const int& aMaxLines)
{
    // Check Max Lines
    if (mMaxLines != aMaxLines) {
        // Set Max Lines
        mMaxLines = aMaxLines;
        // Emit Max Lines Changed Signal
        emit maxLinesChanged(mMaxLines);

        // Emit Layout Changed
        emit layoutChanged();

        // Set Package Dirty
        setPackageDirty();
    }
}

//==============================================================================
// Get Wrap Type
//==============================================================================
int TranslationItemLayout::wrapType()
{
    return mWrapType;
}

//==============================================================================
// Set Wrap Type
//==============================================================================
void TranslationItemLayout::setWrapType(const int& aWrapType)
{
    // Check Wrap Type
    if (mWrapType != aWrapType) {
        // Set Wrap Type
        mWrapType = aWrapType;
        // Emit Wrap Type Changed Signal
        emit wrapTypeChanged(mWrapType);

        // Emit Layout Changed
        emit layoutChanged();

        // Set Package Dirty
        setPackageDirty();
    }
}

//==============================================================================
// Get Elide Type
//==============================================================================
int TranslationItemLayout::elideType()
{
    return mElideType;
}

//==============================================================================
// Set Elide Type
//==============================================================================
void TranslationItemLayout::setElideType(const int& aElideType)
{
    // Check Elide Type
    if (mElideType != aElideType) {
        // Set Elide Type
        mElideType = aElideType;
        // Emit Elide Type Changed Signal
        emit elideTypeChanged(mElideType);

        // Emit Layout Changed
        emit layoutChanged();

        // Set Package Dirty
        setPackageDirty();
    }
}

//==============================================================================
// Get Max Length
//==============================================================================
int TranslationItemLayout::maxLength()
{
    return mMaxLength;
}

//==============================================================================
// Set Max Length
//==============================================================================
void TranslationItemLayout::setMaxLength(const int& aMaxLength)
{
    // Check Max Length
    if (mMaxLength != aMaxLength) {
        // Set Max Length
        mMaxLength = aMaxLength;
        // Emit Max Length Changed Signal
        emit maxLengthChanged(mMaxLength);

        // Emit Layout Changed
        emit layoutChanged();

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Shrinkable
//==============================================================================
bool TranslationItemLayout::shrinkable()
{
    return mShrinkable;
}

//==============================================================================
// Set Shrinkable
//==============================================================================
void TranslationItemLayout::setShrinkable(const bool& aShrinkable)
{
    // Check Shrinkable
    if (mShrinkable != aShrinkable) {
        // Set Shrinkable
        mShrinkable = aShrinkable;
        // Emit Shrinkable Changed Signal
        emit shrinkableChanged(mShrinkable);

        // Check Shrinkable
        if (!mShrinkable) {
            // Clear Alternative Fonr Indexes
            clearAlternativeFontIndexes();
        }

        // Emit Layout Changed
        emit layoutChanged();

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Alternative Fonts
//==============================================================================
QStringList TranslationItemLayout::altFonts()
{
    return mAlternativeFonts;
}

//==============================================================================
// Set Alternative Fonts
//==============================================================================
void TranslationItemLayout::setAltFonts(const QStringList& aAltFonts)
{
    // Check Alternative Fonts
    if (mAlternativeFonts != aAltFonts) {
        // Set Alternative Fonts
        mAlternativeFonts = aAltFonts;
        // Emit Alternative Fonts Changed Signal
        emit altFontsChanged(mAlternativeFonts);

        // Emit Layout Changed
        emit layoutChanged();

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Alternative Font Index
//==============================================================================
int TranslationItemLayout::getAlternativeFontIndex(const QString& aLanguage)
{
    // Check Language
    if (!aLanguage.isEmpty()) {
        int result = mAltFontIndexes.value(aLanguage, -1);
        return result;
    }

    return -1;
}

//==============================================================================
// Set Alternative Font Index
//==============================================================================
void TranslationItemLayout::setAlternativeFontIndex(const QString& aLanguage, const int& aIndex)
{
    // Check Language
    if (!aLanguage.isEmpty()) {
        qDebug() << "TranslationItemLayout::setAlternativeFontIndex - aLanguage: " << aLanguage << " - aIndex: " << aIndex;

        // Get Language Index
        int lIndex = mAltFontIndexes.keys().indexOf(aLanguage);

        // Check Index
        if (aIndex == -1 && lIndex != -1) {
            // Remove Alternative Font Index
            mAltFontIndexes.remove(aLanguage);

//            // Emit Layout Changed
//            emit layoutChanged();

            // Set Package Dirty State
            setPackageDirty();

        } else {
            // Set Alternative Font Index for Language
            mAltFontIndexes[aLanguage] = aIndex;

//            // Emit Layout Changed
//            emit layoutChanged();

            // Set Package Dirty State
            setPackageDirty();
        }
    }
}

//==============================================================================
// Clear Alternative Font Indexes
//==============================================================================
void TranslationItemLayout::clearAlternativeFontIndexes()
{
    qDebug() << "TranslationItemLayout::clearAlternativeFontIndexes";

    // Clear Alternative Font Indexes
    mAltFontIndexes.clear();

    // Emit Layout Changed
    emit layoutChanged();

    // Set Package Dirty State
    setPackageDirty();
}

//==============================================================================
// Export To JSON
//==============================================================================
QJsonObject TranslationItemLayout::exportToJSON()
{
    // Init New JSON Object
    QJsonObject newObject;

    // Check Current Package
    if (mPackage) {
        // Check Name
        if (!mName.isEmpty()) {
            // Set Name
            newObject[PACKAGE_KEY_TEXT_LAYOUT_NAME] = mName;
        }

        // Check Font Name
        if (mFontName != mPackage->defaultFont()) {
            // Set Font
            newObject[PACKAGE_KEY_TEXT_FONT_NAME] = mFontName;
        }

        // Check Font Size
        if (mFontSize != mPackage->defaultFontSize()) {
            // Set Font Size
            newObject[PACKAGE_KEY_TEXT_FONT_SIZE] = mFontSize;
        }

        // Check Max Width
        if (mMaxWidth != DEFAULT_MAX_WIDTH) {
            // Set Max Width
            newObject[PACKAGE_KEY_TEXT_MAX_WIDTH] = mMaxWidth;
        }

        // Check Max Height
        if (mMaxHeight != DEFAULT_MAX_HEIGHT) {
            // Set Max Width
            newObject[PACKAGE_KEY_TEXT_MAX_HEIGHT] = mMaxHeight;
        }

        // Check Max Lines
        if (mMaxLines != DEFAULT_MAX_LINES) {
            // Set Max Lines
            newObject[PACKAGE_KEY_TEXT_MAX_LINES] = mMaxLines;
        }

        // Check Max Length
        if (mMaxLength != DEFAULT_MAX_LENGTH) {
            // Set Max Length
            newObject[PACKAGE_KEY_TEXT_MAX_LENGTH] = mMaxLength;
        }

        // Check Wrap Type
        if (mWrapType != DEFAULT_WRAP_TYPE) {
            // Set Wrap Type
            newObject[PACKAGE_KEY_TEXT_WRAP] = mWrapType;
        }

        // Check Elide Type
        if (mElideType != DEFAULT_ELIDE_TYPE) {
            // Set Elide Type
            newObject[PACKAGE_KEY_TEXT_ELIDE] = mElideType;
        }

        // Check Shrinkable
        if (mShrinkable) {
            // Set Shrinkable
            newObject[PACKAGE_KEY_TEXT_CAN_SHRINK] = mShrinkable;
        }

        // Check Alternative Fonts
        if (mAlternativeFonts.count() > 0) {
            // Set ALternative Fonts
            newObject[PACKAGE_KEY_TEXT_ALTERNATIVE_FONTS] = mAlternativeFonts.join(";");
        }

        // Check Alternative Font Index
        if (mAltFontIndexes.count() > 0) {
            // Init Alternative Font Indexes Array
            QJsonArray newAltFontIndexesArray = QJsonArray();

            // Iterate Through Alt Font Indexes
            for (QString key : mAltFontIndexes.keys()) {
                // Init Key Value Pair String
                QString valueString = QString("%1;%2").arg(key).arg(mAltFontIndexes.value(key));
                // Append to Array
                newAltFontIndexesArray << valueString;
            }

            // Set Alternative Font Index
            newObject[PACKAGE_KEY_TEXT_ALT_FONT_INDEXES] = newAltFontIndexesArray;
        }

        // Check New Object
        if (newObject.isEmpty()) {
            // Set Default
            newObject[PACKAGE_KEY_TEXT_LAYOUT_DEFAULT] = PACKAGE_KEY_TEXT_LAYOUT_DEFAULT;
        }

    }

    return newObject;
}

//==============================================================================
// Set Package Dirty
//==============================================================================
void TranslationItemLayout::setPackageDirty()
{
    // Check Package
    if (mPackage) {
        // Set Dirty
        mPackage->setDirty();
    }
}
