#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QQmlEngine>

#include "translationitem.h"
#include "translationitemlayout.h"
#include "packageinfo.h"

#include "shared/translation_keys.h"

//==============================================================================
// From JSON Object - Static Constructor
//==============================================================================
TranslationItem* TranslationItem::fromJSON(const QJsonObject& aObject, PackageInfo* aPackage)
{
    // Init New Translation Item
    TranslationItem* newItem = nullptr;

    // Check JSON Object
    if (!aObject.isEmpty() && aPackage) {
        // Create New Translation Item
        newItem = new TranslationItem(aPackage, aObject.value(PACKAGE_KEY_TEXT_GROUP).toString());

        // Set Member Values
        newItem->mTextID = aObject.value(PACKAGE_KEY_TEXT_ID).toString();

        //qDebug() << "TranslationItem::fromJSON - mTextID: " << newItem->mTextID;

        newItem->mType = aObject.value(PACKAGE_KEY_TEXT_TYPE).toString();
        newItem->mScreen = aObject.value(PACKAGE_KEY_TEXT_SCREEN).toString();
        newItem->mOriginalText = aObject.value(PACKAGE_KEY_TEXT_ORIGINAL).toString();
        newItem->mComment = aObject.value(PACKAGE_KEY_TEXT_COMMENT).toString();

        // Load Layouts

        // Get Layouts Array
        QJsonArray layoutsArray = aObject.value(PACKAGE_KEY_TEXT_LAYOUTS).toArray();

        // Iterate Through Layouts
        for (QJsonValue layoutValue : layoutsArray) {
            // Append Layout
            newItem->appendLayout(TranslationItemLayout::fromJSON(layoutValue.toObject(), aPackage));
        }

        // Check Layout Count
        if (newItem->layoutCount() == 0) {
            // Append Default Layout
            newItem->addLayout();
        }

        // Check Package Info
        if (aPackage) {
            // Get Package Languages
            QStringList packageLanguages = aPackage->languages();
            // Iterate Through Package Languages
            for (QString pLanguage : packageLanguages) {
                // Init Translation
                newItem->mTranslations[pLanguage] = "";
            }
        }

        // Load Translations

        // Get Translations Array
        QJsonArray translationsArray = aObject.value(PACKAGE_KEY_TEXT_TRANSLATIONS).toArray();

        // Iterate Through Translations
        for (QJsonValue translationValue : translationsArray) {
            // Get JSON Object
            QJsonObject translation = translationValue.toObject();
            // Get Language
            QString language = translation.value(PACKAGE_KEY_TEXT_LANGUAGE).toString();
            // Get Translated Text
            QString translatedText = translation.value(PACKAGE_KEY_TEXT_TRANSLATED_TEXT).toString();

            // Check Language
            if (!language.isEmpty()) {
                // Set Translation
                newItem->setTranslation(language, translatedText);
            }
        }

        // Set State Last
        newItem->mState = aObject.value(PACKAGE_KEY_TEXT_STATE).toString();

        // Set Unfit Languages AFTER Reading Translations!
        newItem->mUnfitItemLanguages = aObject.value(PACKAGE_KEY_TEXT_OUT_OF_BOUNDS).toString().split(";");

        // ...
    }

    return newItem;
}

//==============================================================================
// Constructor
//==============================================================================
TranslationItem::TranslationItem(PackageInfo* aPackage, const QString& aGroup, QObject* aParent)
    : QObject(aParent)
    , mPackage(aPackage)
    , mTextID("")
    , mOriginalText("")
    , mGroup(aGroup)
    , mType(TEXT_TYPE_GUI)
    , mState(TEXT_STATE_DRAFT)
    , mComment("")
    , mLayoutIndex(0)
    , mMergedTranslations("")
    , mUnfitItemLanguages(QStringList())
    , mSelected(false)
{
    //qDebug() << "TranslationItem created.";

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void TranslationItem::init()
{
    // Set Ownership
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // ...
}

//==============================================================================
// ShutDown
//==============================================================================
void TranslationItem::shutDown()
{
    //qDebug() << "TranslationItem::shutDown";

    // Clear
    clear();

    // ...
}

//==============================================================================
// Clear
//==============================================================================
void TranslationItem::clear()
{
    //qDebug() << "TranslationItem::clear";

    // Iterate Through Layouts
    while (mLayouts.count() > 0) {
        // Delete Last Layout
        delete mLayouts.takeLast();
    }

    // Clear Translations
    mTranslations.clear();
}

//==============================================================================
// Set Package Dirty
//==============================================================================
void TranslationItem::setPackageDirty()
{
    // Check Package
    if (mPackage) {
        // Get Index
        int index = mPackage->itemIndex(this);

        // Refresh Translation Item Row
        mPackage->refresh(index);

        // Set Dirty
        mPackage->setDirty();
    }
}

//==============================================================================
// Append Layout
//==============================================================================
void TranslationItem::appendLayout(TranslationItemLayout* aLayout, const bool& aNewTranslation)
{
    // Check Layout
    if (aLayout) {
        //qDebug() << "TranslationItem::appendLayout";

        // Disconnect Layout Changed Signal
        connect(aLayout, SIGNAL(layoutChanged()), this, SLOT(layoutChanged()));

        // Append Layout
        mLayouts << aLayout;

        // ...

        // Emit Layouts Changed Signal
        emit layoutsChanged();

        // Set Current Layout
        //setLayoutIndex(mLayouts.count() - 1);

        // Check New Translation
        if (!aNewTranslation) {
            // Reset Text State
            setTextState(TEXT_STATE_DRAFT);

            // Set Package Dirty State
            setPackageDirty();
        }
    }
}

//==============================================================================
// Layout Changed Slot
//==============================================================================
void TranslationItem::layoutChanged()
{
    //qDebug() << "TranslationItem::layoutChanged";

    // Check Text State
    //if (mState != TEXT_STATE_WARNING)
    {
        // Reset Text State
        setTextState(TEXT_STATE_DRAFT);
    }

    // ...
}

//==============================================================================
// Get Translation
//==============================================================================
QString TranslationItem::getTranslation(const QString& aLanguage)
{
    // Check Language
    if (!aLanguage.isEmpty()) {
        return mTranslations.value(aLanguage);
    }

    return "";
}

//==============================================================================
// Get Merged Translations
//==============================================================================
QString TranslationItem::getMergedTranslations()
{
    return mMergedTranslations;
}

//==============================================================================
// Get Single Line Translation
//==============================================================================
QString TranslationItem::getSingleLineTranslation(const QString& aLanguage)
{
    // Check Language
    if (!aLanguage.isEmpty()) {
        // Get Translation Value
        QString result = mTranslations.value(aLanguage);

        return getSingleLineText(result);
    }

    return "";
}

//==============================================================================
// Get Single Line Text
//==============================================================================
QString TranslationItem::getSingleLineText(const QString& aText)
{
    QString result = aText;

    return result.replace("\n", "\\n");
}

//==============================================================================
// Add Translation
//==============================================================================
void TranslationItem::setTranslation(const QString& aLanguage, const QString& aTranslation)
{
    // Check Language
    if (!aLanguage.isEmpty() && mTranslations.value(aLanguage) != aTranslation) {
        //qDebug() << "TranslationItem::setTranslation - aLanguage: " << aLanguage << " - aTranslation: " << aTranslation;
        // Set Translation
        mTranslations[aLanguage] = aTranslation;

        // Set/Update Merged Translations
        mMergedTranslations = mTranslations.values().join(QChar(0x1E));

        // Emit Translation Changed Signal
        emit translationChanged(aLanguage, aTranslation);

        // Clear Unfit Language Flag
        unMarkItemLanguageUnfit(aLanguage);

        // Reset Text
        setTextState(TEXT_STATE_DRAFT);

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Remove Translation
//==============================================================================
void TranslationItem::removeTranslation(const QString& aLanguage)
{
    // Check Language
    if (mTranslations.keys().indexOf(aLanguage) != -1) {
        //qDebug() << "TranslationItem::removeTranslation - aLanguage: " << aLanguage;

        // Remove Translation
        mTranslations.remove(aLanguage);

        // Clear Unfit Language Flag
        unMarkItemLanguageUnfit(aLanguage);

        // Clear Translation
        //mTranslations[aLanguage] = "";

        // Reset Text
        setTextState(TEXT_STATE_DRAFT);

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Text ID
//==============================================================================
QString TranslationItem::textID()
{
    return mTextID;
}

//==============================================================================
// Set Text ID
//==============================================================================
void TranslationItem::setTextID(const QString& aTextID)
{
    // Check Text ID
    if (mTextID != aTextID) {
        //qDebug() << "TranslationItem::setTextID - aTextID: " << aTextID;
        // Set Text ID
        mTextID = aTextID;
        // Emit Text ID Changed Signal
        emit textIDChanged(mTextID);

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Text Type
//==============================================================================
QString TranslationItem::textType()
{
    return mType;
}

//==============================================================================
// Set Text TYpe
//==============================================================================
void TranslationItem::setTextType(const QString& aTextType)
{
    // Check Text Type
    if (mType != aTextType) {
        //qDebug() << "TranslationItem::setTextType - aTextType: " << aTextType;
        // Set Text Type
        mType = aTextType;
        // Emit Text Type Changed Signal
        emit textTypeChanged(mType);

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Screen
//==============================================================================
QString TranslationItem::screen()
{
    return mScreen;
}

//==============================================================================
// Set Screen
//==============================================================================
void TranslationItem::setScreen(const QString& aScreen)
{
    // Check Screen
    if (mScreen != aScreen) {
        // Set Screen
        mScreen = aScreen;
        // Emit Screen Changed Signal
        emit screenChanged(mScreen);

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Text State
//==============================================================================
QString TranslationItem::textState()
{
    return mState;
}

//==============================================================================
// Set Text State
//==============================================================================
void TranslationItem::setTextState(const QString& aTextState)
{
    // Check State
    if (mState != aTextState) {
        qDebug() << "TranslationItem::setTextState - aTextState: " << aTextState;
        // Set State
        mState = aTextState;
        // Emit Text State Changed Signal
        emit textStateChanged(mState);

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Text Translation Group Name
//==============================================================================
QString TranslationItem::translationGroup()
{
    return mGroup;
}

//==============================================================================
// Set Translation Group
//==============================================================================
void TranslationItem::setTranslationGroup(const QString& aGroupName)
{
    // Check Translation Group
    if (mGroup != aGroupName) {
        // Set Translation Group
        mGroup = aGroupName;

        // ...

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Original Text
//==============================================================================
QString TranslationItem::originalText()
{
    return mOriginalText;
}

//==============================================================================
// Set Original Text
//==============================================================================
void TranslationItem::setOriginalText(const QString& aOriginalText)
{
    // Check Original Text
    if (mOriginalText != aOriginalText) {
        //qDebug() << "TranslationItem::setOriginalText - aOriginalText: " << aOriginalText;
        // Set Original Text
        mOriginalText = aOriginalText;
        // Emit Original Text Changed Signal
        emit originalTextChanged(mOriginalText);

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Comment
//==============================================================================
QString TranslationItem::comment()
{
    return mComment;
}

//==============================================================================
// Set Comment
//==============================================================================
void TranslationItem::setComment(const QString& aComment)
{
    // Check Comment
    if (mComment != aComment) {
        // Set Comment
        mComment = aComment;
        // Emit Comment Changed Signal
        emit commentChanged(mComment);

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Get Available Languages
//==============================================================================
QStringList TranslationItem::languages()
{
    return mTranslations.keys();
}

//==============================================================================
// Get Available Translations
//==============================================================================
QStringList TranslationItem::translations()
{
    return mTranslations.values();
}

//==============================================================================
// Get Number Of Layouts
//==============================================================================
int TranslationItem::layoutCount()
{
    return mLayouts.count();
}

//==============================================================================
// Get Current Layout Index
//==============================================================================
int TranslationItem::layoutIndex()
{
    return mLayoutIndex;
}

//==============================================================================
// Set Current Layout Index
//==============================================================================
void TranslationItem::setLayoutIndex(const int& aIndex)
{
    // Check Index
    if (mLayoutIndex != aIndex && aIndex >= 0 && aIndex < mLayouts.count()) {
        //qDebug() << "TranslationItem::setLayoutIndex - aIndex: " << aIndex;
        // Set Current Layout Index
        mLayoutIndex = aIndex;
        // Emit Current Layout Index Changed Signal
        emit layoutIndexChanged(mLayoutIndex);
        // Emit Current Layout Changed Signal
        emit currentLayoutChanged(currentLayout());
    }
}

//==============================================================================
// Get Current Layout
//==============================================================================
TranslationItemLayout* TranslationItem::currentLayout()
{
    return mLayouts[mLayoutIndex];
}

//==============================================================================
// Add Layout
//==============================================================================
void TranslationItem::addLayout(const bool& aNewTranslation)
{
    // Append Layout
    appendLayout(new TranslationItemLayout(mPackage), aNewTranslation);
}

//==============================================================================
// Remove Layout
//==============================================================================
void TranslationItem::removeLayout(const int& aIndex)
{
    // Check Index
    if (aIndex > 0 && aIndex < mLayouts.count()) {
        //qDebug() << "TranslationItem::removeLayout - aIndex: " << aIndex;

        // Get Layout
        TranslationItemLayout* layout = mLayouts.takeAt(aIndex);

        // Disconnect Layout Changed Signal
        disconnect(layout, SIGNAL(layoutChanged()), this, SLOT(layoutChanged()));

        // Remove Layout
        delete layout;
        // Emit Layouts Cahnged Signal
        emit layoutsChanged();

        // Check Layouts
        if (mLayouts.count() == 1) {
            // Reset Layout Index
            setLayoutIndex(0);
        } else {
            // Set Layout Index
            setLayoutIndex(qBound(0, mLayoutIndex, mLayouts.count() - 1));
        }

        // Set Package Dirty State
        setPackageDirty();
    }
}

//==============================================================================
// Previous Layout
//==============================================================================
void TranslationItem::prevLayout()
{
    // Check Current Layout Index
    if (mLayoutIndex > 0) {
        //qDebug() << "TranslationItem::prevLayout";
        // Set Current Layout Index
        setLayoutIndex(mLayoutIndex - 1);
    }
}

//==============================================================================
// Next Layout
//==============================================================================
void TranslationItem::nextLayout()
{
    // Check Current Layout Index
    if (mLayoutIndex < mLayouts.count() - 1) {
        //qDebug() << "TranslationItem::nextLayout";
        // Set Current Layout Index
        setLayoutIndex(mLayoutIndex + 1);
    }
}

//==============================================================================
// Get Selected State
//==============================================================================
bool TranslationItem::selected()
{
    return mSelected;
}

//==============================================================================
// Set Selected State
//==============================================================================
void TranslationItem::setSelected(const bool& aSelected)
{
    // Check Selected State
    if (mSelected != aSelected) {
        // Set Selected State
        mSelected = aSelected;
        // Emit Selected State Changed Signal
        emit selectedChanged(mSelected);
    }
}

//==============================================================================
// Unfit Translation Item Languages
//==============================================================================
QStringList TranslationItem::unfitLanguages()
{
    return mUnfitItemLanguages;
}

//==============================================================================
// Get Text Data by Column
//==============================================================================
QString TranslationItem::getTextData(const int& aColumn)
{
    // Switch Column
    switch (aColumn) {
        case 0:     return mTextID;
        case 1:     return mScreen;
        case 2:     return mOriginalText;

        default:
            // Check Column
            if (aColumn > 2 && (aColumn - 3) < mPackage->languages().count()) {
                return getTranslation(mPackage->languages()[aColumn - 3]);
            }
        break;
    }

    return "";
}

//==============================================================================
// Set Text Data by Column
//==============================================================================
void TranslationItem::setTextData(const int& aColumn, const QString& aText)
{
    qDebug() << "TranslationItem::setTextData - aColumn: " << aColumn << " - aText: " << aText;

    // Switch Column
    switch (aColumn) {
        case 0:
            // Check Text
            if (!aText.isEmpty()) {
                setTextID(aText);
            }
        break;

        case 1:
            // Set Screen
            setScreen(aText);
        break;

        case 2:
            // Check Text
            if (!aText.isEmpty()) {
                // Set Original Text
                setOriginalText(aText);
            }
        break;

        default:
            // Check Column
            if (aColumn > 2 && (aColumn - 3) < mPackage->languages().count() /*&& !aText.isEmpty()*/) {
                setTranslation(mPackage->languages()[aColumn - 3], aText);
            }
        break;
    }
}

//==============================================================================
// Mark Translation Item Language Unfit/Out Of Bounds
//==============================================================================
void TranslationItem::markItemLanguageUnfit(const QString& aLanguage)
{
    // Get Language Index
    int lIndex = mUnfitItemLanguages.indexOf(aLanguage);
    // Check Index
    if (lIndex == -1) {
        qDebug() << "TranslationItem::markItemLanguageUnfit - aLanguage: " << aLanguage;
        // Append Language
        mUnfitItemLanguages << aLanguage;

        // Emit Unfit Translation Item Languages Changed Signal
        emit unfitLanguagesChanged(mUnfitItemLanguages);

        // Set Text State
        setTextState(TEXT_STATE_WARNING);

//        // Set Package Dirty State
//        setPackageDirty();
    }
}

//==============================================================================
// UnMark Translation Item Language Unfit/Out Of Bounds
//==============================================================================
void TranslationItem::unMarkItemLanguageUnfit(const QString& aLanguage)
{
    // Get Language Index
    int lIndex = mUnfitItemLanguages.indexOf(aLanguage);
    // Check Index
    if (lIndex != -1) {
        qDebug() << "TranslationItem::unMarkItemLanguageUnfit - aLanguage: " << aLanguage;
        // Remove Language
        mUnfitItemLanguages.removeAt(lIndex);

        // Emit Unfit Translation Item Languages Changed Signal
        emit unfitLanguagesChanged(mUnfitItemLanguages);

//        // Set Text State
//        setTextState(TEXT_STATE_DRAFT);

//        // Set Package Dirty State
//        setPackageDirty();
    }
}

//==============================================================================
// Clear Unfit Languages
//==============================================================================
void TranslationItem::clearUnfitLanguages()
{
    qDebug() << "TranslationItem::clearUnfitLanguages";
    // Clear Unfit Languages
    mUnfitItemLanguages.clear();

    // ...

    // Emit Unfit Translation Item Languages Changed Signal
    emit unfitLanguagesChanged(mUnfitItemLanguages);
}

//==============================================================================
// Export Translation Item
//==============================================================================
QJsonObject TranslationItem::exportToJSON()
{
    //qDebug() << "TranslationItem::exportToJSON - mTextID: " << mTextID;

    // Init New JSON Object
    QJsonObject newObject;

    // Set Up Properties - Text ID
    newObject[PACKAGE_KEY_TEXT_ID]          = mTextID;
    // Set Up Properties - Group
    newObject[PACKAGE_KEY_TEXT_GROUP]       = mGroup;
    // Set Up Properties - Type
    newObject[PACKAGE_KEY_TEXT_TYPE]        = mType;
    // Check Screen
    if (!mScreen.isEmpty()) {
        // Set Screen
        newObject[PACKAGE_KEY_TEXT_SCREEN]      = mScreen;
    }
    // Set Up Properties - State
    newObject[PACKAGE_KEY_TEXT_STATE]       = mState;
    // Set Up Properties - Original Text
    newObject[PACKAGE_KEY_TEXT_ORIGINAL]    = mOriginalText;
    // Check Comment
    if (!mComment.isEmpty()) {
        // Set Up Properties - Comment
        newObject[PACKAGE_KEY_TEXT_COMMENT]     = mComment;
    }

    // Init Layouts Array
    QJsonArray layoutsArray;

    // Iterate Throguh Layouts
    for (TranslationItemLayout* layout : mLayouts) {
        // Add Layout
        layoutsArray << layout->exportToJSON();
    }

    // ...

    // Check Layouts Array

    // ...

    // Set Layouts Array Property
    newObject[PACKAGE_KEY_TEXT_LAYOUTS] = layoutsArray;

    // Init Translations Array
    QJsonArray translationsArray;

    // Iterate Through Translations
    for (QString language : mTranslations.keys()) {
        // Get Translation Text
        QString translatedText = mTranslations.value(language);

        // Check Translation Text
        if (!translatedText.trimmed().isEmpty()) {
            // Init New Translation Object
            QJsonObject newTranslationTextObject;

            // Set Properties - Language
            newTranslationTextObject[PACKAGE_KEY_TEXT_LANGUAGE] = language;
            // Set Properties - Translated Text
            newTranslationTextObject[PACKAGE_KEY_TEXT_TRANSLATED_TEXT] = translatedText;

            // Add Translated Text Object
            translationsArray << newTranslationTextObject;
        }
    }

    // ...

    // Set Translations Property
    newObject[PACKAGE_KEY_TEXT_TRANSLATIONS] = translationsArray;

    // Check Unfit/Out Of Bounds Items Language
    if (mUnfitItemLanguages.count() > 0) {
        // Set Unfit/Out Of Bounds Languages
        newObject[PACKAGE_KEY_TEXT_OUT_OF_BOUNDS] = mUnfitItemLanguages.join(";");
    }

    // ...

    return newObject;
}

//==============================================================================
// Destructor
//==============================================================================
TranslationItem::~TranslationItem()
{
    // ...

    // Shut Down
    shutDown();

    // ...

    //qDebug() << "TranslationItem deleted.";
}
