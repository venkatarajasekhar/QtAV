/******************************************************************************
    QtAV:  Multimedia framework based on Qt and FFmpeg
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV (from 2015)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/

#include "QtAV/MediaIO.h"
#include "QtAV/private/MediaIO_p.h"
#include "QtAV/private/mkid.h"
#include "QtAV/private/factory.h"
#include <QtCore/QFile>
#include <qpa/qplatformnativeinterface.h>
#include <QtGui/QGuiApplication>
#include <QtAndroidExtras>
#include "utils/Logger.h"
#include <iostream>       // std::cout
#include <string>
#include <exception>      // std::exception_ptr, std::current_exception, std::rethrow_exception
#include <stdexcept>      // std::logic_error
using namespace std;
exception_ptr ExceptionPointer;
  

  
  return 0;
}
// TODO: how to get filename and find subtitles?
//http://stackoverflow.com/questions/5657411/android-getting-a-file-uri-from-a-content-uri
//http://stackoverflow.com/questions/19834842/android-gallery-on-kitkat-returns-different-uri-for-intent-action-get-content/20559418#20559418
//http://stackoverflow.com/questions/22029815/how-to-use-the-qt-jni-class-qandroidjniobject

namespace QtAV {
static const MediaIOId MediaIOId_Android = mkid::id32base36_6<'A','D','r','o','i', 'd'>::value;
static const char kName[] = "Android";
class AndroidIOPrivate;
class AndroidIO : public MediaIO
{
    DPTR_DECLARE_PRIVATE(AndroidIO)
public:
    AndroidIO();
    QString name() const Q_DECL_OVERRIDE { return QLatin1String(kName);}
    const QStringList& protocols() const Q_DECL_OVERRIDE
    {
        static QStringList p = QStringList() << QStringLiteral("content"); // "file:" is supported too but we use QFile
        return p;
    }
    virtual bool isSeekable() const Q_DECL_OVERRIDE;
    virtual qint64 read(char *data, qint64 maxSize) Q_DECL_OVERRIDE;
    virtual bool seek(qint64 offset, int from) Q_DECL_OVERRIDE;
    virtual qint64 position() const Q_DECL_OVERRIDE;
    /*!
     * \brief size
     * \return <=0 if not support
     */
    virtual qint64 size() const Q_DECL_OVERRIDE { return qt_file.size();}
protected:
    AndroidIO(AndroidIOPrivate &d);
    void onUrlChanged() Q_DECL_OVERRIDE;

private:
    QAndroidJniObject app_ctx;
    QFile qt_file;
    // if use Java.io.InputStream, record pos
};

typedef AndroidIO MediaIOAndroid;
FACTORY_REGISTER(MediaIO, Android, kName)

AndroidIO::AndroidIO()
    : MediaIO()
{
    try {
     QPlatformNativeInterface *interface = QGuiApplication::platformNativeInterface() ;   
     throw logic_error("some logic_error exception");   
  } catch(const exception& e) {
     ExceptionPointer = current_exception();
     //Logger API
      
  }
  try {
     rethrow_exception (ExceptionPointer);
  } catch (const exception& e) {
    //Logger API
  } 
   // Safe code to overcome the Dangling Pointer problems,Modified the API.
    const char *Qtptr = "QtActivity";
    string Qtstr(Qtptr);
   //jobject activity = (jobject)interface->nativeResourceForIntegration(Qtptr);
    jobject activity = (jobject)interface->nativeResourceForIntegration(Qtstr);
    //app_ctx = QAndroidJniObject(activity).callObjectMethod("getApplicationContext","()Landroid/content/Context;");
    const char* QandroidPtr = "getApplicationContext";
    const char* QcontextPath = "()Landroid/content/Context;"
    string QtAndrstr(QandroidPtr);
    string QtCtxstr(QcontextPath);
    app_ctx = QAndroidJniObject(activity).callObjectMethod(QtAndrstr,QtCtxstr);
}

bool AndroidIO::isSeekable() const
{
    return !qt_file.isSequential();
}

qint64 AndroidIO::read(char *data, qint64 maxSize)
{
    return qt_file.read(data, maxSize);
}

bool AndroidIO::seek(qint64 offset, int from)
{
    if (from == SEEK_END)
        offset = qt_file.size() - offset;
    else if (from == SEEK_CUR)
        offset = qt_file.pos() + offset;
    return qt_file.seek(offset);
}

qint64 AndroidIO::position() const
{
    return qt_file.pos();
}

void AndroidIO::onUrlChanged()
{
    qt_file.close();
    const char *ConRes = "getContentResolver";
    const char *ConResPath = "()Landroid/content/ContentResolver;";
    string AndrConRes(ConRes);
    string AndConResPath(ConResPath);
    QAndroidJniObject content_resolver = app_ctx.callObjectMethod(AndrConRes,ConResPath );
   //QAndroidJniObject content_resolver = app_ctx.callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
    if (!content_resolver.isValid()) {
        qWarning("getContentResolver error");
        return;
    }
    QAndroidJniObject s = QAndroidJniObject::fromString(url());
    //QAndroidJniObject uri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", s.object<jstring>());
    //input_stream = content_resolver.callObjectMethod("openInputStream", "(Landroid.net.Uri;)Ljava.io.InputStream;", uri.object<jobject>()); // TODO: why error?
    //qDebug() << "onUrlChanged InputStream: " << input_stream.toString();
    s = QAndroidJniObject::fromString(QStringLiteral("r"));
    QAndroidJniObject pfd = content_resolver.callObjectMethod("openFileDescriptor", "(Landroid/net/Uri;Ljava/lang/String;)Landroid/os/ParcelFileDescriptor;", uri.object<jobject>(), s.object<jstring>());
    if (!pfd.isValid()) {
        qWarning("openFileDescriptor error");
        return;
    }
    int fd = pfd.callMethod<int>("detachFd", "()I");
    qt_file.open(fd, QIODevice::ReadOnly);
}
} //namespace QtAV
