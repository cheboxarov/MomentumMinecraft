#include "startbutton.h"

#include <QEvent>
#include <QDebug>
#include "QLabel"
#include "QFontDatabase"

#define WIDTH 45

StartButton::StartButton(QLabel* boxVal, QWidget *parent) :
    QPushButton(parent),
    m_pressAnimation(this, "height"),
    m_colorAnimation(this, "color"),
    box(boxVal)
{
    box->setObjectName("startButtonBox");
    box->setGeometry(QRect(WIDTH, GetHeight()+8, 330, 65));
    box->setStyleSheet("background-color: rgb(109, 100, 157); border-radius:17px; border: 2px solid rgb(89, 80, 137)");
    installEventFilter(this);
    int id = QFontDatabase::addApplicationFont(":/image/images/minecraft.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont minectaft_font(family);
    this->setFont(minectaft_font);
    this->setObjectName("startButton");
    this->setGeometry(QRect(WIDTH, GetHeight(), 330, 65));
    QString css;
    css = "QPushButton { color:white; font-size: 30px; border-radius:17px;";
    css.append("border: 3px solid rgb(129, 120, 177); ");
    css.append("background-color: rgb(149, 140, 197); }");
    setStyleSheet(css);
}

StartButton::~StartButton() {
}

void StartButton::SetColor(const QColor& color)
{
    m_currentColor = color;
    QString css;
    css = "QPushButton { color:white; font-size: 30px; border-radius:17px;";
    css.append("background-color: rgb(149, 140, 197);");
    QString strColor = QString("rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
    css.append("border: 3px solid " + strColor + "; }");
    setStyleSheet(css);
}

void StartButton::SetHeight(int height) {
    this->setGeometry(QRect(WIDTH, height, 330, 65));

}

int& StartButton::GetHeight() { return m_currentHeight; }

const QColor& StartButton::GetColor() const
{
    return m_currentColor;
}



bool StartButton::eventFilter(QObject *obj, QEvent *e)
{
    if (!active)
        return false;
    if (e->type() == QEvent::MouseButtonRelease) {
        SetHeight(GetHeight());
    }

    if (e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonDblClick) {
        SetHeight(GetHeight()+2);
    }

    if (e->type() == QEvent::HoverEnter) {
        StartHoverEnterAnimation();
    }

    if (e->type() == QEvent::HoverLeave) {
        StartHoverLeaveAnimation();
    }

    return false;
}

void StartButton::setActive(bool state) {
    this->setEnabled(state);
    active = state;
    if (!state) {
        this->setText("Загрузка");
    }
}

void StartButton::StartHoverEnterAnimation()
{
    m_colorAnimation.stop();

    m_pressAnimation.stop();

    m_pressAnimation.setDuration(100);
    m_pressAnimation.setStartValue(GetHeight());
    m_pressAnimation.setEndValue(GetHeight()+1);
    m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style

    m_colorAnimation.setDuration(100); //set your transition
    m_colorAnimation.setStartValue(GetColor()); //starts from current color
    m_colorAnimation.setEndValue(QColor(255,255,255));//set your hover color

    m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style

    m_pressAnimation.start();

    m_colorAnimation.start();
}

void StartButton::StartHoverLeaveAnimation()
{
    m_colorAnimation.stop();

    m_pressAnimation.stop();

    m_pressAnimation.setDuration(100);
    m_pressAnimation.setStartValue(GetHeight());
    m_pressAnimation.setEndValue(GetHeight()-1);
    m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style

    m_colorAnimation.setDuration(100); //set your transition
    m_colorAnimation.setStartValue(GetColor()); //starts from current color
    m_colorAnimation.setEndValue(QColor(129, 120, 177));//set your regular color

    m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style

    m_pressAnimation.start();

    m_colorAnimation.start();
}
