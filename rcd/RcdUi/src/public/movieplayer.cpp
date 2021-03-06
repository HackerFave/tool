/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "movieplayer.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QLabel>
#include <QMovie>
#include <QSlider>
#include <QSpinBox>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
QScopedPointer<MoviePlayer> MoviePlayer::P_MoviePlayer;
mutex MoviePlayer:: _mutex;
MoviePlayer *MoviePlayer::instance()
{
    if (P_MoviePlayer.isNull()) {
        _mutex.lock();
        if (P_MoviePlayer.isNull()) {
            P_MoviePlayer.reset(new MoviePlayer);
        }
        _mutex.unlock();
    }

    return P_MoviePlayer.data();
}

MoviePlayer::MoviePlayer(QWidget *parent)
    : QWidget(parent)
{

    this->setAttribute(Qt::WA_ShowModal);
    QDesktopWidget *desktop = QApplication::desktop();

    QRect deskRect = desktop->availableGeometry();

    int x = (deskRect.width() - this->width())/2;

    int y = (deskRect.height() - this->height())/2;

    this->move(x, y);
    //this->setStyleSheet("QWidget{background-color: rgb(0, 0, 0);}");

    movie = new QMovie(this);
    movie->setCacheMode(QMovie::CacheAll);

    movieLabel = new QLabel(tr("No movie loaded"));
    movieLabel->setAlignment(Qt::AlignCenter);
    movieLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    movieLabel->setBackgroundRole(QPalette::Dark);
    movieLabel->setAutoFillBackground(true);

    currentMovieDirectory = "movies";

    createControls();
    createButtons();

    connect(movie, &QMovie::frameChanged, this, &MoviePlayer::updateFrameSlider);
    connect(movie, &QMovie::stateChanged, this, &MoviePlayer::updateButtons);
    connect(fitCheckBox, &QCheckBox::clicked, this, &MoviePlayer::fitToWindow);
    connect(frameSlider, &QSlider::valueChanged, this, &MoviePlayer::goToFrame);
    connect(speedSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            movie, &QMovie::setSpeed);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(movieLabel);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    updateFrameSlider();
    updateButtons();

    setWindowTitle(tr("Movie Player"));
    //resize(400, 400);
}

void MoviePlayer::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open a Movie"),
                               QApplication::applicationDirPath()+"/filegif");
    if (!fileName.isEmpty())
        openFile(fileName);
}

void MoviePlayer::openFile(const QString &fileName)
{
    currentMovieDirectory = QFileInfo(fileName).path();

    movie->stop();
    movieLabel->setMovie(movie);
    movie->setFileName(fileName);
    movie->start();
    frameLabel->setText(QFileInfo(fileName).fileName());
    updateFrameSlider();
    updateButtons();
}

void MoviePlayer::goToFrame(int frame)
{
    movie->jumpToFrame(frame);
}

void MoviePlayer::fitToWindow()
{
    movieLabel->setScaledContents(fitCheckBox->isChecked());
}

void MoviePlayer::updateFrameSlider()
{
    bool hasFrames = (movie->currentFrameNumber() >= 0);

    if (hasFrames) {
        if (movie->frameCount() > 0) {
            frameSlider->setMaximum(movie->frameCount() - 1);
        } else {
            if (movie->currentFrameNumber() > frameSlider->maximum())
                frameSlider->setMaximum(movie->currentFrameNumber());
        }
        frameSlider->setValue(movie->currentFrameNumber());
    } else {
        frameSlider->setMaximum(0);
    }
    frameLabel->setEnabled(hasFrames);
    frameSlider->setEnabled(hasFrames);
}

void MoviePlayer::updateButtons()
{
    playButton->setEnabled(movie->isValid() && movie->frameCount() != 1
                           && movie->state() == QMovie::NotRunning);
    pauseButton->setEnabled(movie->state() != QMovie::NotRunning);
    pauseButton->setChecked(movie->state() == QMovie::Paused);
    stopButton->setEnabled(movie->state() != QMovie::NotRunning);
}

void MoviePlayer::createControls()
{
    fitCheckBox = new QCheckBox(tr("Fit to Window"));
    fitCheckBox->setStyleSheet("QCheckBox{color: rgb(0, 0, 0);font: 18px;} "
                               "QCheckBox::indicator:unchecked{"
                               "image: url(:/resource/icon/unchecke.png);}"
                               "QCheckBox::indicator:checked{image: url(:/resource/icon/checked.png);}");
    frameLabel = new QLabel(tr("Current frame:"));
    frameLabel->setStyleSheet("QLabel{color: rgb(0, 0, 0);font: 18px;}");

    frameSlider = new QSlider(Qt::Horizontal);
    frameSlider->setTickPosition(QSlider::TicksBelow);
    frameSlider->setTickInterval(10);

   // speedLabel = new QLabel(tr("Speed:"));
   // speedLabel->setStyleSheet("QLabel{color: rgb(0, 0, 0);font: 18px;}");

    speedSpinBox = new QSpinBox;
    speedSpinBox->setRange(1, 9999);
    speedSpinBox->setValue(25);
    movie->setSpeed(25);
    speedSpinBox->setSuffix(tr("%"));

    controlsLayout = new QGridLayout;
    controlsLayout->addWidget(fitCheckBox, 0, 0, 1, 2);
    controlsLayout->addWidget(frameLabel, 1, 0);
    controlsLayout->addWidget(frameSlider, 1, 1, 1, 2);
   // controlsLayout->addWidget(speedLabel, 2, 0);
    controlsLayout->addWidget(speedSpinBox, 2, 1);
}

void MoviePlayer::createButtons()
{
    QSize iconSize(36, 36);

    openButton = new QToolButton;
    openButton->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    openButton->setIconSize(iconSize);
    openButton->setToolTip(tr("Open File"));
    connect(openButton, &QToolButton::clicked, this, &MoviePlayer::open);

    playButton = new QToolButton;
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playButton->setIconSize(iconSize);
    playButton->setToolTip(tr("Play"));
    connect(playButton, &QToolButton::clicked, movie, &QMovie::start);

    pauseButton = new QToolButton;
    pauseButton->setCheckable(true);
    pauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    pauseButton->setIconSize(iconSize);
    pauseButton->setToolTip(tr("Pause"));
    connect(pauseButton, &QToolButton::clicked, movie, &QMovie::setPaused);

    stopButton = new QToolButton;
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    stopButton->setIconSize(iconSize);
    stopButton->setToolTip(tr("Stop"));
    connect(stopButton, &QToolButton::clicked, movie, &QMovie::stop);

    quitButton = new QToolButton;
    quitButton->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    quitButton->setIconSize(iconSize);
    quitButton->setToolTip(tr("Quit"));
    connect(quitButton, &QToolButton::clicked, this, &MoviePlayer::close);

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(openButton);
    buttonsLayout->addWidget(playButton);
    buttonsLayout->addWidget(pauseButton);
    buttonsLayout->addWidget(stopButton);
    buttonsLayout->addWidget(quitButton);
    buttonsLayout->addStretch();
}
