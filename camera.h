// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CAMERA_H
#define CAMERA_H

#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioSource>
#include <QAudioSink>
#include <QAudioBufferInput>
#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QMediaMetaData>
#include <QMediaRecorder>
#include <QScopedPointer>
#include <QBuffer>

#include <QMainWindow>

#include "qioconsumablebuffer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Camera;
}
class QActionGroup;
QT_END_NAMESPACE

class MetaDataDialog;

class Camera : public QMainWindow
{
    Q_OBJECT

public:
    Camera();

public slots:
    void saveMetaData();

private slots:
    void init();

    void setAudio(const QAudioDevice &audioDevice);
    void setCamera(const QCameraDevice &cameraDevice);

    void startCamera();
    void stopCamera();

    void record();
    void pause();
    void stop();
    void setMuted(bool);

    void takeImage();
    void displayCaptureError(int, QImageCapture::Error, const QString &errorString);

    void configureCaptureSettings();
    void configureVideoSettings();
    void configureImageSettings();

    void displayRecorderError();
    void displayCameraError();

    void updateAudioDevice(QAction *action);
    void updateCameraDevice(QAction *action);

    void updateCameraActive(bool active);
    void updateCaptureMode();
    void updateRecorderState(QMediaRecorder::RecorderState state);
    void setExposureCompensation(int index);

    void updateRecordTime();

    void processCapturedImage(int requestId, const QImage &img);

    void displayViewfinder();
    void displayCapturedImage();

    void readyForCapture(bool ready);
    void imageSaved(int id, const QString &fileName);

    void updateAudios();
    void updateCameras();

    void showMetaDataDialog();

    //void updateAudioBuffer();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::Camera *ui;

    QMenu *audioDevicesMenu = nullptr;
    QMenu *videoDevicesMenu = nullptr;

    QActionGroup *videoDevicesGroup = nullptr;
    QActionGroup *audioDevicesGroup = nullptr;

    QMediaDevices m_audioDevices;
    QMediaDevices m_videoDevices;

    QScopedPointer<QAudioSource> m_audioSource;
    QScopedPointer<QAudioSink> m_audioSink;

    QIOConsumableBuffer m_audioBuffer;
    //QScopedPointer<QAudioBufferInput> m_audioInputBuffer;

    QScopedPointer<QImageCapture> m_imageCapture;
    QMediaCaptureSession m_captureSession;
    QScopedPointer<QCamera> m_camera;
    QScopedPointer<QAudioInput> m_audioInput;
    QScopedPointer<QMediaRecorder> m_mediaRecorder;

    bool m_isCapturingImage = false;
    bool m_applicationExiting = false;
    bool m_doImageCapture = true;

    MetaDataDialog *m_metaDataDialog = nullptr;
};

#endif
