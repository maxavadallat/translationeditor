#ifndef COLORPICKERBUTTON_H
#define COLORPICKERBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QRgb>

//==============================================================================
// Color Picker Button Class
//==============================================================================
class ColorPickerButton : public QPushButton
{
public:
    // Constructor
    explicit ColorPickerButton(QWidget* aParent = nullptr);

    // Get Color
    QRgb getColor();

    // Set Color
    void setColor(const QRgb& aColor);

    // Destructor
    virtual ~ColorPickerButton();

protected:
    // Paint Event
    virtual void paintEvent(QPaintEvent* aEvent);

protected:
    // Color
    QRgb        color;
};

#endif // COLORPICKERBUTTON_H
