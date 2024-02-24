#include "versionbutton.h"
#include <QEvent>
#include <QDebug>
#include <QFont>
#include "QFontDatabase"
#include "versions.h"
#include "QPainter"
VersionButton::VersionButton(Version versionVal, int index, QWidget *parent) :
    QPushButton(parent),
    version(versionVal),
    m_selectAnimation(this, "width"),
    m_colorAnimation(this, "color")
{
    int id = QFontDatabase::addApplicationFont(":/image/images/minecraft.ttf");
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
    height = index*110+5;
    this->setGeometry(QRect(22, index*110+5, 300, 100));
    QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(clickSlot()));
    QString css;
    selected = false;
    css = "QPushButton { color:white; border-radius: 10px; ";
    css.append("border: 2px solid rgb(129, 120, 177); ");
    css.append("background: url(" + version.photo +  "); }");
    qDebug() << version.photo;
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
    this->setGeometry(QRect(22 - width, height, 300 + width, 100));
}

int& VersionButton::getHeight() {
    return height;
}

void VersionButton::setHeight(int&& height) {
    this->height = height;
    this->setGeometry(QRect(22 - width, height, 300 + width, 100));
}

void VersionButton::SetColor(const QColor& color)
{
    m_currentColor = color;
    QString css;
    css = "QPushButton { color:white; border-radius: 10px; ";
    if (selected)
        css.append("border: 2px solid rgb(255,255,255); ");
    else
        css.append("border: 2px solid rgb(129, 120, 177); ");
    QString strColor = QString("rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
    css.append("background-color: " + strColor + "; }");
    setStyleSheet(css);
}

const QColor& VersionButton::GetColor() const
{
    return m_currentColor;
}

const Version& VersionButton::getVersion() const {
    return version;
}

bool VersionButton::eventFilter(QObject *obj, QEvent *e)
{

    if (e->type() == QEvent::HoverEnter) {
        isHovered = true;
        if (!selected)
            StartHoverEnterAnimation();
    }

    if (e->type() == QEvent::HoverLeave) {
        isHovered = false;
        if (!selected)
            StartHoverLeaveAnimation();
    }

    return false;
}

int VersionButton::getIndex() { return index; }

void VersionButton::setSelected(bool state) {
    if (selected == state)
        return;
    selected = state;
    if (state) {
        StartHoverEnterAnimation(20);
        QString css;
        css = "QPushButton { color:white; border-radius: 10px; ";
        if (selected)
            css.append("border: 2px solid rgb(255,255,255); ");
        else
            css.append("border: 2px solid rgb(129, 120, 177); ");
        css.append("background: url(" + version.photo +  "); }");
        setStyleSheet(css);
    } else {
        StartHoverLeaveAnimation(20);
        QString css;
        css = "QPushButton { color:white; border-radius: 10px; ";
        if (selected)
            css.append("border: 2px solid rgb(255,255,255); ");
        else
            css.append("border: 2px solid rgb(129, 120, 177); ");
        css.append("background: url(" + version.photo +  "); }");
        setStyleSheet(css);
    }
}

bool VersionButton::isSelected() { return selected; }

void VersionButton::StartHoverEnterAnimation(int width)
{
    m_selectAnimation.stop();
    m_selectAnimation.setDuration(100);
    m_selectAnimation.setStartValue(getWidth());
    m_selectAnimation.setEndValue(width);
    m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style
    m_selectAnimation.start();

    //m_colorAnimation.stop();

    //m_colorAnimation.setDuration(200); //set your transition
   // m_colorAnimation.setStartValue(GetColor()); //starts from current color
   // m_colorAnimation.setEndValue(QColor(162,122,251));//set your hover color

    //m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style

    //m_colorAnimation.start();
}

void VersionButton::StartHoverLeaveAnimation(int width)
{
    m_selectAnimation.stop();
    m_selectAnimation.setDuration(100);
    m_selectAnimation.setStartValue(getWidth());
    m_selectAnimation.setEndValue(0);
    m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style
    m_selectAnimation.start();

    //m_colorAnimation.stop();

    //m_colorAnimation.setDuration (200); //set your transition
    //m_colorAnimation.setStartValue(GetColor()); //starts from current color
    //m_colorAnimation.setEndValue(QColor(149, 140, 197));//set your regular color

    //m_colorAnimation.setEasingCurve(QEasingCurve::Linear);//animation style

    //m_colorAnimation.start();
}
