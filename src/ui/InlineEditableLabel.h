#pragma once
#include <qgraphicseffect.h>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QToolButton>
#include <QSvgWidget>
namespace ui
{

    class InlineEditableLabel : public QWidget
    {
        Q_OBJECT
    public:
        explicit InlineEditableLabel(QWidget* parent = nullptr);

        void setText(const QString& text);
        QString getText() const;

    signals:
        void textChanged(const QString& text);

    public slots:
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;

    protected:
        void mousePressEvent(QMouseEvent* event) override;

    private slots:
        void startEditing();
        void finishEditing();
        void fadeIconTo(qreal targetOpacity);

    private:
        QLabel* mLabel;
        QLineEdit* mLineEdit;
        QSvgWidget* mEditIcon;
        QGraphicsOpacityEffect* mIconEffect;
        QPropertyAnimation* mAnim;
    };
} // ui