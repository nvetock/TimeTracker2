#pragma once

#include <QDialog>

class QLineEdit;
class QLabel;
class QPushButton;

namespace ui
{
    class NewItemDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit NewItemDialog(const QString& title,
                               const QString& prompt,
                               QWidget* parent = nullptr);

        QString text() const;

    private slots:
        void onTextChanged(const QString& text);

    private:
        QLineEdit*   mEdit{nullptr};
        QPushButton* mOkButton{nullptr};
    };
}
