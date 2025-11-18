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
        QSize sizeHint() const override;
    };
}
