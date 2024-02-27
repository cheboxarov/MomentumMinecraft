#include "archiver.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <zip.h>
#include <QDebug>
#include <QDir>
#include <thread>

Archiver::Archiver(char* filePath, char* archiveRoot, QObject *parent) : QObject(parent), filePath(filePath), archiveRoot(archiveRoot) {}

bool Archiver::enumerateArchive() {

    struct zip *za;// Структура для работы с архивом
    int err; // Код последней ошибки
    // Попытаться открыть файл с архивом, если не получилось - сообщить об ощибке
    if ((za = zip_open(filePath, ZIP_CREATE | ZIP_RDONLY, &err)) == NULL) {
        qDebug() << "archve not found";
        return false;
    }
    // Пройтись по всем элементам архива
    this->loopThroughFiles(this->archiveRoot, za);
    // Закрыть файл с архивом (если не получилось - сообщить об ошибке)
    if (zip_close(za) == -1) {
        qDebug() << "Cant close the archive";
        return false;
    }
    emit archiverFinished();
    return true;
}

bool Archiver::loopThroughFiles(const char* archiveRoot, struct zip *za) {
    int i; // Переменная для итерирования
    struct zip_stat zipStat; // Информация о файле
    size_t currentItemNameLength; // Длина имени текущего элемента в архиве

    for (i = 0; i < zip_get_num_entries(za, 0); i++) {

        if (zip_stat_index(za, i, 0, &zipStat) == 0) {

            currentItemNameLength = strlen(zipStat.name);
            char bufForFileName[300]; // Буфер для имени файла с учётом корневой папки
            strcpy(bufForFileName, archiveRoot);
            strcat(bufForFileName, zipStat.name);
            if (zipStat.name[currentItemNameLength - 1] == '/') { // Если текущий элемент архива - папка, то нужно создать папку на диске

                if (!QDir().mkpath(bufForFileName)) {
                    qWarning() << "Ошибка при создании директории" << bufForFileName;
                    return false;
                }
            } else { // Текущий элемент - файл, а значит нужно последовательно считать все данные и записать их на диск

                writeCurrentFile(i, za, zipStat, bufForFileName);
            }
            if (i && zip_get_num_entries(za, 0))
                emit archiverProgressSignal(i, zip_get_num_entries(za, 0));
        }
    }
    return true;
}


bool Archiver::writeCurrentFile(int i, struct zip *za, struct zip_stat zipStat, char bufForFileName[]) {
    struct zip_file *zipFile; // Структура для работы с текущим файлом архива
    int fd; // Файловый дескриптор
    zip_int64_t lenOfReadData; // Сколько байт было прочитано из файла
    char buf[512]; // Буфер для чтения информации из архива

    zipFile = zip_fopen_index(za, i, 0);
    if (!zipFile) {

        return 1;
    }
    fd = open(bufForFileName, O_RDWR | O_TRUNC | O_CREAT | O_BINARY, 0644); //Создать файл
    if (fd < 0) { // Если файл нельзя создать, значит произошла ошибка

        zip_fclose(zipFile);
        return false;
    }
    zip_uint64_t sum = 0; // Полный размер текущего файла в архиве
    while (sum != zipStat.size) {

        lenOfReadData = zip_fread(zipFile, buf, sizeof(buf) * sizeof(buf[0]));
        if (lenOfReadData < 0) {
            qDebug() << "Error of reading archiveted file";
            zip_fclose(zipFile);
            return false;
        }
        if (write(fd, buf, lenOfReadData) < 0) {
            qDebug() << "Error or writing file on disk";
            zip_fclose(zipFile);
            return false;
        }
        sum += lenOfReadData;
    }
    close(fd);
    int zipCloseResult = zip_fclose(zipFile);
    if (zipCloseResult != 0) {
        qDebug() << "Result archiver error: " << zipCloseResult;
        return false;
    }
    return true;
}
