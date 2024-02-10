#ifndef VERSIONBUTTON_H
#define VERSIONBUTTON_H
#include <QPushButton>
#include <QColor>
#include <QPropertyAnimation>
#include "versions.h"
class VersionButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ GetColor WRITE SetColor)
    Q_PROPERTY(int width READ getWidth WRITE setWidth)
public:
    explicit VersionButton(Version version, int index, QWidget* parent = 0);
    void SetColor(const QColor& color);
    const QColor& GetColor() const;
    void setSelected(bool state);
    void setWidth(int width);
    int& getWidth();
    bool isSelected();
    int getIndex();
    Version getVersion();
protected:
    bool eventFilter(QObject *obj, QEvent *e);
public slots:
    void clickSlot();
signals:
    void clickSignal(int);
private:
    Version version;
    bool selected;
    QColor m_currentColor = QColor(149, 140, 197);
    int width = 0;
    QString name;
    QString version_text;
    QPropertyAnimation m_selectAnimation;
    QPropertyAnimation m_colorAnimation;
    int index;
    void StartHoverEnterAnimation(int width = 5);
    void StartHoverLeaveAnimation(int width = 5);
    bool isHovered = false;
};

#endif // VERSIONBUTTON_H
