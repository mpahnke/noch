/**
 * NOCH - Network Update Check
 *
 *
 **/

#include <QCoreApplication>
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




#include <iostream>


using namespace std;

//QString getAndPrintArgs(QCoreApplication a)
//{
//    QStringList args = a.arguments();
//    QStringList::const_iterator constIter;

//    for( constIter = args.constBegin(); constIter != args.constEnd(); ++constIter )
//    {
//      cout << (*constIter).toLocal8Bit().constData() << endl;
//    }
//    return result;
//} //// getAndPrintArgs(a);
QT_USE_NAMESPACE

class NetworkUp2DateChecker: public QObject
{
  Q_OBJECT
  QNetworkAccessManager manager;
  QSettings settings;

public:
  static const QString getOrganisationName() { return "lwo"; }
  static const QString getApplicationName() { return "noch"; }
  static const QString getOrganisationDomain() { return "lwo.org"; }
  static const QString getURLKEY() { return "URLs"; }
  static const QString getLATESTKEY() { return "LATESTURL"; }

  NetworkUp2DateChecker();
  void doDownload(const QUrl &url);
  QString saveFileName(const QUrl &url);
  bool saveToDisk(const QString &filename, QIODevice *data);
  QByteArray calCS(const QString &filename, QCryptographicHash::Algorithm hashAlg);

  bool noticedCleanup;

public slots:
  void execute();
  void downloadFinished(QNetworkReply *reply);


};


///
/// \brief NetworkUp2DateChecker::NetworkUp2DateChecker
///
NetworkUp2DateChecker::NetworkUp2DateChecker()
{
  connect( &manager, SIGNAL( finished( QNetworkReply*) ), SLOT(downloadFinished( QNetworkReply*) ) );
  //md5Generator( QCryptographicHash::Sha1 );

  QSettings settings();
}


///
/// \brief NetworkUp2DateChecker::execute
///
void NetworkUp2DateChecker::execute()
{
  QStringList args = QCoreApplication::instance()->arguments();
      args.takeFirst();           // skip the first argument, which is the program's name
      if (args.isEmpty()) {
          printf("Qt Download example - downloads all URLs in parallel\n"
                 "Usage: download url1 [url2... urlN] checked\n"
                 "\n"
                 "Downloads the URLs passed in the command-line to the local directory\n"
                 "If the target file already exists, a .0, .1, .2, etc. is appended to\n"
                 "differentiate.\n"
                 "checked - is the flag to acknoledge and cleanup the stored checksum.\n");
          QCoreApplication::instance()->quit();
          return;
      }

      //foreach (QString arg, args) {
          QUrl url = QUrl::fromEncoded(args.takeFirst().toLocal8Bit());

          noticedCleanup = ( !args.isEmpty() && args.takeFirst() == QString("checked") );
          qDebug() << "flag" << noticedCleanup;
          doDownload(url);
      //}
}


///
/// \brief NetworkUp2DateChecker::saveFileName
/// \param url
/// \return
///
QString NetworkUp2DateChecker::saveFileName(const QUrl &url)
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
bool NetworkUp2DateChecker::saveToDisk(const QString &filename, QIODevice *data)
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
void NetworkUp2DateChecker::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

//#ifndef QT_NO_SSL
//    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
//#endif

    //currentDownloads.append(reply);
}


QByteArray NetworkUp2DateChecker::calCS( const QString &filename, QCryptographicHash::Algorithm hashAlg )
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
void NetworkUp2DateChecker::downloadFinished(QNetworkReply *reply)
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
        {
          if( noticedCleanup )
          {
            settings.beginGroup( getURLKEY() );
            settings.remove( curl.toString() );
            settings.endGroup();
            cout << qPrintable( curl.toString() ) << " change has been acknoledged. checksum has been removed." << endl ;
            result = 2;
          }
          else
          {
            cout << qPrintable( curl.toString() ) << " has been changed. Please Check!" << endl ;
            result = -1;
          }

        }



        QCoreApplication::instance()->exit( result );
    }

    //currentDownloads.removeAll(reply);
    //reply->deleteLater();

    //if (currentDownloads.isEmpty())
        // all downloads finished
    ////    QCoreApplication::instance()->quit();
}


///
/// \brief main
/// \param argc
/// \param argv
/// \return
///
int main(int argc, char *argv[])
{    
    QCoreApplication a(argc, argv);

    a.setOrganizationName( NetworkUp2DateChecker::getOrganisationName() );
    a.setOrganizationDomain( NetworkUp2DateChecker::getOrganisationDomain() );
    a.setApplicationName( NetworkUp2DateChecker::getApplicationName() );



    NetworkUp2DateChecker checker;
    //checker.doDownload( QUrl( "http://qt-project.org/doc/qt-5/qtnetwork-download-main-cpp.html" ) );


        QTimer::singleShot(0, &checker, SLOT(execute()));


    return a.exec();
}

#include "main.moc"
