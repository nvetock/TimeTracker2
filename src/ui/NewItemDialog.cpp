#include "NewItemDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace ui
{
    NewItemDialog::NewItemDialog(const QString& title,
                                 const QString& prompt,
                                 QWidget* parent)
        : QDialog(parent)
    {
        setWindowTitle(title);
        setModal(true);

        auto* root = new QVBoxLayout(this);
        root->setContentsMargins(16, 16, 16, 16);
        root->setSpacing(8);

        auto* label = new QLabel(prompt, this);
        label->setWordWrap(true);

        mEdit = new QLineEdit(this);
        mEdit->setPlaceholderText("Enter name…");

        auto* buttonRow = new QHBoxLayout();
        buttonRow->setContentsMargins(0, 0, 0, 0);
        buttonRow->setSpacing(8);

        auto* cancelBtn = new QPushButton(tr("Cancel"), this);
        mOkButton = new QPushButton(tr("OK"), this);
        mOkButton->setDefault(true);

        mOkButton->setEnabled(false); // disabled until text entered

        buttonRow->addStretch();
        buttonRow->addWidget(cancelBtn);
        buttonRow->addWidget(mOkButton);

        root->addWidget(label);
        root->addWidget(mEdit);
        root->addLayout(buttonRow);

        connect(cancelBtn, &QPushButton::clicked,
                this, &QDialog::reject);

        connect(mOkButton, &QPushButton::clicked,
                this, &QDialog::accept);

        connect(mEdit, &QLineEdit::textChanged,
                this, &NewItemDialog::onTextChanged);
    }

    QString NewItemDialog::text() const
    {
        return mEdit ? mEdit->text().trimmed() : QString{};
    }

    void NewItemDialog::onTextChanged(const QString& text)
    {
        if (!mOkButton) return;
        mOkButton->setEnabled(!text.trimmed().isEmpty());
    }
}
