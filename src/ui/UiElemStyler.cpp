#include "UiElemStyler.h"

#include <QWidget>
#include <QString>
#include <QDate>
#include <QDateTime>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>

#include "ClickableSvgWidget.h"

namespace ui
{
    void buttonStylerPrimary(QPushButton& button)
    {
    }

    void buttonStylerSecondary(QPushButton& button)
    {
    }

    void buttonStylerVLayout(QVBoxLayout& layout, const QString& className)
    {
    }

    void buttonStylerHLayout(QHBoxLayout& layout, const QString& className)
    {
    }

    QPushButton* generateButton(const QString& buttonText, const QString& className, const int height,
        QWidget* parent)
    {
        auto* btn = new QPushButton(buttonText.toUpper(), parent);
        btn->setObjectName(className);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMaximumHeight(height);
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

        return btn;
    }

    QPushButton* generateButton(const QString& buttonText, const QString& className,
        const bool isFlat, QWidget* parent)
    {
        auto* btn = new QPushButton(buttonText.toUpper(), parent);
        btn->setObjectName(className);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFlat(isFlat);
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

        return btn;
    }

    class QPushButton* generateButton(const QString& buttonText, const QString& className, const int height, const bool isCheckable,
        QWidget* parent)
    {
        auto* btn = new QPushButton(buttonText.toUpper(), parent);
        btn->setObjectName(className);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMaximumHeight(height);
        btn->setCheckable(isCheckable);
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        return btn;
    }

    QLabel* generateLabel(const QString& labelText, const QString& className, const QString& alignment,
        bool wordWrap, QWidget* parent)
    {
        auto* label = new QLabel(labelText, parent);
        label->setObjectName(className);

        if (alignment == "center") label->setAlignment(Qt::AlignCenter);
        else if (alignment == "left") label->setAlignment(Qt::AlignLeft);
        else if (alignment == "right") label->setAlignment(Qt::AlignRight);

        label->setWordWrap(wordWrap);

        return label;
    }


    void comboBoxStyler(QComboBox& comboBox, const QString& className, const QString& placeholderText, const bool editable)
    {
        comboBox.setObjectName(className);
        comboBox.setEditable(editable);
        comboBox.lineEdit()->setPlaceholderText(placeholderText);
        comboBox.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    void comboArrowStyler(ClickableSvgWidget& arrow, const QString& className, int iconSize)
    {
        arrow.setObjectName(className);
        arrow.setFixedSize(iconSize, iconSize);
        arrow.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    class QComboBox* generateComboBox(const QString& className, const QString& placeholderText, bool editable,
        QWidget* parent)
    {
        auto* box = new QComboBox(parent);
        box->setObjectName(className);
        box->setEditable(editable);
        box->lineEdit()->setPlaceholderText(placeholderText);
        box->lineEdit()->setStyleSheet("QLineEdit { selection-background-color: #00E099;}");
        box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        box->setInsertPolicy(QComboBox::NoInsert);

        return box;
    }

    ClickableSvgWidget* generateIcon(const QString& svgPath, QWidget* parent, const QString& className, const int iconSize)
    {
        auto* obj = new ClickableSvgWidget(svgPath, parent);
        obj->setObjectName(className);
        obj->setFixedSize(iconSize, iconSize);
        obj->raise();
        obj->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        return obj;
    }

} // ui