#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "versionbutton.h"
#include "versions.h"
#include <QMouseEvent>
#include <QPoint>
#include "config.h"
#include "QLabel"

class Downloader;
class StartButton;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void initializeMainWindow();
    void setReadyToStart(bool state);
    void showAvailableVersions();
    void setSelectedVersionButton(int index);
    bool isDownloading() const;
    void setDownloading(bool state);
    bool isVersionDownloaded(bool isFirst = false) const;
    void downloadSelectedVersion();
    void stopDownloading();
    void initializeClientSettings();
    bool eventFilter(QObject* obj, QEvent* e);

public slots:
    void handleLoadedVersions(bool error);
    void handleDownloadFinished();
    void updateDownloadProgress(qint64 bytesSent, qint64 bytesTotal);
    void onVersionButtonClick(int index);
    void onStartButtonClick();
    const Version& getSelectedVersion() const;
    void archiverProgress(int current, int total);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_clientNameLine_textChanged(const QString& newNickname);

private:
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    int wheelOffset = 0;
    StartButton* startButton;
    bool isInUpperDrag;
    bool isInScrollArea = false;
    QPointF mousePoint;
    int selectedVersionIndex;
    bool downloading;
    bool isReadyToStart;

    struct Client {
        QString nickname;
    } client;

    Ui::MainWindow* ui;
    Versions* versions;
    QVector<VersionButton*> versionButtons;
    Downloader* downloader;
    QLabel* loadingVersionsLabel;
};

#endif // MAINWINDOW_H
