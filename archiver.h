#ifndef ARCHIVER_H
#define ARCHIVER_H
#include "QObject"
class Archiver : public QObject
{
    Q_OBJECT
public:
    Archiver(char* filePath, char* archiveRoot = "", QObject *parent = nullptr);
    bool enumerateArchive();
signals:
    void archiverProgressSignal(int i, int full);
    void archiverFinished();
private:
    char* filePath;
    char* archiveRoot;
    bool loopThroughFiles(const char* archiveRoot, struct zip *za);
    bool writeCurrentFile(int i, struct zip *za, struct zip_stat zipStat, char bufForFileName[]);
};

#endif // ARCHIVER_H
