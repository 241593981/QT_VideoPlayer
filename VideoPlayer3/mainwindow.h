#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QVideoWidget>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct MediaObjectInfo {
    QString fileName;
    QString filePath;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initWidget();
    void mediaPlayerInit();

private:
    Ui::MainWindow *ui;

    QMediaPlayer *videoPlayer;
    QMediaPlaylist *mediaPlaylist;
    QVideoWidget *videoWidget;

public slots:
    void SltVideoListWidgetCliked(QListWidgetItem*); // 视频列表点击
    void SltVideoComboboxCliked(int); //视频倍速点击
    void SltMediaPlayerDurationChanged(qint64); // 媒体时长发生变化
    void SltMediaPlayerPositionChanged(qint64); // 媒体当前播放时间发生变化
    void SltPlayProgressScrollBarReleased(); // 播放进度条
    void SltVolumeScrollBarReleased(); // 音量进度条

private slots:
      void on_file_pushButton_clicked();
      void on_play_pushButton_clicked();
      void on_next_pushButton_clicked();
      void on_pre_pushButton_clicked();
};
#endif // MAINWINDOW_H
