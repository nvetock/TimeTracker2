#include "SideTab.h"

#include <QPainter>
#include <QPainterPath>

namespace ui
{
    SideTab::SideTab(QWidget* parent)
    {
    }

    QSize SideTab::sizeHint() const
    {
        return QSize(36, 200);
    }

    void SideTab::paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);

        QPainter p{this};
        p.setRenderHint(QPainter::Antialiasing, true);

        QRectF r = rect();
        r.adjust(0.5, 0.5, -0.5, -0.5);

        QPainterPath path;
        const qreal radius = 12.0;

        path.moveTo(r.left() + radius, r.top());
        path.lineTo(r.right(), r.top());
        path.lineTo(r.right(), r.bottom());

        path.lineTo(r.left() + radius, r.bottom());
        path.quadTo(r.left(), r.bottom(), r.left(), r.bottom() - radius);
        path.lineTo(r.left(), r.top() + radius);
        path.quadTo(r.left(), r.top(), r.left() + radius, r.top());

        p.fillPath(path, QColor("#262626"));

        // Border
        p.setPen(QColor("#262626"));
        p.drawPath(path);

        // Draw verrttical text
        p.save();
        // Rotate around center
        QPointF c = r.center();
        p.translate(c);
        p.rotate(-90);
        p.translate(-c);

        // Build the font
        QFont font;
        font.setFamily("Barlow Semi Condensed");
        font.setBold(true);
        font.setPointSize(9);
        font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
        p.setFont(font);
        p.setPen(QColor("#CCF4A9"));

        QString text{"TIME TRACKER"};
        QSizeF textSize = p.fontMetrics().size(0,text);
        QPointF textPos(c.x() - textSize.width() * 0.5,
                        c.y() + textSize.height() * 0.5);

        p.drawText(textPos, text);
        p.restore();
    }
}
