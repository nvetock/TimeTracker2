#include "LogHistoryPage.h"

#include <QLabel>
#include <QVBoxLayout>

namespace ui
{
    LogHistoryPage::LogHistoryPage(QWidget* parent)
        : BaseCardPage(parent)
    {
        setTitle("LOG HISTORY");
        showBackButton(true);

        auto* body = getBodyLayout();
        auto* label = new QLabel("Log history UI goes here.", this);
        label->setAlignment(Qt::AlignCenter);
        body->addWidget(label);
    }
}
