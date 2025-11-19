#pragma once

#include <QWidget>

namespace ui
{
    class SideTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit SideTab(QWidget* parent = nullptr);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        QSize sizeHint() const override;

    private:
        bool mHover;
    };
}
