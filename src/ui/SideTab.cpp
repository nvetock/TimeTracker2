#include "SideTab.h"

#include <QPainter>
#include <QPainterPath>

namespace ui
{
    SideTab::SideTab(QWidget* parent)
        : QWidget(parent)
        , mHover{false}
    {
    }

    QSize SideTab::sizeHint() const
    {
        return QSize(32, 152);
    }

    void SideTab::enterEvent(QEnterEvent* event)
    {
        Q_UNUSED(event);
        mHover = true;
        update();
    }

    void SideTab::leaveEvent(QEvent* event)
    {
        Q_UNUSED(event);
        mHover = false;
        update();
    }

    void SideTab::paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);

        QPainter p{this};
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setRenderHint(QPainter::TextAntialiasing, true);
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);

        QRectF r = rect();
        r.adjust(0.5, 0.5, -0.5, -0.5);

        QPainterPath path;
        constexpr qreal radius = 12.0;

        path.moveTo(r.left() + radius, r.top());
        path.lineTo(r.right(), r.top());
        path.lineTo(r.right(), r.bottom());

        path.lineTo(r.left() + radius, r.bottom());
        path.quadTo(r.left(), r.bottom(), r.left(), r.bottom() - radius);
        path.lineTo(r.left(), r.top() + radius);
        path.quadTo(r.left(), r.top(), r.left() + radius, r.top());

        const bool recording = property("recording").toBool();
        const bool idle = property("idle").toBool();
        const bool ended = property("ended").toBool();

        QColor baseColor{};
        if (recording) baseColor = QColor("#FF3B30"); // red
        else if (idle) baseColor = QColor("#FFC107");
        else if (ended) baseColor = QColor("#CCCCCC");
        else baseColor = QColor("#00E099");
        //QColor hoverColor("#00F5C6");
        QColor fill = mHover
            ? baseColor.lighter(110)
            : baseColor;

        //QColor baseColor("#00E099");
        //QColor hoverColor("#00F5C6");
        //QColor fill = mHover
        //    ? hoverColor
        //    : baseColor;

        p.fillPath(path, fill);

        // Border
        p.setPen(fill);
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
        font.setHintingPreference(QFont::PreferFullHinting);
        font.setStyleStrategy(QFont::PreferAntialias);
        font.setBold(true);
        font.setPointSize(11);
        font.setLetterSpacing(QFont::AbsoluteSpacing, 3);
        p.setFont(font);
        p.setPen(QColor("#262626"));

        QString text{"TIME TRACKER"};
        QSizeF textSize = p.fontMetrics().size(0,text);
        QPointF textPos(c.x() - textSize.width() * 0.5,
                        c.y() + textSize.height() * 0.5 - 3);

        p.drawText(textPos, text);
        p.restore();
    }
}
