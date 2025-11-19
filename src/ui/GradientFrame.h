#pragma once

#include <QFrame>
#include <QPainter>
#include <QPainterPath>

namespace ui
{
    class GradientFrame : public QFrame
    {
        Q_OBJECT

    public:
        explicit GradientFrame(QWidget* parent = nullptr)
            : QFrame{parent}
        {
            setAttribute(Qt::WA_TranslucentBackground);
        }

    protected:
        void paintEvent(QPaintEvent* event) override
        {
            QPainter painter{this};
            painter.setRenderHint(QPainter::Antialiasing);

            // Gradient
            QLinearGradient gradient(0,0,0,height());
            gradient.setColorAt(0.0, QColor("#373737"));
            gradient.setColorAt(1.0, QColor("#242424"));

            painter.setBrush(gradient);
            painter.setPen(Qt::NoPen);

            //const QRect r = rect();
            //painter.drawRoundedRect(r, 16,16);

            // corner radius curve
            constexpr int radius = 32;

            QPainterPath path;
            path.moveTo(width(), 0);                        // top right
            path.lineTo(0, 0);                              // top left
            path.lineTo(0, height() - radius);              // down left
            path.quadTo(0, height(), radius, height());     // bottom-left corner
            path.lineTo(width(), height());                 // bottom right
            path.closeSubpath();

            painter.drawPath(path);

            QFrame::paintEvent(event);
        }
    };
} // ui