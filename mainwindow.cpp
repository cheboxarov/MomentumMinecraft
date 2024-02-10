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
#include "QPainterPath"
#include "QGraphicsBlurEffect"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , isReadyToStart(false)
    , ui(new Ui::MainWindow)
    , versions(new Versions)
    , downloader(new Downloader)
{
    ui->setupUi(this);
    ui->upperGroupBox->installEventFilter(this);
    ui->clientNameLine->installEventFilter(this);

    QPainterPath painPath;
    painPath.addRoundedRect(rect(), 8, 8, Qt::AbsoluteSize);
    this->setMask(painPath.toFillPolygon().toPolygon());

    initMainWindow();
    initClientSettings();

}

MainWindow::~MainWindow()
{
    if (client.nickname != "Твой никнейм" && client.nickname != "Выберите ник!") {
        Config::saveConfig(client.nickname);
        qDebug() << "Save config" << client.nickname;
    }
    delete ui;
    delete versions;
}

void MainWindow::initClientSettings() {
    QString client_name = Config::getNickname();
    ui->clientNameLine->setText(client_name);
    client.nickname = client_name;
}

void MainWindow::initMainWindow()
{
    startButton = new StartButton(new QLabel(ui->groupBox_2), ui->groupBox_2);

    int id = QFontDatabase::addApplicationFont(":/image/images/minecraft.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont minectaft_font(family);

    minectaft_font.setPixelSize(16);
    ui->textAbout->setFont(minectaft_font);
    ui->textAbout->setAlignment(Qt::AlignCenter);
    ui->textAbout->setIndent(5);

    minectaft_font.setWeight(QFont::Bold);
    minectaft_font.setPixelSize(35);
    ui->titleAbout->setFont(minectaft_font);

    minectaft_font.setPixelSize(30);
    ui->downloadProgress->setFont(minectaft_font);
    ui->downloadProgress->setAlignment(Qt::AlignCenter);

    minectaft_font.setWeight(QFont::Normal);
    minectaft_font.setPixelSize(20);
    ui->clientNameLine->setFont(minectaft_font);
    ui->clientNameLine->setAlignment(Qt::AlignCenter);

    setReadyToStart(isReadyToStart);

    versions->loadVersions();
    showVersions();

    QObject::connect(downloader, &Downloader::downloadFinished, this, &MainWindow::downloadFinished);
    QObject::connect(downloader, &Downloader::downloadProgress, this, &MainWindow::changeDownloadProgress);

    QObject::connect(startButton, SIGNAL(clicked()), this, SLOT(on_startButton_clicked()));
}

void MainWindow::setReadyToStart(bool state) {
    isReadyToStart = state;
    if (state) {
        startButton->setText(QCoreApplication::translate("MainWindow", "\320\230\320\223\320\240\320\220\320\242\320\254", nullptr));
    } else {
        startButton->setText(QString("Загрузить"));
    }
}

void MainWindow::showVersions() {
    if (versions->getLoadedVersions().empty())
        return;

    for(Version version : versions->getLoadedVersions()) {
        qDebug() << "Loaded version:" << version.minecraft_version << version.name << version.size << version.url_to_download;
        versionButtons.push_back(new VersionButton(version, versionButtons.size(), ui->versionsBox));
    }
    for(VersionButton* btn : versionButtons) {
        QObject::connect(btn, SIGNAL(clickSignal(int)), this, SLOT(on_click_versionButton(int)));
    }
    if (versionButtons[0])
        setSelecterVersionBtn(0);
}



void MainWindow::setSelecterVersionBtn(int index) {
    VersionButton* btn = versionButtons[index];
    btn->setSelected(true);
    ui->titleAbout->setText(versions->getLoadedVersions()[index].name);
    ui->titleAbout->setAlignment(Qt::AlignCenter);
    ui->textAbout->setText(versions->getLoadedVersions()[index].description);
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
        startButton->setText("Отменить");
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
    if (getSelectedVersion().isNull)
        return;
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
    setReadyToStart(checkVersionIsDownladed());
    if (!isReadyToStart) {
        if (!isDownloading()) {
            downloadSelectedVersion();
        } else {
            stopDownloading();
        }
    } else {
        if (getSelectedVersion().isNull)
            return;
        std::string start(getSelectedVersion().startcmd);
        if (client.nickname == "Твой никнейм")
            ui->clientNameLine->setText("Выберите ник!");
        if (client.nickname != "Твой никнейм" && client.nickname != "Выберите ник!") {
            Config::saveConfig(client.nickname);
            replace_first(start, "nickname", client.nickname.toStdString());
            this->hide();
            system(start.c_str());
            this->show();
        }
    }
}

Version MainWindow::getSelectedVersion() {
    for (VersionButton* btn : versionButtons ) {
        if (btn->getVersion().id == selectedVersionIndex) {
            return btn->getVersion();
        }
    }
    return Version(true);
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
    if (getSelectedVersion().isNull)
        return false;
    Version selected_ver = getSelectedVersion();
    QString repos = QCoreApplication::applicationDirPath() + "/" + selected_ver.name_of_file + ".zip";
    std::fstream file(repos.toStdString());
    if (!file)
        return false;
    file.seekg (0, std::ios::end);
    int size = file.tellg();
    file.close();
    if (size == selected_ver.size) {
        startButton->setText(QString("Загрузка"));
        std::string game_dir = QString(QCoreApplication::applicationDirPath() + "/game").toStdString();
        if (!dirExists(game_dir)) {
            Archiver ar((char*)repos.toStdString().c_str(), "");
            ar.enumerateArchive();
        }
        startButton->setText(QString("Играть"));
        return true;
    }
    return false;
}


void MainWindow::on_pushButton_clicked()
{
    this->close();
}

bool MainWindow::eventFilter(QObject* obj, QEvent* e) {
    if (obj == (QObject*)ui->upperGroupBox) {
        if (e->type() == QEvent::Enter) {
            m_inUpperDrag = true;
        }
        if (e->type() == QEvent::Leave) {
            m_inUpperDrag = false;
        }
        return QWidget::eventFilter(obj, e);;
    }
    if (obj == (QObject*)ui->clientNameLine) {
        if (e->type() == QEvent::MouseButtonPress) {
            if (ui->clientNameLine->text() == "Твой никнейм" || ui->clientNameLine->text() == "Выберите ник!") {
                ui->clientNameLine->setText("");
            }
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePoint = event->pos();
        event->accept();
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (m_inUpperDrag) {
        const QPointF delta = event->globalPos() - m_mousePoint;
        move(delta.toPoint());

        event->accept();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    showMinimized();
}


void MainWindow::on_clientNameLine_textChanged(const QString &nick)
{
    client.nickname = nick;
}

