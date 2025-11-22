#include "ExportPage.h"

#include <QLabel>
#include <QVBoxLayout>

namespace ui
{
    ExportPage::ExportPage(QWidget* parent)
        : BaseCardPage(parent)
    {
        setTitle("EXPORT");
        showBackButton(true);

        auto* body = getBodyLayout();
        auto* label = new QLabel("Export UI goes here.", this);
        label->setAlignment(Qt::AlignCenter);
        body->addWidget(label);
    }
}
