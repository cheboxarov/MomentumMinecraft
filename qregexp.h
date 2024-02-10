#ifndef QREGEXP_H
#define QREGEXP_H
#include <QRegularExpression>
class QRegExp {
public:
    QRegExp();
    QRegExp(const QString &pattern);
    int indexIn(const QString &str, int offset = 0);
    int matchedLength();
private:
    QString pattern;
    int mLength;
};
#endif // QREGEXP_H
