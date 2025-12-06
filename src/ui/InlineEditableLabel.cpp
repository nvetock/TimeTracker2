#include "InlineEditableLabel.h"

#include <QFile>

namespace ui
{
    InlineEditableLabel::InlineEditableLabel(QWidget* parent)
        : QWidget{parent}
    {
        setObjectName("EditableLabel");

        auto* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(8);  // small gap between pencil + text

        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setMinimumWidth(240);

        // Pencil SVG widget
        const QString iconPath = ":/icons/resources/icons/pencil.svg";
        mEditIcon = new QSvgWidget(iconPath, this);
        mEditIcon->setObjectName("InlineEditIcon");
        mEditIcon->setFixedSize(16, 16);
        mEditIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        mEditIcon->setCursor(Qt::PointingHandCursor);
        mEditIcon->resize(mEditIcon->sizeHint());
        mEditIcon->hide();  // hidden until hover
        mEditIcon->raise();

        // Opacity + Ani
        mIconEffect = new QGraphicsOpacityEffect(mEditIcon);
        mIconEffect->setOpacity(0.0);
        mEditIcon->setGraphicsEffect(mIconEffect);

        mAnim = new QPropertyAnimation(mIconEffect, "opacity", this);
        mAnim->setDuration(150);
        mAnim->setEasingCurve(QEasingCurve::OutCubic);

        connect(mAnim, &QPropertyAnimation::finished,
            this, &InlineEditableLabel::handleAnimFinished);

        // Description label
        mLabel = new QLabel("Add description...", this);
        mLabel->setObjectName("InlineEditLabel");
        mLabel->setAlignment(Qt::AlignCenter);
        mLabel->setCursor(Qt::PointingHandCursor);
        mLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        // Line edit
        mLineEdit = new QLineEdit(this);
        mLineEdit->setObjectName("InlineLineEdit");
        mLineEdit->setPlaceholderText("Add description...");
        mLineEdit->hide();
        //mLineEdit->setBaseSize(QSize(500, 72));
        mLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        // Add to layout in Horizontal order
        layout->addWidget(mLabel,     0);
        layout->addWidget(mLineEdit,  0);
        layout->addWidget(mEditIcon, 0);

        connect(mLineEdit, &QLineEdit::editingFinished,
            this, &InlineEditableLabel::finishEditing);
    }

    void InlineEditableLabel::enterEvent(QEnterEvent* event)
    {
        QWidget::enterEvent(event);

        if (mLabel->isVisible())
        {
            //mEditIcon->show();
            fadeIconTo(1.0);
        }
    }

    void InlineEditableLabel::leaveEvent(QEvent* event)
    {
        QWidget::leaveEvent(event);

        if (mLabel->isVisible())
        {
            fadeIconTo(0.0);
            // mEditIcon->hide();
        }
    }

    // void InlineEditableLabel::resizeEvent(QResizeEvent* event)
    // {
    //     QWidget::resizeEvent(event);
    //
    //     if (mLabel->isVisible())
    //     {
    //         const int padding = 4;
    //         mEditIcon->move(
    //             mLabel->width() - mEditIcon->width() - padding, padding
    //         );
    //     }
    // }

    void InlineEditableLabel::resizeEvent(QResizeEvent* event)
    {
        QWidget::resizeEvent(event);

        if (mLabel->isVisible())
        {
            const int padding = 4;

            // Get where the label is inside the layout
            QPoint labelPos = mLabel->pos();       // top-left of label
            int labelWidth  = mLabel->width();
            int labelHeight = mLabel->height();

            // Position the icon just inside the right edge of the label
            int x = labelPos.x() + labelWidth - mEditIcon->width() - padding;
            int y = labelPos.y() + (labelHeight - mEditIcon->height()) / 2;

            mEditIcon->move(x, y);
        }
    }

    void InlineEditableLabel::setText(const QString& text)
    {
        mLabel->setText(
            text.isEmpty()
            ? "Add description..."
            : text);
        mLabel->setAlignment(Qt::AlignCenter);
        mLineEdit->setText(text);
    }

    QString InlineEditableLabel::getText() const
    {
        const QString text = mLabel->text();
        if (text == "Add description...") return {};
        return text;
    }

    void InlineEditableLabel::mousePressEvent(QMouseEvent* event)
    {
        Q_UNUSED(event);
        startEditing();
    }

    void InlineEditableLabel::startEditing()
    {
        fadeIconTo(0.0);
        // mEditIcon->hide();

        mLineEdit->setText(mLabel->text() == "Add description..." ? QString() : mLabel->text());

        mLabel->hide();
        mLineEdit->show();
        mLineEdit->setFocus();
        mLineEdit->selectAll();
    }

    void InlineEditableLabel::finishEditing()
    {
        const QString newText = mLineEdit->text();
        setText(newText);
        emit textChanged(newText);

        mLineEdit->hide();
        mLabel->show();
    }

    void InlineEditableLabel::fadeIconTo(qreal targetOpacity)
    {
        if (!mAnim || !mIconEffect) return;

        mAnim->stop();

        if (targetOpacity > 0.0) mEditIcon->show();

        mAnim->setStartValue(mIconEffect->opacity());
        mAnim->setEndValue(targetOpacity);
        mAnim->start();
    }

    void InlineEditableLabel::handleAnimFinished()
    {
        if (!mIconEffect) return;

        if (qFuzzyIsNull(mIconEffect->opacity())) mEditIcon->hide();
    }
} // ui