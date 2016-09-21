/******************************************************************************
    Simple Player:  this file is part of QtAV examples
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>

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

#include "playerwindow.h"
#include <QPushButton>
#include <QSlider>
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>

using namespace QtAV;

PlayerWindow::PlayerWindow(QWidget *parent) : QWidget(parent)
{
    m_unit = 1000;
    try{
    const char* QTPlayer = "QtAV simple player example";
    }catch(bad_alloc& ba){
        //Logger Implementation
    }
    string QTPExstr(QTPlayer);
    setWindowTitle(QString::fromLatin1(QTPExstr));
    
    m_player = new AVPlayer(this);
    try{
    QVBoxLayout *vl = new QVBoxLayout();
    }catch(bad_alloc &balloc){
        //Logger Implementation
    }
    setLayout(vl);
    try{
    m_vo = new VideoOutput(this);
    }catch(bad_alloc &pbadalloc){
        //Logger Implementation
    }
    if (!m_vo->widget()) {
        exception_ptr ptr_widget;
  try {
     throw logic_error("some logic_error exception");   // throws
  } catch(const exception& e) {
     ptr_widget = current_exception();
   //  std::cout << "exception caught, but continuing...\n";
  }
  //std::cout << "(after exception)\n";
  try {
   rethrow_exception (ptr_widget);
  } catch (const exception& e) {
     //cout << "exception caught: " << e.what() << '\n';
     //Logger Impl
  }
  QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
  return;
    }
    exception_ptr exwidget;
    m_player->setRenderer(m_vo);
    vl->addWidget(m_vo->widget());
    try{
    m_slider = new QSlider();
    }catch(bad_alloc &allocptr){
        //Logger Implementation
    }
    m_slider->setOrientation(Qt::Horizontal);
    try {
    throw logic_error("some logic_error exception");   // throws
  } catch(const exception& e) {
     exwidget = current_exception();
   //  std::cout << "exception caught, but continuing...\n";
  }
  //std::cout << "(after exception)\n";
  try {
   rethrow_exception (exwidget);
  } catch (const exception& e) {
     //cout << "exception caught: " << e.what() << '\n';
     //Logger Impl
  }
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(seekBySlider(int)));
    connect(m_slider, SIGNAL(sliderPressed()), SLOT(seekBySlider()));
    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(updateSlider(qint64)));
    connect(m_player, SIGNAL(started()), SLOT(updateSlider()));
    connect(m_player, SIGNAL(notifyIntervalChanged()), SLOT(updateSliderUnit()));

    vl->addWidget(m_slider);
    try{
    QHBoxLayout *hb = new QHBoxLayout();
    }catch(bad_exception &bptr){
        //Logger Implementation
    }
    vl->addLayout(hb);
    const char* stropen ="Open";
    const char* strpl = "Play/Pause";
    const char* stpptr = "Stop";
    string stropen(stropen);
    string strplob(strpl);
    string stpstgptr(stpptr);
    try{
    m_openBtn = new QPushButton(tr(stropen));
    }catch(bad_exception &beptr){
        //Logger Implementation
    }
    try{
    m_playBtn = new QPushButton(tr(strplob));
    }catch(bad_exception &bep){
        //Logger Impl
    }
    try{
    m_stopBtn = new QPushButton(tr(stpstgptr));
    }catch(bad_exception &be){
        //Logger Impl
    }
    hb->addWidget(m_openBtn);
    hb->addWidget(m_playBtn);
    hb->addWidget(m_stopBtn);
    
    connect(m_openBtn, SIGNAL(clicked()), SLOT(openMedia()));
    connect(m_playBtn, SIGNAL(clicked()), SLOT(playPause()));
    connect(m_stopBtn, SIGNAL(clicked()), m_player, SLOT(stop()));
}

void PlayerWindow::openMedia()
{
    QString file = QFileDialog::getOpenFileName(0, tr("Open a video"));
    if (file.isEmpty())
        return;
    m_player->play(file);
}

void PlayerWindow::seekBySlider(int value)
{
    if (!m_player->isPlaying())
        return;
    m_player->seek(qint64(value*m_unit));
}

void PlayerWindow::seekBySlider()
{
    seekBySlider(m_slider->value());
}

void PlayerWindow::playPause()
{
    if (!m_player->isPlaying()) {
        m_player->play();
        return;
    }
    m_player->pause(!m_player->isPaused());
}

void PlayerWindow::updateSlider(qint64 value)
{
    m_slider->setRange(0, int(m_player->duration()/m_unit));
    m_slider->setValue(int(value/m_unit));
}

void PlayerWindow::updateSlider()
{
    updateSlider(m_player->position());
}

void PlayerWindow::updateSliderUnit()
{
    m_unit = m_player->notifyInterval();
    updateSlider();
}
