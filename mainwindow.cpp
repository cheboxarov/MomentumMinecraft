#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFontDatabase>
#include <QObject>
#include "startbutton.h"
#include "downloader.h"
#include <fstream>
#include "archiver.h"
#include <windows.h>
#include <string>
#include <QProcess>
#include <QPainterPath>
#include <QGraphicsBlurEffect>
#include <QWheelEvent>
#include <thread>

void replaceFirst(
    std::string& s,
    const std::string& toReplace,
    const std::string& replaceWith)
{
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos)
        return;
    s.replace(pos, toReplace.length(), replaceWith);
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      isInUpperDrag(false),
      ui(new Ui::MainWindow),
      versions(new Versions),
      downloader(new Downloader),
      selectedVersionIndex(-1),
      downloading(false),
      isReadyToStart(false)
{
    ui->setupUi(this);
    ui->upperGroupBox->installEventFilter(this);
    ui->clientNameLine->installEventFilter(this);
    ui->scrollArea->installEventFilter(this);
    QPainterPath roundedRectPath;
    roundedRectPath.addRoundedRect(rect(), 8, 8, Qt::AbsoluteSize);
    this->setMask(roundedRectPath.toFillPolygon().toPolygon());

    initializeMainWindow();
    initializeClientSettings();
}

MainWindow::~MainWindow()
{
    if (client.nickname != "Твой никнейм" && client.nickname != "Выберите ник!")
    {
        Config::saveConfig(client.nickname);
        qDebug() << "Save config" << client.nickname;
    }
    delete ui;
    delete versions;
}

void MainWindow::initializeClientSettings()
{
    const QString& clientName = Config::getNickname();
    ui->clientNameLine->setText(clientName);
    client.nickname = clientName;
}

void MainWindow::initializeMainWindow()
{
    startButton = new StartButton(new QLabel(ui->groupBox_3), ui->groupBox_3);

    int id = QFontDatabase::addApplicationFont(":/image/images/minecraft.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont minecraftFont;
    minecraftFont.setFamily(fontFamily);

    minecraftFont.setPixelSize(16);
    ui->textAbout->setFont(minecraftFont);
    ui->textAbout->setAlignment(Qt::AlignCenter);
    ui->textAbout->setIndent(10);

    minecraftFont.setWeight(QFont::Bold);
    minecraftFont.setPixelSize(35);
    ui->titleAbout->setFont(minecraftFont);

    minecraftFont.setPixelSize(25);
    ui->downloadProgress->setFont(minecraftFont);
    ui->downloadProgress->setAlignment(Qt::AlignCenter);

    minecraftFont.setWeight(QFont::Normal);
    minecraftFont.setPixelSize(20);
    ui->clientNameLine->setFont(minecraftFont);
    ui->clientNameLine->setAlignment(Qt::AlignCenter);

    QObject::connect(versions, &Versions::versionsLoaded, this, &MainWindow::handleLoadedVersions);
    QObject::connect(downloader, &Downloader::downloadFinished, this, &MainWindow::handleDownloadFinished);
    QObject::connect(downloader, &Downloader::downloadProgress, this, &MainWindow::updateDownloadProgress);

    QObject::connect(startButton, SIGNAL(clicked()), this, SLOT(onStartButtonClick()));
    setReadyToStart(isReadyToStart);
    versions->startDownloadVersions();
}

void MainWindow::handleLoadedVersions(bool error)
{
    qDebug() << "Versions loaded!";
    this->show();
    if(!error)
        showAvailableVersions();
}

void MainWindow::setReadyToStart(bool state)
{
    isReadyToStart = state;
    if (state)
    {
        startButton->setText(QCoreApplication::translate("MainWindow", "\320\230\320\223\320\240\320\220\320\242\320\254", nullptr));
    }
    else
    {
        startButton->setText(QString("Загрузить"));
    }
}

void MainWindow::showAvailableVersions()
{
    for (const Version& version : versions->getLoadedVersions())
    {
        qDebug() << "Loaded version:" << version.minecraft_version << version.name << version.size << version.url_to_download << versionButtons.size();
        versionButtons.push_back(new VersionButton(version, versionButtons.size(), ui->scrollArea));
        versionButtons[versionButtons.size() - 1]->show();
    }
    for (VersionButton* btn : versionButtons)
    {
        QObject::connect(btn, SIGNAL(clickSignal(int)), this, SLOT(onVersionButtonClick(int)));
    }
    setSelectedVersionButton(0);
}

void MainWindow::setSelectedVersionButton(int index)
{
    VersionButton* btn = versionButtons[index];
    btn->setSelected(true);
    ui->titleAbout->setText(versions->getLoadedVersions()[index].name);
    ui->titleAbout->setAlignment(Qt::AlignCenter);
    ui->textAbout->setText(versions->getLoadedVersions()[index].description);
    selectedVersionIndex = btn->getVersion().id;
    qDebug() << "Version selected:" << selectedVersionIndex;
    setReadyToStart(isVersionDownloaded(false));
}

void MainWindow::onVersionButtonClick(int index)
{
    if (!isDownloading()  && !isArchiver())
    {
        for (VersionButton* btn : versionButtons)
        {
            if (btn->getIndex() == index && !btn->isSelected())
            {
                setSelectedVersionButton(index);
            }
            else if (btn->getIndex() != index)
                btn->setSelected(false);
        }
    }
}

bool MainWindow::isArchiver() const {
    return archiverState;
}

void MainWindow::setArchiver(bool state) {
    archiverState = state;
    startButton->setActive(!state);
}

bool MainWindow::isDownloading() const
{
    return downloading;
}

void MainWindow::setDownloading(bool state)
{
    if (state)
        startButton->setText("Отменить");
    else
    {
        ui->downloadProgress->setText("");
        setReadyToStart(isVersionDownloaded(true));
    }
    downloading = state;
}

void MainWindow::handleDownloadFinished()
{
    setDownloading(false);
}

void MainWindow::updateDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if (bytesSent && bytesTotal)
    {
        QString text("Загрузка " + QString::number((float)bytesSent / bytesTotal * 100, 'f', 1) + " %");
        ui->downloadProgress->setText(text);
    }
}

void MainWindow::archiverProgress(int current, int total)
{
    QString text("Распаковка " + QString::number((float)current / total * 100, 'f', 1) + " %");
    ui->downloadProgress->setText(text);
}

void MainWindow::archiverFinished() {
    setArchiver(false);
    if (getSelectedVersion().isNull)
        return;
    std::string startCmd(getSelectedVersion().startcmd);
    qDebug() << getSelectedVersion().startcmd;
    if (client.nickname == "Твой никнейм")
        ui->clientNameLine->setText("Выберите ник!");
    if (client.nickname != "Твой никнейм" && client.nickname != "Выберите ник!")
        {
        Config::saveConfig(client.nickname);
        replaceFirst(startCmd, "nickname", client.nickname.toStdString());
        this->hide();
        system(startCmd.c_str());
        this->show();
    }
}

void MainWindow::downloadSelectedVersion()
{
    if (getSelectedVersion().isNull)
        return;
    const Version& selectedVersion = getSelectedVersion();
    const QUrl& urlToDownload = selectedVersion.url_to_download;
    const QString& repository = QCoreApplication::applicationDirPath() + "/" + selectedVersion.name_of_file + ".zip";
    qDebug() << repository;
    downloader->downloadFile(urlToDownload, repository);
    setDownloading(true);
}

void MainWindow::stopDownloading()
{
    delete downloader;
    downloader = new Downloader;
    QObject::connect(downloader, &Downloader::downloadProgress, this, &MainWindow::updateDownloadProgress);
    QObject::connect(downloader, &Downloader::downloadFinished, this, &MainWindow::handleDownloadFinished);
    setDownloading(false);
}

void MainWindow::onStartButtonClick()
{
    setArchiver(true);
    std::thread th([&](){
        setReadyToStart(isVersionDownloaded(true));
    });
    th.detach();
    if (!isReadyToStart)
    {
        if (!isDownloading())
        {
            downloadSelectedVersion();
        }
        else
        {
            stopDownloading();
        }
    }
}

const Version& MainWindow::getSelectedVersion() const
{
    for (const VersionButton* btn : versionButtons)
    {
        if (btn->getVersion().id == selectedVersionIndex)
        {
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

bool MainWindow::isVersionDownloaded(bool withArchive) const
{
    if (getSelectedVersion().isNull)
        return false;
    const Version& selectedVer = getSelectedVersion();
    const QString& repository = QCoreApplication::applicationDirPath() + "/" + selectedVer.name_of_file + ".zip";
    std::fstream file(repository.toStdString());
    if (!file)
        return false;
    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.close();
    if (size == selectedVer.size)
    {
        startButton->setText(QString("Загрузка"));
        const std::string& gameDir = QString(QCoreApplication::applicationDirPath() + "/game").toStdString();
        if (!dirExists(gameDir) && !this->isHidden() && withArchive)
        {
            Archiver archiver((char*)repository.toStdString().c_str());
            QObject::connect(&archiver, &Archiver::archiverProgressSignal, this, &MainWindow::archiverProgress);
            QObject::connect(&archiver, &Archiver::archiverFinished, this, &MainWindow::archiverFinished);
            archiver.enumerateArchive();
        }
        ui->downloadProgress->setText("");
        startButton->setText(QString("Играть"));
        return true;
    }
    return false;
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}

bool MainWindow::eventFilter(QObject* obj, QEvent* e)
{
    if (obj == (QObject*)ui->scrollArea)
    {
        if (e->type() == QEvent::Enter)
        {
            isInScrollArea = true;
        }
        if (e->type() == QEvent::Leave)
        {
            isInScrollArea = false;
        }
        return QWidget::eventFilter(obj, e);
    }

    if (obj == (QObject*)ui->upperGroupBox)
    {
        if (e->type() == QEvent::Enter)
        {
            isInUpperDrag = true;
        }
        if (e->type() == QEvent::Leave)
        {
            isInUpperDrag = false;
        }
        return QWidget::eventFilter(obj, e);
    }
    if (obj == (QObject*)ui->clientNameLine)
    {
        if (e->type() == QEvent::MouseButtonPress)
        {
            if (ui->clientNameLine->text() == "Твой никнейм" || ui->clientNameLine->text() == "Выберите ник!")
            {
                ui->clientNameLine->setText("");
            }
        }
    }
    return false;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePoint = event->pos();
        event->accept();
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (isInUpperDrag)
    {
        const QPointF delta = event->globalPos() - mousePoint;
        move(delta.toPoint());

        event->accept();
    }
}

void MainWindow::wheelEvent(QWheelEvent* event) {
    if (isInScrollArea) {
        if (event->angleDelta().ry() > 0)
        {
            if (wheelOffset > 0)
            {
                for (VersionButton* btn : versionButtons)
                {
                    int height = btn->getHeight();
                    btn->setHeight(height+40);
                }
                wheelOffset -= 1;
            }
        }
        else
        {
            if (versionButtons.size() > 4 && wheelOffset < versionButtons.size() * 1.5)
            {
                for (VersionButton* btn : versionButtons)
                {
                    int height = btn->getHeight();
                    btn->setHeight(height-40);
                }
                wheelOffset += 1;
            }
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    showMinimized();
}

void MainWindow::on_clientNameLine_textChanged(const QString& nick)
{
    client.nickname = nick;
}
