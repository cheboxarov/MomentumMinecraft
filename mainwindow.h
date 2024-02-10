#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "versionbutton.h"
#include "versions.h"
#include <QMouseEvent>
#include <QPoint>
#include "config.h"

class Downloader;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class StartButton;
struct Client {
    Client(QString nickname) : nickname(nickname) {}
    QString nickname;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initMainWindow();
    void setReadyToStart(bool state);
    void showVersions();
    void setSelecterVersionBtn(int index);
    bool isDownloading();
    void setDownloading(bool state);
    bool checkVersionIsDownladed();
    void downloadSelectedVersion();
    void stopDownloading();
    void initClientSettings();
    bool eventFilter(QObject* obj, QEvent* e);
public slots:
    void downloadFinished();
    void changeDownloadProgress(qint64 bytesSent, qint64 bytesTotal);
    void on_click_versionButton(int);
    void on_startButton_clicked();
    Version getSelectedVersion();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
private:
    StartButton* startButton;
    bool m_inUpperDrag = false;
    QPointF m_mousePoint;
    int selectedVersionIndex;
    bool downloading;
    bool isReadyToStart;
    Client* client;
    Ui::MainWindow *ui;
    Versions* versions;
    QVector<VersionButton*> versionButtons;
    Downloader* downloader;
};
#endif // MAINWINDOW_H
