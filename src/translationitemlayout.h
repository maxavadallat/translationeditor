#ifndef TRANSLATIONITEMLAYOUT_H
#define TRANSLATIONITEMLAYOUT_H

#include <QObject>
#include <QMap>

class PackageInfo;

//==============================================================================
// Translation Item Layout Class
//==============================================================================
class TranslationItemLayout : public QObject
{
    Q_OBJECT

    // Layout Name
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    // Font Name
    Q_PROPERTY(QString fontName READ fontName WRITE setFontName NOTIFY fontNameChanged)
    // Font Size
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    // Max Width
    Q_PROPERTY(int maxWidth READ maxWidth WRITE setMaxWidth NOTIFY maxWidthChanged)
    // Max Height
    Q_PROPERTY(int maxHeight READ maxHeight WRITE setMaxHeight NOTIFY maxHeightChanged)
    // Max Lines
    Q_PROPERTY(int maxLines READ maxLines WRITE setMaxLines NOTIFY maxLinesChanged)
    // Wrap Type
    Q_PROPERTY(int wrapType READ wrapType WRITE setWrapType NOTIFY wrapTypeChanged)
    // Elide Type
    Q_PROPERTY(int elideType READ elideType WRITE setElideType NOTIFY elideTypeChanged)
    // Max Length
    Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength NOTIFY maxLengthChanged)
    // Shrinkable
    Q_PROPERTY(bool shrinkable READ shrinkable WRITE setShrinkable NOTIFY shrinkableChanged)
    // Alternative Fonts
    Q_PROPERTY(QStringList altFonts READ altFonts WRITE setAltFonts NOTIFY altFontsChanged)

public:
    // Static Constructor - From JSON Object
    static TranslationItemLayout* fromJSON(const QJsonObject& aObject, PackageInfo* aPackage);

    // Constructor
    explicit TranslationItemLayout(PackageInfo* aPackage, QObject* aParent = nullptr);

    // Get Layout Name
    QString name();
    // Set Layout Name
    void setName(const QString& aNewName);

    // Get Font Name
    QString fontName();
    // Set Font Name
    void setFontName(const QString& aFontName);

    // Get Font Size
    int fontSize();
    // Set Font Size
    void setFontSize(const int& aFontSize);

    // Get Max Width
    int maxWidth();
    // Set Max Width
    void setMaxWidth(const int& aMaxWidth);

    // Get Max Height
    int maxHeight();
    // Set Max Height
    void setMaxHeight(const int& aMaxHeight);

    // Get Max Lines
    int maxLines();
    // Set Max Lines
    void setMaxLines(const int& aMaxLines);

    // Get Wrap Type
    int wrapType();
    // Set Wrap Type
    void setWrapType(const int& aWrapType);

    // Get Elide Type
    int elideType();
    // Set Elide Type
    void setElideType(const int& aElideType);

    // Get Max Length
    int maxLength();
    // Set Max Length
    void setMaxLength(const int& aMaxLength);

    // Get Shrinkable
    bool shrinkable();
    // Set Shrinkable
    void setShrinkable(const bool& aShrinkable);

    // Get Alternative Fonts
    QStringList altFonts();
    // Set Alternative Fonts
    void setAltFonts(const QStringList& aAltFonts);

    // Get Alternative Font Index
    Q_INVOKABLE int getAlternativeFontIndex(const QString& aLanguage);
    // Set Alternative Font Index
    Q_INVOKABLE void setAlternativeFontIndex(const QString& aLanguage, const int& aIndex);
    // Clear Alternative Font Indexes
    Q_INVOKABLE void clearAlternativeFontIndexes();

    // Export To JSON
    QJsonObject exportToJSON();

signals:
    // Layout Name Changed Signal
    void nameChanged(const QString& newName);
    // Font Name Changed Signal
    void fontNameChanged(const QString& newFontName);
    // Font Size Changed Signal
    void fontSizeChanged(const int& newSize);
    // Max Width Changed Signal
    void maxWidthChanged(const int& newMaxWidth);
    // Max Height Changed Signal
    void maxHeightChanged(const int& newMaxHeight);
    // Max Lines Changed Signal
    void maxLinesChanged(const int& newMaxLines);
    // Wrap Type Changed Signal
    void wrapTypeChanged(const int& newWrapType);
    // Elide Type Changed Signal
    void elideTypeChanged(const int& newElideType);
    // Max Length Changed Signal
    void maxLengthChanged(const int& newMaxLength);
    // Shrinkable Changed Signal
    void shrinkableChanged(const bool& aShrinkable);
    // Alternative Fonts Changed Signal
    void altFontsChanged(const QStringList& aAltFonts);
    // Alternative Font Index Changed Signal
    void altFontIndexChanged(const int& aAltFontIndex);
    // Layout Changed Signal
    void layoutChanged();

protected:
    // Set Package Dirty
    void setPackageDirty();

protected: // Data
    // Current Package
    PackageInfo*            mPackage;
    // Name
    QString                 mName;
    // Font Name
    QString                 mFontName;
    // Font Size
    int                     mFontSize;

    // Max Width
    int                     mMaxWidth;
    // Max Height
    int                     mMaxHeight;

    // Max Lines
    int                     mMaxLines;

    // Wrap Type
    int                     mWrapType;
    // Elide Type
    int                     mElideType;

    // Max Length
    int                     mMaxLength;

    // Shrinkable
    bool                    mShrinkable;

    // Alternative Fonts
    QStringList             mAlternativeFonts;

    // Alternative Font Indexs
    QMap<QString, int>      mAltFontIndexes;
};

#endif // TRANSLATIONITEMLAYOUT_H
