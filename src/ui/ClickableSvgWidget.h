#pragma once
#include <QSvgWidget>

namespace ui
{
    class ClickableSvgWidget : public QSvgWidget
    {
        Q_OBJECT
    public:
        explicit ClickableSvgWidget(const QString& path, QWidget* parent = nullptr)
            : QSvgWidget(path, parent)
        {
            setCursor(Qt::PointingHandCursor);
        }

    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent* event) override
        {
            if (event->button() == Qt::LeftButton)
            {
                emit clicked();
            }

            QSvgWidget::mousePressEvent(event);
        }
    };
} // ui