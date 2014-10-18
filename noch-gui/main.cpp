#include "nochwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  a.setOrganizationName( nochWidget::getOrganisationName() );
  a.setOrganizationDomain( nochWidget::getOrganisationDomain() );
  a.setApplicationName( nochWidget::getApplicationName() );

  nochWidget w;
  w.show();
  w.execute();
  return a.exec();
}
