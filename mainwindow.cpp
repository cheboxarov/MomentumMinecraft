#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPixmap"
#include "QFontDatabase"
#include "QObject"
#include "startbutton.h"
#include "downloader.h"
#include <fstream>
#include "archiver.h"
#include <windows.h>
#include <string>
#include "QProcess"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isReadyToStart(false)
    , versions(new Versions)
    , downloader(new Downloader)
{
    ui->setupUi(this);
    delete ui->startButton;
    QLabel* box = new QLabel(ui->groupBox_2);
    ui->startButton = new StartButton(box, ui->groupBox_2);
    initMainWindow();
    initClientSettings();
}

void MainWindow::initClientSettings() {
    QString client_name = "Твой никнейм";
    client = new Client(client_name);
    ui->clientNameLine->setText(client_name);
}

void MainWindow::initMainWindow()
{
    int id = QFontDatabase::addApplicationFont(":/image/images/minecraft2.otf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont minectaft_font(family);

    minectaft_font.setPixelSize(18);
    ui->textAbout->setFont(minectaft_font);
    ui->textAbout->setAlignment(Qt::AlignCenter);
    ui->textAbout->setIndent(5);

    minectaft_font.setWeight(QFont::Bold);
    minectaft_font.setPixelSize(40);
    ui->titleAbout->setFont(minectaft_font);

    minectaft_font.setPixelSize(30);
    ui->downloadProgress->setFont(minectaft_font);
    ui->downloadProgress->setAlignment(Qt::AlignCenter);

    ui->clientNameLine->setAlignment(Qt::AlignCenter);

    setReadyToStart(isReadyToStart);

    versions->loadVersions();
    showVersions();

    QObject::connect(downloader, &Downloader::downloadFinished, this, &MainWindow::downloadFinished);
    QObject::connect(downloader, &Downloader::downloadProgress, this, &MainWindow::changeDownloadProgress);

    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(on_startButton_clicked()));
}

void MainWindow::setReadyToStart(bool state) {
    isReadyToStart = state;
    if (state) {
        ui->startButton->setText(QCoreApplication::translate("MainWindow", "\320\230\320\223\320\240\320\220\320\242\320\254", nullptr));
    } else {
        ui->startButton->setText(QString("Загрузить"));
    }
}

void MainWindow::showVersions() {
    for(Version version : versions->loaded_versions) {
        qDebug() << version.minecraft_version << version.name << version.size << version.url_to_download;
        versionButtons.push_back(new VersionButton(version, versionButtons.size(), ui->versionsBox));
    }
    for(VersionButton* btn : versionButtons) {
        QObject::connect(btn, SIGNAL(clickSignal(int)), this, SLOT(on_click_versionButton(int)));
    }
    if (versionButtons[0])
        setSelecterVersionBtn(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete versions;
}

void MainWindow::setSelecterVersionBtn(int index) {
    VersionButton* btn = versionButtons[index];
    btn->setSelected(true);
    ui->titleAbout->setText(versions->loaded_versions[index].name);
    ui->titleAbout->setAlignment(Qt::AlignCenter);
    ui->textAbout->setText(versions->loaded_versions[index].description);
    selectedVersionIndex = btn->getVersion().id;
    qDebug() << selectedVersionIndex;
    setReadyToStart(checkVersionIsDownladed());
}

void MainWindow::on_click_versionButton(int index) {
    if (!isDownloading()) {
        for (VersionButton* btn : versionButtons) {
            if(btn->getIndex() == index && !btn->isSelected()) {
                setSelecterVersionBtn(index);
            }
            else if(btn->getIndex() != index)
                btn->setSelected(false);
        }
    }
}

bool MainWindow::isDownloading() { return downloading; }

void MainWindow::setDownloading(bool state) {
    if (state)
        ui->startButton->setText("Отменить");
    else {
        ui->downloadProgress->setText("");
        setReadyToStart(checkVersionIsDownladed());
    }
    this->downloading = state;
}

void MainWindow::downloadFinished() {
    setDownloading(false);
}

void MainWindow::changeDownloadProgress(qint64 bytesSent, qint64 bytesTotal) {
    if (bytesSent && bytesTotal) {
        QString text("Загрузка "+QString::number((float)bytesSent / bytesTotal * 100, 'f', 1) + " %");
        ui->downloadProgress->setText(text);
    }
}

void MainWindow::downloadSelectedVersion() {
    Version selected_ver = getSelectedVersion();
    QUrl url_to_download = selected_ver.url_to_download;
    QString repos = QCoreApplication::applicationDirPath() + "/" + selected_ver.name_of_file + ".zip";
    qDebug() << repos;
    downloader->downloadFile(url_to_download, repos);
    setDownloading(true);
}

void MainWindow::stopDownloading() {
    delete downloader;
    downloader = new Downloader;
    QObject::connect(downloader, &Downloader::downloadProgress, this, &MainWindow::changeDownloadProgress);
    QObject::connect(downloader, &Downloader::downloadFinished, this, &MainWindow::downloadFinished);
    setDownloading(false);
}

void replace_first(
    std::string& s,
    std::string const& toReplace,
    std::string const& replaceWith
    ) {
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos) return;
    s.replace(pos, toReplace.length(), replaceWith);
}

void MainWindow::on_startButton_clicked()
{
    client->nickname = ui->clientNameLine->text();
    setReadyToStart(checkVersionIsDownladed());
    if (!isReadyToStart) {
        if (!isDownloading()) {
            downloadSelectedVersion();
        } else {
            stopDownloading();
        }
    } else {
        this->hide();
        std::string start(getSelectedVersion().startcmd);
        replace_first(start, "penis", client->nickname.toStdString());
        system(start.c_str());
        this->show();
    }
}

Version MainWindow::getSelectedVersion() {
    for (VersionButton* btn : versionButtons ) {
        if (btn->getVersion().id == selectedVersionIndex) {
            return btn->getVersion();
        }
    }
}

bool dirExists(const std::string& dirName_in)
{
    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
}

bool MainWindow::checkVersionIsDownladed() {
    Version selected_ver = getSelectedVersion();
    QString repos = QCoreApplication::applicationDirPath() + "/" + selected_ver.name_of_file + ".zip";
    std::fstream file(repos.toStdString());
    if (!file)
        return false;
    file.seekg (0, std::ios::end);
    int size = file.tellg();
    file.close();
    if (size == selected_ver.size) {
        ui->startButton->setText(QString("Загрузка"));
        std::string game_dir = QString(QCoreApplication::applicationDirPath() + "/game").toStdString();
        if (!dirExists(game_dir)) {
            Archiver ar((char*)repos.toStdString().c_str(), "");
            ar.enumerateArchive();
        }
        ui->startButton->setText(QString("Играть"));
        return true;
    }
    return false;
}


void MainWindow::on_pushButton_clicked()
{
    this->close();
}

