#ifndef STARTBUTTON_H
#define STARTBUTTON_H

#include <QPushButton>
#include <QColor>
#include <QPropertyAnimation>
class QLabel;
class StartButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ GetColor WRITE SetColor)
    Q_PROPERTY(int height READ GetHeight WRITE SetHeight)

public:
    explicit StartButton(QLabel* box, QWidget *parent = 0);
    ~StartButton();
    void SetColor(const QColor& color);
    int& GetHeight();
    void SetHeight(int height);
    const QColor& GetColor() const;
    void setActive(bool state);
    bool isActive();

protected:
    bool eventFilter(QObject *obj, QEvent *e);

private:
    QColor m_currentColor = QColor(149, 140, 197);
    int m_currentHeight = 60;
    QPropertyAnimation m_pressAnimation;
    QPropertyAnimation m_colorAnimation;
    QLabel* box;
    void StartHoverEnterAnimation();
    void StartHoverLeaveAnimation();
    bool active = true;
};
#endif // STARTBUTTON_H
