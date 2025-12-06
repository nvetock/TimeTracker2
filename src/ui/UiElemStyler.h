#pragma once
#include "ClickableSvgWidget.h"

class QString;
class QDate;
class QDateTime;
class QPushButton;
class QGridLayout;
class QRadioButton;
class QCheckBox;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QComboBox;

namespace ui
{
    class ClickableSvgWidget;
    // --------------------------------
    // General
    // --------------------------------

    void buttonStylerPrimary(QPushButton& button);
    void buttonStylerSecondary(QPushButton& button);

    void buttonStylerVLayout(QVBoxLayout& layout, const QString& className);
    void buttonStylerHLayout(QHBoxLayout& layout, const QString& className);

    template<typename T>
    void setZeroMarginAndSpaceBetween(T& layout, int spacing)
    {
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(spacing);
    }

    class QPushButton* generateButton(const QString& buttonText,
                                      const QString& className,
                                      int height,
                                      QWidget* parent = nullptr);

    class QPushButton* generateButton(const QString& buttonText,
                                      const QString& className,
                                      bool isFlat = true,
                                      QWidget* parent = nullptr);

    class QPushButton* generateButton(const QString& buttonText,
                                           const QString& className,
                                           int height,
                                           bool isCheckable = true,
                                           QWidget* parent = nullptr);

    class QLabel* generateLabel(const QString& labelText,
                                const QString& className,
                                const QString& alignment,
                                bool wordWrap = false,
                                QWidget* parent = nullptr);

    // --------------------------------
    // TrackWorkSetupPage
    // --------------------------------

    void comboBoxStyler(QComboBox& comboBox,
                        const QString& className,
                        const QString& placeholderText,
                        bool editable = true);

    void comboArrowStyler(ClickableSvgWidget& arrow,
                          const QString& className,
                          int iconSize);

    class QComboBox* generateComboBox(const QString& className, const QString& placeholderText, bool editable = true, QWidget* parent = nullptr);

    class ClickableSvgWidget* generateIcon(const QString& svgPath,
                                          QWidget* parent,
                                          const QString& className,
                                          int iconSize);




} // ui