#include "qregexp.h"

QRegExp::QRegExp() {
    this->mLength = -1;
}

QRegExp::QRegExp(const QString &pattern) {
    this->pattern = pattern;
}

int QRegExp::indexIn(const QString &str, int offset) {
    //Найти совпадение в str со смещением позиции offset (по умолчанию 0).
    //Возвращает позицию первого совпадения или -1, если совпадений не было.
    QRegularExpression q (pattern);
    QRegularExpressionMatch match = q.match(str,offset);
    bool hasMatch = match.hasMatch();
    if (!hasMatch) { this->mLength = -1; return -1; }
    int startOffset = match.capturedStart();
    int endOffset = match.capturedEnd();
    this->mLength = endOffset - startOffset;
    return startOffset;
}

int	QRegExp::matchedLength() {
    //Длина последней совпавшей строки или -1, если совпадений не было.
    return this->mLength;
}
