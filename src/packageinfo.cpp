#include <QDebug>
#include <QQmlEngine>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "packageinfo.h"
#include "packagegroup.h"
#include "translationitem.h"
#include "translationitemlayout.h"
#include "shared/translation_keys.h"
#include "constants.h"

//==============================================================================
// From JSON Object
//==============================================================================
PackageInfo* PackageInfo::fromJSON(const QJsonObject& aObject)
{
    // Check JSON Object
    if (!aObject.isEmpty()) {
        // Create New Package Info
        PackageInfo* newPackageInfo = new PackageInfo();

        // Import From JSON Object
        newPackageInfo->importFromJSON(aObject);

        return newPackageInfo;
    }

    return nullptr;
}

//==============================================================================
// Constructor
//==============================================================================
PackageInfo::PackageInfo(QObject* aParent)
    : QAbstractListModel(aParent)
    , mFilePath("")
    , mName("")
    , mFontsDir("")
    , mDefaultFont("")
    , mDefaultFontSize(DEFAULT_TEXT_FONT_SIZE)
    , mLanguages()
    , mSelectedLanguages()
    , mAllLanguages(false)
    , mCurrentGroup(nullptr)
    , mFilterText("")
    , mDirty(false)
    , mSelectedItemCount(0)
    , mSingleSelectionIndex(-1)
{
    qDebug() << "PackageInfo created.";

    // Init
    init();

    // ...
}

//==============================================================================
// Init
//==============================================================================
void PackageInfo::init()
{
    qDebug() << "PackageInfo::init";

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // ...
}

//==============================================================================
// Shut Down
//==============================================================================
void PackageInfo::shutDown()
{
    qDebug() << "PackageInfo::shutDown";

    // Clear
    clear();
}

//==============================================================================
// Clear
//==============================================================================
void PackageInfo::clear()
{
    qDebug() << "PackageInfo::clear";

    // Get Groups
    QStringList groups = mGroups.keys();

    // Iterate Through Groups
    for (QString groupName : groups) {
        // Delete Group
        delete mGroups.take(groupName);
    }
}

//==============================================================================
// Set Dirty State
//==============================================================================
void PackageInfo::setDirty(const bool& aDirty)
{
    // Check Dirty State
    if (mDirty != aDirty) {
        qDebug() << "PackageInfo::setDirty - aDirty: " << aDirty;

        // Set Dirty State
        mDirty = aDirty;
        // Emit Dirty State Changed Signal
        emit dirtyChanged(mDirty);
    }
}

//==============================================================================
// Import From JSON Object
//==============================================================================
void PackageInfo::importFromJSON(const QJsonObject& aObject)
{
    // Check JSON Object
    if (!aObject.isEmpty()) {
        qDebug() << "PackageInfo::importFromJSON - mFilePath: " << mFilePath;

        // Set Package Name
        setPackageName(aObject.value(PACKAGE_KEY_PACKAGE_NAME).toString());

        // Init Package File Info
        QFileInfo packageInfo(mFilePath);
        // Init Package File Name Absolute Path
        QString absolutePath = packageInfo.absolutePath();
        // Check Last Char
        if (absolutePath.right(1) != "/") absolutePath += "/";
//        // Set Fonts Dir
//        setFontsDir(aObject.value(PACKAGE_KEY_FONTS_DIR).toString());
        // Set Fonts Dir
        setFontsDir(QDir::cleanPath(absolutePath + aObject.value(PACKAGE_KEY_FONTS_DIR).toString()));
        // Set Default Font
        setDefaultFont(aObject.value(PACKAGE_KEY_DEFAULT_FONT_NAME).toString());
        // Set Default Font Size
        setDefaultFontSize(aObject.value(PACKAGE_KEY_DEFAULT_FONT_SIZE).toInt(DEFAULT_TEXT_FONT_SIZE));

        // Clear Languages
        mLanguages.clear();
        // Set Languages
        mLanguages = aObject.value(PACKAGE_KEY_LANGUAGES).toString().split(";");
        // Sort Languages
        mLanguages.sort();
        // Emit Languages Changed
        emit languagesChanged(mLanguages);

        qDebug() << "PackageInfo::importFromJSON - mLanguages: " << mLanguages;

        // Get Groups Array
        QJsonArray groupsArray = aObject.value(PACKAGE_KEY_GROUPS).toArray();

        // Get Group Count
        int gCount = groupsArray.count();

        qDebug() << "PackageInfo::importFromJSON - gCount: " << gCount;

        // Iterate Through Groups
        for (int i=0; i<gCount; i++) {
            // Get Group Object
            QJsonObject group = groupsArray[i].toObject();
            // Get Group Name
            QString groupName = group.value(PACKAGE_KEY_GROUP_NAME).toString();

            qDebug() << "PackageInfo::importFromJSON - groupName: " << groupName;

            // Create New group
            PackageGroup* newGroup = new PackageGroup(groupName, this);
            // Get Items
            QJsonArray items = group.value(PACKAGE_KEY_TRANSLATION_ITEMS).toArray();

            // Get Items Count
            int iCount = items.count();

            qDebug() << "PackageInfo::importFromJSON - iCount: " << iCount;

            // Load Items
            for (int j=0; j<iCount; j++) {
                // Gt Translation Item Object
                QJsonObject item = items[j].toObject();
                // Create New Translation Item
                TranslationItem* newItem = TranslationItem::fromJSON(item, this);

                // Check New Item
                if (newItem) {
                    //qDebug() << "PackageInfo::importFromJSON - newItem: " << newItem->textID();
                    // Add New Translation Item
                    newGroup->addItem(newItem);
                }
            }

            // Append Group
            mGroups[groupName] = newGroup;

            // Emit Groups Changed Signal
            emit groupsChanged(mGroups.keys());
        }
    }
}

//==============================================================================
// Accept Row
//==============================================================================
bool PackageInfo::acceptRow(const int& aIndex, const QString& aFilterText) const
{
    return acceptItem(getItem(aIndex), aFilterText);
}

//==============================================================================
// Accept Item
//==============================================================================
bool PackageInfo::acceptItem(TranslationItem* aItem, const QString& aFilterText) const
{
    // Check Item
    if (!aItem) {
        return false;
    }

    // Chekc Filter Text
    if (aFilterText.isEmpty()) {
        return true;
    }

    // Check Text ID
    if (aItem->textID().indexOf(aFilterText, 0, Qt::CaseInsensitive) != -1) {
        return true;
    }

    // Check Original Text
    if (aItem->originalText().indexOf(aFilterText, 0, Qt::CaseInsensitive) != -1) {
        return true;
    }

    // Get Languages Count
    int lCount = aItem->languages().count();

    // Iterate Through Translations
    for (int i=0; i<lCount; i++) {
        // Get Translation
        QString translation = aItem->getTranslation(aItem->languages()[i]);
        // Check Translation
        if (translation.indexOf(aFilterText, 0, Qt::CaseInsensitive) != -1) {
            return true;
        }
    }

    return false;
}

//==============================================================================
// Export To JSON Object
//==============================================================================
QJsonObject PackageInfo::exportToJSON()
{
    qDebug() << "PackageInfo::exportToJSON - mFilePath: " << mFilePath;

    // Init New JSON Object
    QJsonObject newObject;

    // Set Package Properties
    newObject[PACKAGE_KEY_PACKAGE_NAME]         = mName;

    // Init Package File Info
    QFileInfo packageInfo(mFilePath);
    // Init Package Dir
    QDir packageDir(packageInfo.absolutePath());

    //qDebug() << "#### PackageInfo::exportToJSON - relative Path: " << packageDir.relativeFilePath(mFontsDir);

    //newObject[PACKAGE_KEY_FONTS_DIR]            = mFontsDir;
    newObject[PACKAGE_KEY_FONTS_DIR]            = packageDir.relativeFilePath(mFontsDir);
    newObject[PACKAGE_KEY_DEFAULT_FONT_NAME]    = mDefaultFont;
    newObject[PACKAGE_KEY_DEFAULT_FONT_SIZE]    = mDefaultFontSize;
    newObject[PACKAGE_KEY_LANGUAGES]            = mLanguages.join(';');

    // Init Groups Array
    QJsonArray  groupsArray;

    // Iterate Through Groups
    for (PackageGroup* group : mGroups) {
        // Init Pacakge Group Object
        QJsonObject groupObject;

        // Set Properties
        groupObject[PACKAGE_KEY_GROUP_NAME] = group->groupName();

        // Init Items Array
        QJsonArray itemsArray;

        // Iterate Through Translation Items
        for (TranslationItem* item : group->mItems) {
            // Append Item To Items Array
            itemsArray << item->exportToJSON();
        }

        groupObject[PACKAGE_KEY_TRANSLATION_ITEMS] = itemsArray;

        // Append Group To Groups Array
        groupsArray << groupObject;
    }

    // Set Groups
    newObject[PACKAGE_KEY_GROUPS] = groupsArray;

    return newObject;
}

//==============================================================================
// Row Count
//==============================================================================
int PackageInfo::rowCount(const QModelIndex& ) const
{
    // Init Row Count
    int rCount = 0;

    // Iterate Throuh Groups
    for (QString groupName : mGroups.keys()) {
        // Add Group
        rCount += mGroups[groupName]->rowCount();
    }

    return rCount;
}

//==============================================================================
// Data
//==============================================================================
QVariant PackageInfo::data(const QModelIndex& index, int role) const
{
    // Get Row Index
    int rowIndex = index.row();
    // Check Row Index
    if (rowIndex >= 0 && rowIndex < rowCount()) {
        // Get Translation Item
        TranslationItem* trItem = getItem(rowIndex);
//        // Get Languages Count
//        int lCount = mLanguages.count();

        // Switch Role
        switch (role) {
            case EPIRSourceIndex:   return rowIndex;
            case EPIRTextID:        return trItem->textID();
            case EPIRScreen:        return trItem->screen();
            case EPIRType:          return trItem->textType();
            case EPIRState:         return trItem->textState();
            case EPIROriginal:      return trItem->originalText();
            case EPIRGroup:         return trItem->translationGroup();
            case EPIRComment:       return trItem->comment();
            case EPIRSelected:      return trItem->selected();
            case EPIRLanguages:     return trItem->languages();
            case EPIRMergedTrs:     return trItem->getMergedTranslations();

//            case EPIRMergedTrs: {
//                // Init Merged Translations
//                QStringList mergedTranslations;

//                // Iterate Through Languages
//                for (QString language : mLanguages) {
//                    // Append Translation
//                    mergedTranslations << trItem->getTranslation(language);
//                }

//                return mergedTranslations.join(QChar(0x1E));
//            }

//            default: {
//                // Check Role
//                if (role >= EPIRTranslations && role < EPIRTranslations + lCount) {
//                    // Get Language Index
//                    int lIndex = role - EPIRTranslations;
//                    // Get Language
//                    QString language = mLanguages[lIndex];
//                    // Return Translation By Language
//                    return trItem->getTranslation(language);
//                }
//            } break;
        }
    }

    return QVariant();
}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> PackageInfo::roleNames() const
{
    // Init Role Names
    QHash<int, QByteArray> rNames;

    // Set Up Role Names
    rNames[EPIRSourceIndex]     = "triSourceIndex";
    rNames[EPIRTextID]          = "triTextID";
    rNames[EPIRScreen]          = "triScreen";
    rNames[EPIRType]            = "triType";
    rNames[EPIRState]           = "triState";
    rNames[EPIROriginal]        = "triOriginal";
    rNames[EPIRGroup]           = "triGroup";
    rNames[EPIRComment]         = "triComment";
    rNames[EPIRSelected]        = "triSelected";
    rNames[EPIRLanguages]       = "triLanguages";
    rNames[EPIRTranslations]    = "triTranslations";
    rNames[EPIRMergedTrs]       = "triMerged";

//    // Get Languages Count
//    int lCount = mLanguages.count();

//    // Iterate Through Languages
//    for (int i=0; i<lCount; i++) {
//        // Set Roles For Translations
//        rNames[EPIRTranslations + i] = QString("triTranslation%1").arg(i + 1, 3, QChar('0')).toLocal8Bit();
//    }

    // ...

    return rNames;
}

//==============================================================================
// Get Path
//==============================================================================
QString PackageInfo::path()
{
    return mFilePath;
}

//==============================================================================
// Load Package
//==============================================================================
void PackageInfo::loadPackage(const QString& aPackagePath)
{
    // Check Package Path
    if (!aPackagePath.isEmpty()) {
        qDebug() << "PackageInfo::loadPackage - aPackagePath: " << aPackagePath;

        // Init Package File
        QFile packageFile(aPackagePath);

        // Open Package File
        if (packageFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // Init JSON Content
            QString jsonContent = packageFile.readAll();
            // Close Package File
            packageFile.close();
            // Init JSON Document
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonContent.toUtf8());

            // Set Path
            mFilePath = aPackagePath;

            // Begin Reset Model
            beginResetModel();

            // Import From JSON
            importFromJSON(jsonDoc.object());

            // End Reset Model
            endResetModel();

            // Emit Count Changed Signal
            emit countChanged(count());

            // ...

            // Reset Dirty State
            setDirty(false);

        } else {
            qWarning() << "PackageInfo::loadPackage - CAN NOT OPEN FILE! - aPackagePath: " << aPackagePath;
        }

    } else {
        qWarning() << "PackageInfo::loadPackage IS EMPTY!";
    }
}

//==============================================================================
// Save Package
//==============================================================================
void PackageInfo::savePackage(const QString& aPackagePath)
{
    // Init Package Path
    QString packagePath = aPackagePath.isEmpty() ? mFilePath : aPackagePath;

    // Check Package Path
    if (!packagePath.isEmpty() && mDirty) {
        qDebug() << "PackageInfo::savePackage - packagePath: " << packagePath;

        // Set Package Path
        mFilePath = packagePath;

        // Init JSON File
        QFile jsonFile(packagePath);

        // Open File For Reading
        if (jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // Init Text Stream
            QTextStream textStream(&jsonFile);
            // Export To JSON Object
            QJsonObject mPackageObject = exportToJSON();
            // Init JSON Document
            QJsonDocument jsonDocument(mPackageObject);
            // Write Document To Text Stream
            textStream << jsonDocument.toJson();
            // Flush
            textStream.flush();
            // Close File
            jsonFile.close();

            // Reset Dirty State
            setDirty(false);
        }

        // ...
    }
}

//==============================================================================
// Add Group
//==============================================================================
void PackageInfo::addGroup(const QString& aGroupName)
{
    // Check Group Name
    if (!aGroupName.isEmpty() && mGroups.keys().indexOf(aGroupName) == -1) {
        qDebug() << "PackageInfo::addGroup - aGroupName: " << aGroupName;

        // Create New Group
        PackageGroup* newGroup = new PackageGroup(aGroupName, this);

        // Add Group
        mGroups[aGroupName] = newGroup;

        // Emit Groups Changed Signal
        emit groupsChanged(mGroups.keys());

        // ...

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Remove Group
//==============================================================================
void PackageInfo::removeGroup(const QString& aGroupName)
{
    // Check Group Name
    if (mGroups.keys().indexOf(aGroupName) != -1) {
        qDebug() << "PackageInfo::removeGroup - aGroupName: " << aGroupName;

        // Take Group
        PackageGroup* group = mGroups.take(aGroupName);

        // Delete Group
        delete group;

        // Emit Groups Changed Signal
        emit groupsChanged(mGroups.keys());

        // ...

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Select Group
//==============================================================================
void PackageInfo::selectGroup(const QString& aGroupName)
{
    // Check Group Name
    if (aGroupName.isEmpty()) {
        // Reset Current Group
        mCurrentGroup = nullptr;

    } else if (mGroups.keys().indexOf(aGroupName) != -1) {
        qDebug() << "PackageInfo::selectGroup - aGroupName: " << aGroupName;

        // Set Current Group
        mCurrentGroup = mGroups[aGroupName];

        // ...
    }

    // Emit Current Group Changed Signal
    emit currentGroupChanged(mCurrentGroup);

    // ...
}

//==============================================================================
// Add Translation Item
//==============================================================================
bool PackageInfo::addTranslationItem(const QString& aGroupName, TranslationItem* aTranslationItem)
{
    // Check Translation Item
    if (!aTranslationItem) {
        return false;
    }

    // Get Group
    PackageGroup* group = mGroups.value(aGroupName);

    // Check Group
    if (group) {
        // Get Group Name Index
        int gnIndex = mGroups.keys().indexOf(aGroupName);
        // Init Insert Index
        int iIndex = 0;

        // Iterate Groups
        for (int i=0; i<=gnIndex; i++) {
            // Get Group
            PackageGroup* group = mGroups[mGroups.keys()[i]];
            // Inc Index
            iIndex += group->rowCount();
        }

        qDebug() << "PackageInfo::addTranslationItem - groups: " << mGroups.keys();
        qDebug() << "PackageInfo::addTranslationItem - aGroupName: " << aGroupName << " - gnIndex: " << gnIndex << " - iIndex: " << iIndex;

        // Begin Insert Rows
        beginInsertRows(QModelIndex(), iIndex, iIndex);
        // Add Translation Item
        group->addItem(aTranslationItem);
        // End Insert Rows
        endInsertRows();

        // Emit Count Changed Signal
        emit countChanged(count());

        // Set Dirty State
        setDirty();

        return true;
    }

    return false;
}

//==============================================================================
// Remove Translation Item
//==============================================================================
bool PackageInfo::removeTranslationItem(const QString& aGroupName, const int& aIndex)
{
    // Get Group
    PackageGroup* group = mGroups[aGroupName];
    // Check Group
    if (group) {
        // Get Group Name Index
        int gnIndex = mGroups.keys().indexOf(aGroupName);
        // Init Remove Index
        int rIndex = 0;

        // Iterate Groups
        for (int i=0; i<=gnIndex; i++) {
            // Get Group
            PackageGroup* group = mGroups[mGroups.keys()[i]];
            // Inc Index
            rIndex += group->rowCount();
        }

        // Begin Remove Rows
        beginRemoveRows(QModelIndex(), rIndex, rIndex);

        // Remove Item
        TranslationItem* itemToRemove = group->removeItem(aIndex, false);

        // End Remove Rows
        endRemoveRows();

        // Emit Count Changed Signal
        emit countChanged(count());

        // Check Item
        if (itemToRemove) {
            // Delete Item
            delete itemToRemove;

            // Set Dirty State
            setDirty();

            return true;
        }
    }

    return false;
}

//==============================================================================
// Remove Translation Item By Index
//==============================================================================
void PackageInfo::removeTranslationItem(const int& aIndex)
{
    qDebug() << "PackageInfo::removeTranslationItem - aIndex: " << aIndex;

    // Init Removal Index
    int removalIndex = aIndex;

    // Check Index
    if (removalIndex == -1 && mSelectedItemCount == 0) {
        // Set Removal Index
        removalIndex = mSingleSelectionIndex;
    }

    // Check Index
    if (removalIndex >= 0 && removalIndex < rowCount()) {
        // Init First Index Offset
        int fIndex = 0;
        // Iterate Through Groups
        for (QString groupName : mGroups.keys()) {
            // Get Group
            PackageGroup* group = mGroups[groupName];

            // Check Index
            if (removalIndex >= fIndex && removalIndex < (group->rowCount() + fIndex)) {
                // Emit Item About To Be Removed Signal
                emit itemAboutToBeRemoved(removalIndex);

                // Begin Remove Rows
                beginRemoveRows(QModelIndex(), removalIndex, removalIndex);
                // Remove Item From Group
                group->removeItem(removalIndex - fIndex);
                // End Remove Rows
                endRemoveRows();

                // Emit Item Removed Signal
                emit itemRemoved(removalIndex);
                // Emit Count Changed Signal
                emit countChanged(count());

                // Set Dirty State
                setDirty();

                return;
            }

            // Inc First Index Offset
            fIndex += group->rowCount();
        }
    } else if (mSelectedItemCount > 0) {

        // Emit Item About To Be Removed Signal
        emit itemAboutToBeRemoved(-1);

        // Remove Selected Items

        // Begin Reset Model
        beginResetModel();

        // Iterate Through Groups
        for (QString groupName : mGroups.keys()) {
            // Get Group
            PackageGroup* group = mGroups[groupName];
            // Get Group Count
            int gCount = group ? group->rowCount() : 0;

            // Iterate Through Group
            for (int i=gCount-1; i>=0; i--) {
                TranslationItem* item = group->mItems[i];
                // Check If Selected
                if (item->selected()) {
                    // Remove Item
                    group->removeItem(i);
                }
            }
        }

        // End Reset Model
        endResetModel();

        // Emit Item Removed Signal
        emit itemRemoved(-1);
        // Emit Count Changed Signal
        emit countChanged(count());

        // Set Dirty State
        setDirty();

        // Reset Selected Items Count
        mSelectedItemCount = 0;
        // Emit Selected Items Count Changed Signal
        emit selectedItemCountChanged(mSelectedItemCount);
    }
}

//==============================================================================
// Get Package Name
//==============================================================================
QString PackageInfo::packageName()
{
    return mName;
}

//==============================================================================
// Set Package Name
//==============================================================================
void PackageInfo::setPackageName(const QString& aName)
{
    // Check Package Name
    if (mName != aName) {
        // Set Package Name
        mName = aName;
        // Emit Package Name Changed Signal
        emit packageNameChanged(mName);

        // ...

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Fonts Dir
//==============================================================================
QString PackageInfo::fontsDir()
{
    return mFontsDir;
}

//==============================================================================
// Set Fonts Dir
//==============================================================================
void PackageInfo::setFontsDir(const QString& aFontsDir)
{
    // Check Fonts Dir
    if (mFontsDir != aFontsDir) {
        qDebug() << "PackageInfo::setFontsDir - aFontsDir: " << aFontsDir;
        // Set Fonts Dir
        mFontsDir = aFontsDir;
        // Emit Fonts Dir Changed Signal
        emit fontsDirChanged(mFontsDir);

        // ...

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Default Font Name
//==============================================================================
QString PackageInfo::defaultFont()
{
    return mDefaultFont;
}

//==============================================================================
// Set Default Font Name
//==============================================================================
void PackageInfo::setDefaultFont(const QString& aFontName)
{
    // Check Default Font
    if (mDefaultFont != aFontName) {
        qDebug() << "PackageInfo::setDefaultFont - aFontName: " << aFontName;
        // Set Default Font
        mDefaultFont = aFontName;
        // Emit Default Font Changed Signal
        emit defaultFontChanged(mDefaultFont);

        // ...

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Default Font Size
//==============================================================================
int PackageInfo::defaultFontSize()
{
    return mDefaultFontSize;
}

//==============================================================================
// Set Default Font Size
//==============================================================================
void PackageInfo::setDefaultFontSize(const int& aFontSize)
{
    // Check Default Font Size
    if (mDefaultFontSize != aFontSize) {
        qDebug() << "PackageInfo::setDefaultFontSize - aFontSize: " << aFontSize;
        // Set Default Font Size
        mDefaultFontSize = aFontSize;
        // Emit Default Font Size Changed Signal
        emit defaultFontSizeChanged(mDefaultFontSize);

        // ...

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Available Languages
//==============================================================================
QStringList PackageInfo::languages()
{
    return mLanguages;
}

//==============================================================================
// Get Groups
//==============================================================================
QStringList PackageInfo::groups()
{
    return mGroups.keys();
}

//==============================================================================
// Get Selected Groups
//==============================================================================
QStringList PackageInfo::selectedGroups()
{
    return mSelectedGroups;
}

//==============================================================================
// Get Selected Languages
//==============================================================================
QStringList PackageInfo::selectedLanguages()
{
    return mSelectedLanguages;
}

//==============================================================================
// All Languages Selected
//==============================================================================
bool PackageInfo::allLanguages()
{
    return mAllLanguages;
}

//==============================================================================
// Get Current Group
//==============================================================================
PackageGroup* PackageInfo::currentGroup()
{
    return mCurrentGroup;
}

//==============================================================================
// Get Filter Text
//==============================================================================
QString PackageInfo::filterText()
{
    return mFilterText;
}

//==============================================================================
// Set Filter Text
//==============================================================================
void PackageInfo::setFilterText(const QString& aFilterText)
{
    // Check Filter Text
    if (mFilterText != aFilterText) {
        // Set Filter Text
        mFilterText = aFilterText;
        // Emit Filter Text Changed Signal
        emit filterTextChanged(mFilterText);
    }
}

//==============================================================================
// Get Dirty State
//==============================================================================
bool PackageInfo::isDirty()
{
    return mDirty;
}

//==============================================================================
// Get Selected Item Count
//==============================================================================
int PackageInfo::selectedItemCount()
{
    return mSelectedItemCount;
}

//==============================================================================
// Get Single (Popup) Selection Index
//==============================================================================
int PackageInfo::singleSelectionIndex()
{
    return mSingleSelectionIndex;
}

//==============================================================================
// Set Single (Popup) Selection Index
//==============================================================================
void PackageInfo::setSingleSelectionIndex(const int& aSelectionIndex)
{
    // Check Single Selection Index
    if (mSingleSelectionIndex != aSelectionIndex) {
        qDebug() << "PackageInfo::setSingleSelectionIndex - aSelectionIndex: " << aSelectionIndex;
        // Set Single Selection Index
        mSingleSelectionIndex = aSelectionIndex;
        // Emit Single Selection Index Changed Signal
        emit singleSelectionIndexChanged(mSingleSelectionIndex);
    }
}

//==============================================================================
// Get Count
//==============================================================================
int PackageInfo::count()
{
    return  rowCount();
}

//==============================================================================
// Get Group By Name
//==============================================================================
PackageGroup* PackageInfo::packageGroup(const QString& aName)
{
    // Check Group Name
    if (mGroups.keys().indexOf(aName) != -1) {
        return mGroups[aName];
    }

    return nullptr;
}

//==============================================================================
// Get Package Group By Index
//==============================================================================
PackageGroup* PackageInfo::packageGroup(const int& aIndex)
{
    // Get Keys Count
    int kCount = mGroups.keys().count();
    // Check Index
    if (aIndex >= 0 && aIndex < kCount) {
        return mGroups[mGroups.keys()[aIndex]];
    }

    return nullptr;
}

//==============================================================================
// Select All Groups
//==============================================================================
void PackageInfo::selectAllGroups()
{
    // Append Group Name
    mSelectedGroups = mGroups.keys();
    // Emit Selected Groups Changed Signal
    emit selectedGroupsChanged(mSelectedGroups);
}

//==============================================================================
// Deselect All Groups
//==============================================================================
void PackageInfo::deselectAllGroups()
{
    // Clear Selected Groups
    mSelectedGroups.clear();
    // Emit Selected Groups Changed Signal
    emit selectedGroupsChanged(mSelectedGroups);
}

//==============================================================================
// Set Selected Group
//==============================================================================
void PackageInfo::setSelectedGroup(const QString& aGroupName, const bool& aSelected)
{
    // Check Group Name
    if (mGroups.keys().indexOf(aGroupName) != -1) {
        // Get Selected Group Index
        int sgIndex = mSelectedGroups.indexOf(aGroupName);
        // Check Selected
        if (aSelected) {
            // Check Selected Groups
            if (sgIndex == -1) {
                // Append Group Name
                mSelectedGroups << aGroupName;
                // Emit Selected Groups Changed Signal
                emit selectedGroupsChanged(mSelectedGroups);
            }
        } else {
            // Check Selected Groups
            if (sgIndex != -1) {
                // Remove group
                mSelectedGroups.removeAt(sgIndex);
                // Emit Selected Groups Changed Signal
                emit selectedGroupsChanged(mSelectedGroups);
            }
        }
    }
}

//==============================================================================
// Set Selected Language
//==============================================================================
void PackageInfo::setSelectedLanguage(const QString& aLanguage, const bool& aSelected)
{
    // Check Language
    if (mLanguages.indexOf(aLanguage) != -1) {
        // Get Index
        int slIndex = mSelectedLanguages.indexOf(aLanguage);
        // Check Selected State
        if (aSelected) {
            // Check Selected Languages
            if (slIndex == -1) {
                // Append Selected Language
                mSelectedLanguages << aLanguage;
                // Emit Selected Languages Changed Signal
                emit selectedLanguagesChanged(mSelectedLanguages);
                // Set All Selected Languages
                mAllLanguages = (mLanguages == mSelectedLanguages);
            }
        } else {
            // Check Selected Languages Index
            if (slIndex != -1) {
                // Remove Selected Language
                mSelectedLanguages.removeAt(slIndex);
                // Emit Selected Languages Changed Signal
                emit selectedLanguagesChanged(mSelectedLanguages);
            }
        }
    }
}

//==============================================================================
// Select All Languages
//==============================================================================
void PackageInfo::setAllLanguagesSelected(const bool& aSelected)
{
    // Check All Selected Languages
    if (mAllLanguages != aSelected) {
        // Set All Selected Languages
        mAllLanguages = aSelected;

        // Check Selected
        if (mAllLanguages) {
            // Add All Languages
            mSelectedLanguages = mLanguages;
        } else {
            // Clear Selected Languages
            mSelectedLanguages.clear();
        }

        // Emit Selected Languages Changed Signal
        emit selectedLanguagesChanged(mSelectedLanguages);
    }
}

//==============================================================================
// Get Translation Item By Index
//==============================================================================
TranslationItem* PackageInfo::getItem(const int& aIndex) const
{
    // Init Item
    TranslationItem* item = nullptr;

    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Init First Index Offset
        int fIndex = 0;
        // Iterate Through Groups
        for (QString groupName : mGroups.keys()) {
            // Get Group
            PackageGroup* group = mGroups[groupName];

            // Check Index
            if (aIndex >= fIndex && aIndex < (group->rowCount() + fIndex)) {
                // Set Translation Item
                item = group->getItem(aIndex - fIndex);

                return item;
            }

            // In First Index Offset
            fIndex += group->rowCount();
        }
    }

    return item;
}

//==============================================================================
// Set Item Selected
//==============================================================================
void PackageInfo::setItemSelected(const int& aIndex, const bool& aSelected)
{
    // Get Item
    TranslationItem* item = getItem(aIndex);
    // Check Item
    if (item) {

        // Check Selected State
        if (aSelected && !item->selected()) {
            // Inc Selected Item Count
            mSelectedItemCount++;
            // Emit Selected item Count Changed Signal
            emit selectedItemCountChanged(mSelectedItemCount);
        } else if (!aSelected && item->selected()) {
            // Dec Selected Item Count
            mSelectedItemCount--;
            // Emit Selected item Count Changed Signal
            emit selectedItemCountChanged(mSelectedItemCount);
        }

        // Set Selected
        item->setSelected(aSelected);
        // Refresh
        refresh(aIndex);
    }
}

//==============================================================================
// Select All Items
//==============================================================================
void PackageInfo::selectAllItems()
{
    // Check Groups
    if (mGroups.keys().count() == 0) {
        return;
    }

    // Init Package Index
    int pIndex = 0;

    // Iterate Trough Groups
    for (QString groupName : mGroups.keys()) {
        // Get Group
        PackageGroup* group = mGroups[groupName];
        // Get Item Count
        int iCount = group->mItems.count();
        // Iterate Through Items
        for (int i=0; i<iCount; i++) {
            // Get Translation Item
            TranslationItem* item = group->mItems[i];

            // Check If In Selected Groups
            if (mSelectedGroups.indexOf(item->translationGroup()) != -1) {
                // Check If Not Filtered
                if (acceptItem(item, mFilterText)) {

                    // Check Selected State
                    if (!item->selected()) {
                        // Set Selected Item Count
                        mSelectedItemCount++;
                        // Emit Selected item Count Changed Signal
                        emit selectedItemCountChanged(mSelectedItemCount);
                    }

                    // Set Item Selected
                    item->setSelected(true);

                    // Refresh Item
                    refresh(pIndex);
                }
            }

            // Inc Package Index
            pIndex++;
        }
    }
}

//==============================================================================
// Select No Items
//==============================================================================
void PackageInfo::selectNoItems()
{
    // Check Groups
    if (mGroups.keys().count() == 0) {
        return;
    }

    // Init Package Index
    int pIndex = 0;

    // Iterate Trough Groups
    for (QString groupName : mGroups.keys()) {
        // Get Group
        PackageGroup* group = mGroups[groupName];
        // Get Item Count
        int iCount = group->mItems.count();
        // Iterate Through Items
        for (int i=0; i<iCount; i++) {
            // Get Translation Item
            TranslationItem* item = group->mItems[i];
            // Set Item Selected
            item->setSelected(false);
            // Refresh Item
            refresh(pIndex);
            // Inc Package Index
            pIndex++;
        }
    }

    // Set Selected Item Count
    mSelectedItemCount = 0;
    // Emit Selected item Count Changed Signal
    emit selectedItemCountChanged(mSelectedItemCount);
}

//==============================================================================
// Move Item(s) To a Different Group, If Index is -1 Moves all Selected Items
//==============================================================================
void PackageInfo::moveItem(const QString& aGroup, const int& aIndex)
{
    // Check Group Name
    if (!aGroup.isEmpty()) {
        // Get New group
        PackageGroup* newGroup = mGroups.value(aGroup);

        // Check Index
        if (aIndex != -1) {
            // Get Item
            TranslationItem* item = getItem(aIndex);
            // Check Item
            if (item && item->translationGroup() != aGroup) {
                // Get Current Group
                PackageGroup* itemGroup = mGroups.value(item->translationGroup());
                // Get Index
                int itemIndex = itemGroup ? itemGroup->itemIndex(item) : -1;
                // Check Item Index
                if (itemIndex != -1 && newGroup) {
                    // Begin Reset Model
                    beginResetModel();
                    // Remove Item From Group
                    itemGroup->removeItem(itemIndex, false);
                    // Add Item to New Group
                    newGroup->addItem(item);
                    // End Reset Model
                    endResetModel();
                }
            }
        } else {
            // Iterate Trough Groups
            for (QString groupName : mGroups.keys()) {
                // Check Group Name
                if (groupName != aGroup && newGroup) {
                    // Get Group
                    PackageGroup* group = mGroups.value(groupName);
                    // Get Group Count
                    int gCount = group->rowCount();

                    // Begin Reset Model
                    beginResetModel();

                    // Iterate Through Group Items
                    for (int g=gCount-1; g>=0; g--) {
                        // Get Translation Item
                        TranslationItem* item = group->mItems[g];
                        // Check Selected State
                        if (item->selected()) {
                            // Remove Item
                            group->removeItem(g, false);
                            // Add Item to New Group
                            newGroup->addItem(item);
                        }
                    }

                    // End Reset Model
                    endResetModel();
                }
            }
        }
    }
}

//==============================================================================
// Find/Get Translation Item By Text ID
//==============================================================================
TranslationItem* PackageInfo::findItem(const QString& aTextID)
{
    // Check Text ID
    if (!aTextID.isEmpty()) {
        // Iterate Through Groups
        for (QString groupName : mGroups.keys()) {
            // Get Group
            PackageGroup* group = mGroups[groupName];
            // Find Translation Item
            TranslationItem* item = group->findItem(aTextID);

            // Check Item
            if (item) {
                return item;
            }
        }
    }

    return nullptr;
}

//==============================================================================
// Get Translation Item Index
//==============================================================================
int PackageInfo::itemIndex(TranslationItem* aItem)
{
    // Check Item
    if (aItem) {
        // Init Global/Package Index Offset
        int pIndexOffs = 0;
        // Iterate Through Groups
        for (QString groupName : mGroups.keys()) {
            // Get Group
            PackageGroup* group = mGroups[groupName];

            // Get Group Index
            int gIndex = group->itemIndex(aItem);

            // Check Group
            if (gIndex != -1) {
                return gIndex + pIndexOffs;
            }

            // Inc Global/Package Index
            pIndexOffs += group->rowCount();
        }
    }

    return -1;
}

//==============================================================================
// Refresh Item
//==============================================================================
void PackageInfo::refresh(const int& aIndex)
{
    // Emit Data Changed Signal
    emit dataChanged(index(aIndex), index(aIndex));
}

//==============================================================================
// Add Available Language
//==============================================================================
void PackageInfo::addLanguage(const QString& aLanguage)
{
    // Check Language
    if (!aLanguage.isEmpty() && mLanguages.indexOf(aLanguage) == -1) {
        qDebug() << "PackageInfo::addLanguage - aLanguage: " << aLanguage;

        // Begin Reset Model
        beginResetModel();

        // Add Language
        mLanguages << aLanguage;

        // Sort Languages
        mLanguages.sort();

        // End Reset Model
        endResetModel();

        // Emit Languages ChangedSignal
        emit languagesChanged(mLanguages);

        // ...

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Remove Available Language
//==============================================================================
void PackageInfo::removeLanguage(const QString& aLanguage)
{
    // Get Language Index
    int lIndex = mLanguages.indexOf(aLanguage);

    // Check Language
    if (!aLanguage.isEmpty() && lIndex != -1) {
        qDebug() << "PackageInfo::removeLanguage - aLanguage: " << aLanguage;

        // Begin Reset Model
        beginResetModel();

        // Remove Language
        mLanguages.removeAt(lIndex);

        // End Reset Model
        endResetModel();

        // Emit Languages ChangedSignal
        emit languagesChanged(mLanguages);

        // TODO: Remove Translations From All Groups Translation Items

        // ...

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Destructor
//==============================================================================
PackageInfo::~PackageInfo()
{
    // Save Package
    //savePackage();

    // Shut Down
    shutDown();

    qDebug() << "PackageInfo deleted.";
}

