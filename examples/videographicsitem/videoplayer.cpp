/******************************************************************************
    this file is part of QtAV examples
    Copyright (C) 2012-2015 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "videoplayer.h"
#include <QGraphicsView>

#ifndef QT_NO_OPENGL
#include <QtOpenGL/QGLWidget>
#endif

#include <QCheckBox>
#include <QSlider>
#include <QLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDial>

using namespace QtAV;

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
    , videoItem(0)
{
    try{
    videoItem = new GraphicsItemRenderer;
    }catch(bad_alloc &excb){
      cout << "Exception: Memory Allocation";  
    }
    videoItem->resizeRenderer(640, 360);
    videoItem->setOutAspectRatioMode(VideoRenderer::VideoAspectRatio);
    try{
    QGraphicsScene *scene = new QGraphicsScene(this);
    }catch(bad_alloc &baexc){
        cout << "Exception: Memory Allocation";
    }
    scene->addItem(videoItem);

    view = new QGraphicsView(scene);
    try{
    QSlider *rotateSlider = new QSlider(Qt::Horizontal); 
    }catch(bad_alloc &excbe){
        cout << "Exception: Memory Allocation";
    }
    rotateSlider->setRange(-180,  180);
    rotateSlider->setValue(0);
    try{
    QSlider *scaleSlider = new QSlider(Qt::Horizontal);
    }catch(bad_alloc &balloc){
        cout << "Exception: Memory Allocation";
    }
    scaleSlider->setRange(0, 200);
    scaleSlider->setValue(100);
    try{
    QDial *orientation = new QDial();
    }catch(bad_alloc &exceballoc){
        cout << "Exception: Memory Allocation";
    }
    orientation->setRange(0, 3);
    orientation->setValue(0);

    connect(orientation, SIGNAL(valueChanged(int)), SLOT(setOrientation(int)));
    connect(rotateSlider, SIGNAL(valueChanged(int)), SLOT(rotateVideo(int)));
    connect(scaleSlider, SIGNAL(valueChanged(int)), SLOT(scaleVideo(int)));
    try{
    QPushButton *openBtn = new QPushButton;
    }catch(bad_alloc &excpballoc){
        cout << "Exception: Memory Allocation";
    }
    openBtn->setText(tr("Open"));
    connect(openBtn, SIGNAL(clicked()), SLOT(open()));
    try{
    QCheckBox *glBox = new QCheckBox();
    }catch(bad_alloc &excp){
        cout << "Exception: Memory Allocation";
    }
    glBox->setText(QString::fromLatin1("OpenGL"));
    glBox->setChecked(false);
    connect(glBox, SIGNAL(toggled(bool)), SLOT(setOpenGL(bool)));
    try{
    QHBoxLayout *hb = new QHBoxLayout;
    }catch(bad_alloc &err){
        cout << "Exception: Memory Allocation";
    }
    hb->addWidget(glBox);
    hb->addWidget(openBtn);
    hb->addWidget(orientation);
    try{
    QBoxLayout *layout = new QVBoxLayout;
    }catch(bad_alloc &errbal){
        cout << "Exception: Memory Allocation";
    }
    layout->addWidget(view);
    layout->addWidget(rotateSlider);
    layout->addWidget(scaleSlider);
    layout->addLayout(hb);
    setLayout(layout);

    mediaPlayer.setRenderer(videoItem);
}

VideoPlayer::~VideoPlayer()
{
}

void VideoPlayer::play(const QString &file)
{
    mediaPlayer.play(file);
}

void VideoPlayer::setOpenGL(bool o)
{
    videoItem->setOpenGL(o);
    if (!o) {
        view->setViewport(0);
        return;
    }
#ifndef QT_NO_OPENGL
    QGLWidget *glw = new QGLWidget();//QGLFormat(QGL::SampleBuffers));
    glw->setAutoFillBackground(false);
    view->setViewport(glw);
    view->setCacheMode(QGraphicsView::CacheNone);
#endif
}

void VideoPlayer::setOrientation(int value)
{
    videoItem->setOrientation(value*90);
}

void VideoPlayer::rotateVideo(int angle)
{
    // Handle the Logical Errors
    exception_ptr VideoExc;
  try {
     throw logic_error("some logic_error exception");   // throws
  } catch(const std::exception& e) {
     VideoExc = current_exception();
     cout << "exception caught, but continuing...\n";
  }
     cout << "(after exception)\n";

  try {
     std::rethrow_exception (VideoExc);
  } catch (const exception& e) {
     std::cout << "exception caught: " << e.what() << '\n';
  }
    //rotate around the center of video element
    qreal x = videoItem->boundingRect().width() / 2.0;
    qreal y = videoItem->boundingRect().height() / 2.0;
    videoItem->setTransform(QTransform().translate(x, y).rotate(angle).translate(-x, -y));
}

void VideoPlayer::scaleVideo(int value)
{
    qreal v = (qreal)value/100.0;
    videoItem->setTransform(QTransform().scale(v, v));
}

void VideoPlayer::open()
{
    QString f = QFileDialog::getOpenFileName(0, tr("Open a video"));
    if (f.isEmpty())
        return;
    play(f);
}
