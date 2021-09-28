#include <QDebug>
#include <QAbstractButton>
#include <QStringList>
#include <QString>

#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QFileInfo>
#include <QSettings>
#include <QFile>
#include <QList>
#include <QDebug>
#include <QTimer>
#include <QUrl>



#include "nochwidget.h"
#include "ui_nochwidget.h"

nochWidget::nochWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::nochWidget)
{
  ui->setupUi(this);

  connect( &manager, SIGNAL( finished( QNetworkReply*) ), SLOT(downloadFinished( QNetworkReply*) ) );
  //md5Generator( QCryptographicHash::Sha1 );

  QSettings settings;
}

nochWidget::~nochWidget()
{
  delete ui;
}


void nochWidget::execute()
{
  QStringList args = qApp->instance()->arguments();
   qDebug() <<  args.takeFirst().toLocal8Bit();
  //args.takeFirst();           // skip the first argument, which is the program's name
      if (args.isEmpty()) {
          qDebug() << "Qt Download example - downloads all URLs in parallel\n"
                 "Usage: download url1 [url2... urlN]\n"
                 "\n"
                 "Downloads the URLs passed in the command-line to the local directory\n"
                 "If the target file already exists, a .0, .1, .2, etc. is appended to\n"
                 "differentiate.\n";
          qApp->instance()->quit();
          return;
      }

      //foreach (QString arg, args) {
          QUrl url = QUrl::fromEncoded(args.takeFirst().toLocal8Bit());
          doDownload(url);
      //}
}


///
/// \brief NetworkUp2DateChecker::saveFileName
/// \param url
/// \return
///
QString nochWidget::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}


///
/// \brief NetworkUp2DateChecker::saveToDisk
/// \param filename
/// \param data
/// \return
///
bool nochWidget::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        cerr << "Could not open "<< qPrintable(filename)<< "for writing: %s\n"  << qPrintable(file.errorString());
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

///
/// \brief NetworkUp2DateChecker::doDownload
/// \param url
///
void nochWidget::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);


    //manager.

//#ifndef QT_NO_SSL
//    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
//#endif

    //currentDownloads.append(reply);
}


QByteArray nochWidget::calcCheckSum(const QString &filename, QCryptographicHash::Algorithm hashAlg )
{
  QCryptographicHash md5Generator( hashAlg );
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) {
      cerr << "Could not open "<< qPrintable(filename)<< "for reading: %s\n"  << qPrintable(file.errorString());

  }

  //QFile file(filename);
   md5Generator.addData( file.readAll() );
   file.close();

   if( file.remove() != true )
   {
     cerr << "Could not remove " << qPrintable( filename ) << endl;
   }
   else
     qDebug() << "removed " << qPrintable( filename );

   return md5Generator.result().toHex();
}


///
/// \brief NetworkUp2DateChecker::downloadFinished
/// \param reply
///
void nochWidget::downloadFinished(QNetworkReply *reply)
{
    int result = 0;
    QUrl curl = reply->url();
    if (reply->error())
    {
      //fprintf(stderr, "Download of %s failed: %s\n",                url.toEncoded().constData(),                qPrintable(reply->errorString()));
      cerr << "download failed" <<  curl.path().toLocal8Bit().constData()  << endl;
    }
    else
    {
        QString filename = saveFileName(curl);
        if (saveToDisk(filename, reply))
            cout << "Download of "<< curl.toEncoded().constData() << " succeeded (saved to " << qPrintable(filename) << ")" << endl;

        settings.beginGroup( getURLKEY() );
        QString oldChecksum=settings.value( QString( curl.toString() ), curl.toString() ).toString();
        settings.endGroup();

        // qDebug() << "oldCS:" << oldChecksum  << "for" << QString( curl.toEncoded() ).replace( QString("/"), QString("|") );

        QByteArray cs = calCS( filename, QCryptographicHash::Md5 );



        if( oldChecksum == cs )
        {
          ui->updateInfoLabel->setText( "no changes for " + curl.toString() + " detected." );
          cout << "no changes for '" << qPrintable( curl.toString() ) << "' detected." << endl ;

        }
        else if( oldChecksum == curl.toString() )
        {
          cout << "no checksum for new URL: '" << qPrintable( curl.toString() ) << "' found." << endl << "Storing now." << endl ;
          settings.beginGroup( getURLKEY() );
          settings.setValue( curl.toString(), QString( cs ) );
          settings.endGroup();

          result = 1;
        }
        else
          result = -1;





        //emit// QCoreApplication::instance()->exit( result );
    }

    //currentDownloads.removeAll(reply);
    //reply->deleteLater();

    //if (currentDownloads.isEmpty())
        // all downloads finished
    ////    QCoreApplication::instance()->quit();
}


void nochWidget::on_ClosebuttonBox_clicked(QAbstractButton *button)
{

    qApp->instance()->quit();
}
