#include "versionbutton.h"
#include <QEvent>
#include <QDebug>
#include <QFont>
#include "QFontDatabase"
#include "versions.h""
VersionButton::VersionButton(Version versionVal, int index, QWidget *parent) :
    QPushButton(parent),
    m_colorAnimation(this, "color"),
    m_selectAnimation(this, "width"),
    version(versionVal)
{
    int id = QFontDatabase::addApplicationFont(":/image/images/minecraft2.otf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont minectaft_font(family);
    minectaft_font.setPixelSize(18);
    this->setFont(minectaft_font);
    this->name = versionVal.name;
    this->version_text = versionVal.minecraft_version;
    this->index = index;
    this->setObjectName(QString::number(index));
    this->setText(name+" "+version_text);
    this->installEventFilter(this);
    this->setGeometry(QRect(22, index*105, 300, 100));
    QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(clickSlot()));
    QString css;
    selected = false;
    css = "QPushButton { color:white; border-radius: 5px; ";
    css.append("border: 3px solid rgb(129, 120, 177); ");
    css.append("background-color: rgb(149, 140, 197); }");
    setStyleSheet(css);
}

void VersionButton::clickSlot() {
    emit clickSignal(this->index);
}

int& VersionButton::getWidth() {
    return width;
}

void VersionButton::setWidth(int width) {
    this->width = width;
    this->setGeometry(QRect(22 - width, index*105, 300 + width*2, 100));
}

void VersionButton::SetColor(const QColor& color)
{
    m_currentColor = color;
    QString css;
    css = "QPushButton { color:white; border-radius: 5px; ";
    if (selected)
        css.append("border: 3px solid rgb(255,255,255); ");
    else
        css.append("border: 3px solid rgb(129, 120, 177); ");
    QString strColor = QString("rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
    css.append("background-color: " + strColor + "; }");
    setStyleSheet(css);
}

const QColor& VersionButton::GetColor() const
{
    return m_currentColor;
}

Version VersionButton::getVersion() {
    return version;
}

bool VersionButton::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::HoverEnter) {
        StartHoverEnterAnimation();
    }

    if (e->type() == QEvent::HoverLeave) {
        StartHoverLeaveAnimation();
    }

    return false;
}

int VersionButton::getIndex() { return index; }

void VersionButton::setSelected(bool state) {
    selected = state;
    if (state) {
        QString css;
        css = "QPushButton { color:white; border-radius: 5px; ";
        if (selected)
            css.append("border: 3px solid rgb(255,255,255); ");
        else
            css.append("border: 3px solid rgb(129, 120, 177); ");
        css.append("background-color: rgb(149, 140, 197); }");
        setStyleSheet(css);
    } else {
        QString css;
        css = "QPushButton { color:white; border-radius: 5px; ";
        if (selected)
            css.append("border: 3px solid rgb(255,255,255); ");
        else
            css.append("border: 3px solid rgb(129, 120, 177); ");
        css.append("background-color: rgb(149, 140, 197); }");
        setStyleSheet(css);
    }
}

bool VersionButton::isSelected() { return selected; }

void VersionButton::StartHoverEnterAnimation()
{
    m_selectAnimation.stop();
    m_selectAnimation.setDuration(200);
    m_selectAnimation.setStartValue(0);
    m_selectAnimation.setEndValue(2);
    m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style
    m_selectAnimation.start();
    m_colorAnimation.stop();

    m_colorAnimation.setDuration(500); //set your transition
    m_colorAnimation.setStartValue(GetColor()); //starts from current color
    m_colorAnimation.setEndValue(QColor(162,122,251));//set your hover color

    m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style

    m_colorAnimation.start();
}

void VersionButton::StartHoverLeaveAnimation()
{
    m_selectAnimation.stop();
    m_selectAnimation.setDuration(200);
    m_selectAnimation.setStartValue(2);
    m_selectAnimation.setEndValue(0);
    m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style
    m_selectAnimation.start();

    m_colorAnimation.stop();

    m_colorAnimation.setDuration(500); //set your transition
    m_colorAnimation.setStartValue(GetColor()); //starts from current color
    m_colorAnimation.setEndValue(QColor(149, 140, 197));//set your regular color

    m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style

    m_colorAnimation.start();
}
