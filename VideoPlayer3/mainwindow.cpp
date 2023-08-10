#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initWidget(); // 初始化窗口
    mediaPlayerInit(); // 媒体播放初始化

    connect(ui->videoListWidget, SIGNAL(itemClicked(QListWidgetItem*)),this, SLOT(SltVideoListWidgetCliked(QListWidgetItem*)));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(SltVideoComboboxCliked(int)));
    connect(videoPlayer, SIGNAL(durationChanged(qint64)),this,SLOT(SltMediaPlayerDurationChanged(qint64)));
    connect(videoPlayer,SIGNAL(positionChanged(qint64)), this,SLOT(SltMediaPlayerPositionChanged(qint64)));
    connect(ui->sound_Slider, SIGNAL(sliderReleased()),this, SLOT(SltVolumeScrollBarReleased()));
    connect(ui->video_Slider, SIGNAL(sliderReleased()), this, SLOT(SltPlayProgressScrollBarReleased()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWidget()
{
    ui->comboBox->addItem("0.5x");
    ui->comboBox->addItem("1.0x");
    ui->comboBox->addItem("1.5x");
    ui->comboBox->addItem("2.0x");
    ui->comboBox->setCurrentIndex(1);

    ui->sound_Slider->setValue(50);

    videoWidget = new QVideoWidget(ui->videolabel);
    videoWidget->setStyleSheet("border-image: none;" "background: transparent;""border:none");
    videoWidget->resize(ui->videolabel->size());



}

void MainWindow::mediaPlayerInit()
{
    videoPlayer = new QMediaPlayer(this);
    mediaPlaylist = new QMediaPlaylist(this); // 播放列表
    mediaPlaylist->clear();
    videoPlayer->setPlaylist(mediaPlaylist);
    videoPlayer->setVideoOutput(videoWidget); // 设置视频输出窗口
    mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop); // 设置播放模式，Loop 是列循环
    videoPlayer->setVolume(ui->sound_Slider->value());
}

void MainWindow::on_file_pushButton_clicked()
{
    QString struCurrenPath=QDir::homePath();
    QString strDlgTitle="请选择视频文件";
    QString strFilter="MP4 Files(*.mp4);;All Files(*.*)";

    QString strAllFiles=QFileDialog::getOpenFileName(this,strDlgTitle,struCurrenPath,strFilter);

    //QMessageBox::information(this,"提示",strAllFiles,QMessageBox::Yes|QMessageBox::No);
    if(strAllFiles.isEmpty())
    {
        QMessageBox::information(this,"错误","打开视频文件失败，请重新检查?",QMessageBox::Yes);
        return;
    }
    QMessageBox::information(this,"提示","打开成功",QMessageBox::Yes);

    QFileInfo FileInfo(strAllFiles);
    mediaPlaylist->addMedia(QUrl::fromLocalFile(strAllFiles));
    ui->videoListWidget->addItem(FileInfo.fileName());
}

void MainWindow::on_play_pushButton_clicked()
{
    int state = videoPlayer->state();
    switch (state) {
        case QMediaPlayer::StoppedState:
            ui->play_pushButton->setText("暂停");
            videoPlayer->play();
        break;
        case QMediaPlayer::PlayingState:
            ui->play_pushButton->setText("播放");
            videoPlayer->pause();
        break;
        case QMediaPlayer::PausedState:
            ui->play_pushButton->setText("暂停");
            videoPlayer->play();
        break;
    }
}

void MainWindow::on_next_pushButton_clicked()
{
    videoPlayer->stop(); // 停止播放
    int count = mediaPlaylist->mediaCount(); // 获取播放列表的视频个数
    if (0 == count)
        return;
    /* 列表下一个 */
    mediaPlaylist->next();
    videoPlayer->play();
}

void MainWindow::on_pre_pushButton_clicked()
{
    videoPlayer->stop(); // 停止播放
    int count = mediaPlaylist->mediaCount(); // 获取播放列表的视频个数
    if (0 == count)
        return;
    /* 列表上一个 */
    mediaPlaylist->previous();
    videoPlayer->play();
}

void MainWindow::SltVideoListWidgetCliked(QListWidgetItem *item)
{
    videoPlayer->stop();
    mediaPlaylist->setCurrentIndex(ui->videoListWidget->row(item)); // 选中点击的项
    videoPlayer->play();
}

void MainWindow::SltVideoComboboxCliked(int a)
{
    if(a == 0)
    {
        videoPlayer->setPlaybackRate(0.5);
    }
    if(a == 1)
    {
        videoPlayer->setPlaybackRate(1.0);
    }
    if(a == 2)
    {
        videoPlayer->setPlaybackRate(1.5);
    }
    if(a == 3)
    {
        videoPlayer->setPlaybackRate(2.0);
    }
}

void MainWindow::SltMediaPlayerDurationChanged(qint64 duration)
{
    ui->video_Slider->setRange(0, duration / 1000);
}

void MainWindow::SltMediaPlayerPositionChanged(qint64 position)
{
    if (!ui->video_Slider->isSliderDown()) // 进度条没有按下
            ui->video_Slider->setValue(position / 1000);
}

void MainWindow::SltVolumeScrollBarReleased()
{
    /* 设置音量 */
    videoPlayer->setVolume(ui->sound_Slider->value());
}

void MainWindow::SltPlayProgressScrollBarReleased()
{
    /* 设置媒体播放的位置 */
    videoPlayer->setPosition(ui->video_Slider->value() * 1000);
}
