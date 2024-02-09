#ifndef ARCHIVER_H
#define ARCHIVER_H
class Archiver
{
public:
    Archiver(char* filePath, char* archiveRoot);
    bool enumerateArchive();
private:
    char* filePath;
    char* archiveRoot;
    bool loopThroughFiles(const char* archiveRoot, struct zip *za);
    bool writeCurrentFile(int i, struct zip *za, struct zip_stat zipStat, char bufForFileName[]);
};

#endif // ARCHIVER_H
