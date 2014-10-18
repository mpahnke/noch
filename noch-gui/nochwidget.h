#ifndef NOCHWIDGET_H
#define NOCHWIDGET_H

#include <QWidget>

class QAbstractButton;
#include <QNetworkAccessManager>
#include <QSettings>

#include <iostream>


using namespace std;

namespace Ui {
class nochWidget;
}

QT_USE_NAMESPACE

class nochWidget : public QWidget
{
  Q_OBJECT

  QNetworkAccessManager manager;
  QSettings settings;

public:
  explicit nochWidget(QWidget *parent = 0);
  ~nochWidget();

  static const QString getOrganisationName() { return "lwo"; }
  static const QString getApplicationName() { return "noch"; }
  static const QString getOrganisationDomain() { return "lwo.org"; }
  static const QString getURLKEY() { return "URLs"; }

  void doDownload(const QUrl &url);
  QString saveFileName(const QUrl &url);
  bool saveToDisk(const QString &filename, QIODevice *data);
  QByteArray calCS(const QString &filename, QCryptographicHash::Algorithm hashAlg);

public slots:
  void on_ClosebuttonBox_clicked(QAbstractButton *button);

  void execute();
  void downloadFinished(QNetworkReply *reply);

private:
  Ui::nochWidget *ui;
};

#endif // NOCHWIDGET_H
