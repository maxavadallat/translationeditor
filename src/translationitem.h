#ifndef TRANSLATIONITEM_H
#define TRANSLATIONITEM_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QJsonObject>


#define TEXT_TYPE_GUI               "gui"

#define TEXT_STATE_DRAFT            "Draft"
#define TEXT_STATE_WARNING          "Warning"
#define TEXT_STATE_CHECKED          "Checked"


class TranslationItemLayout;
class PackageInfo;

//==============================================================================
// Translation Item
//==============================================================================
class TranslationItem : public QObject
{
    Q_OBJECT

    // Text/Label ID
    Q_PROPERTY(QString textID READ textID WRITE setTextID NOTIFY textIDChanged)
    // Type
    Q_PROPERTY(QString textType READ textType WRITE setTextType NOTIFY textTypeChanged)
    // Screen
    Q_PROPERTY(QString screen READ screen WRITE setScreen NOTIFY screenChanged)
    // State
    Q_PROPERTY(QString textState READ textState WRITE setTextState NOTIFY textStateChanged)
    // Group/Category
    Q_PROPERTY(QString translationGroup READ translationGroup CONSTANT)
    // Comment
    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)
    // Original Text
    Q_PROPERTY(QString originalText READ originalText WRITE setOriginalText NOTIFY originalTextChanged)
    // Available Languages
    Q_PROPERTY(QStringList languages READ languages NOTIFY languagesChanged)
    // Available Translations
    Q_PROPERTY(QStringList translations READ translations NOTIFY translationsChanged)

    // Current Layout Index
    Q_PROPERTY(int layoutIndex READ layoutIndex WRITE setLayoutIndex NOTIFY layoutIndexChanged)
    // Current Layout
    Q_PROPERTY(TranslationItemLayout* currentLayout READ currentLayout NOTIFY currentLayoutChanged)

    // Selected State
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)

    // Unfit Translation Item Languages
    Q_PROPERTY(QStringList unfitLanguages READ unfitLanguages NOTIFY unfitLanguagesChanged)

public:
    // From JSON Object
    static TranslationItem* fromJSON(const QJsonObject& aObject, PackageInfo* aPackage);

    // Constructor
    explicit TranslationItem(PackageInfo* aPackage, const QString& aGroup, QObject* aParent = nullptr);

    // Get Translation
    Q_INVOKABLE QString getTranslation(const QString& aLanguage);
    // Get Merged Translations
    Q_INVOKABLE QString getMergedTranslations();
    // Get Single Line Translation
    Q_INVOKABLE QString getSingleLineTranslation(const QString& aLanguage);
    // Get Single Line Text
    Q_INVOKABLE QString getSingleLineText(const QString& aText);

    // Add/Set Translation
    void setTranslation(const QString& aLanguage, const QString& aTranslation);
    // Remove Translation
    void removeTranslation(const QString& aLanguage);

    // Get Text ID
    QString textID();
    // Set Text ID
    void setTextID(const QString& aTextID);

    // Get Text Type
    QString textType();
    // Set Text TYpe
    void setTextType(const QString& aTextType);

    // Get Screen
    QString screen();
    // Set Screen
    void setScreen(const QString& aScreen);

    // Get Text State
    QString textState();
    // Set Text State
    void setTextState(const QString& aTextState);

    // Get Text Group
    QString translationGroup();
    // Set Translation Group
    void setTranslationGroup(const QString& aGroupName);

    // Get Original Text
    QString originalText();
    // Set Original Text
    void setOriginalText(const QString& aOriginalText);

    // Get Comment
    QString comment();
    // Set Comment
    void setComment(const QString& aComment);

    // Get Available Languages
    QStringList languages();

    // Get Available Translations
    QStringList translations();

    // Get Number Of Layouts
    Q_INVOKABLE int layoutCount();

    // Get Current Layout Index
    int layoutIndex();
    // Set Current Layout Index
    void setLayoutIndex(const int& aIndex);

    // Get Current Layout
    TranslationItemLayout* currentLayout();

    // Add Translation Layout
    void addLayout(const bool& aNewTranslation = false);
    // Remove Translation Layout
    void removeLayout(const int& aIndex = -1);

    // Go To Previous Layout
    Q_INVOKABLE void prevLayout();
    // Go To Next Layout
    Q_INVOKABLE void nextLayout();

    // Get Selected State
    bool selected();
    // Set Selected State
    void setSelected(const bool& aSelected);

    // Unfit Translation Item Languages
    QStringList unfitLanguages();

    // Get Text Data by Column
    Q_INVOKABLE QString getTextData(const int& aColumn);
    // Set Text Data by Column
    Q_INVOKABLE void setTextData(const int& aColumn, const QString& aText);

    // Mark Translation Item Language Unfit/Out Of Bounds
    Q_INVOKABLE void markItemLanguageUnfit(const QString& aLanguage);
    // UnMark Translation Item Language Unfit/Out Of Bounds
    Q_INVOKABLE void unMarkItemLanguageUnfit(const QString& aLanguage);
    // Clear Unfit Languages
    Q_INVOKABLE void clearUnfitLanguages();

    // Export Translation Item to JSON Object
    QJsonObject exportToJSON();

    // Destructor
    ~TranslationItem();

signals:
    // Text ID Changed Signal
    void textIDChanged(const QString& newTextID);
    // Text Type Changed Signal
    void textTypeChanged(const QString& newTextType);
    // Text Screen Changed Signal
    void screenChanged(const QString& newScreen);
    // Text State Changed Signal
    void textStateChanged(const QString& newTextState);
    // Original Text Changed Signal
    void originalTextChanged(const QString& newOriginalText);
    // Comment Changed Signal
    void commentChanged(const QString& newComment);
    // Languages Changed Signal
    void languagesChanged(const QStringList& availableLanguages);
    // Translations Changed Signal
    void translationsChanged(const QString& availableTranslations);
    // Layouts Changed Signal
    void layoutsChanged();
    // Current Layout Index Changed Signal
    void layoutIndexChanged(const int& newLayoutIndex);
    // Current Layout Changed Signal
    void currentLayoutChanged(TranslationItemLayout* aCurrentLayout);
    // Translation Changed Signal
    void translationChanged(const QString& aLanguage, const QString& aTranslation);
    // Selected State changed Signal
    void selectedChanged(const bool& aSelected);
    // Unfit Translation Item Languages Changed Signal
    void unfitLanguagesChanged(const QStringList& unfitLanguages);

protected:
    // Init
    void init();
    // ShutDown
    void shutDown();
    // Clear
    void clear();

    // Set Package Dirty
    void setPackageDirty();

    // Append Layout
    void appendLayout(TranslationItemLayout* aLayout, const bool& aNewTranslation = false);

protected slots:
    // Layout Changed Slot
    void layoutChanged();

protected: // Data
    // Current Package
    PackageInfo*                    mPackage;
    // ID
    QString                         mTextID;
    // Original Text
    QString                         mOriginalText;
    // Group
    QString                         mGroup;
    // Type
    QString                         mType;
    // Screen
    QString                         mScreen;
    // State
    QString                         mState;
    // Comment
    QString                         mComment;

    // Item Layouts
    QList<TranslationItemLayout*>   mLayouts;

    // Current Layout Index
    int                             mLayoutIndex;

    // Translations
    QMap<QString, QString>          mTranslations;

    // Merged Translations
    QString                         mMergedTranslations;

    // Unfit Translation Item Languages
    QStringList                     mUnfitItemLanguages;

    // Selected
    bool                            mSelected;
};

#endif // TRANSLATIONITEM_H
