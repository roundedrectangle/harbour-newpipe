#include <QtQuick>

#include <sailfishapp.h>

#include "searchmodel.h"
#include "extractor.h"

int main(int argc, char *argv[])
{
  int result;

  QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
  QCoreApplication::setOrganizationName("uk.co.flypig");
  QCoreApplication::setApplicationName("harbour-newpipe");
  QScopedPointer<SearchModel> searchModel(new SearchModel(app.data()));
  QScopedPointer<Extractor> extractor(new Extractor(searchModel.data(), app.data()));
  QScopedPointer<QQuickView> view(SailfishApp::createView());
  QQmlContext *ctxt = view->rootContext();

  ctxt->setContextProperty("extractor", extractor.data());
  ctxt->setContextProperty("searchModel", searchModel.data());

  view->setSource(SailfishApp::pathTo("qml/harbour-newpipe.qml"));
  view->show();
  result = app->exec();

  return result;
}
