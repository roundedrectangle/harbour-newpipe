#include <QtQuick>

#include <sailfishapp.h>

#include "extractor.h"

int main(int argc, char *argv[])
{
  int result;
  // SailfishApp::main() will display "qml/harbour-newpipe.qml", if you need more
  // control over initialization, you can use:
  //
  //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
  //   - SailfishApp::createView() to get a new QQuickView * instance
  //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
  //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
  //
  // To display the view, call "show()" (will show fullscreen on device).

  QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
  QCoreApplication::setOrganizationName("uk.co.flypig");
  QCoreApplication::setApplicationName("harbour-newpipe");
  QScopedPointer<Extractor> extractor(new Extractor());
  QScopedPointer<QQuickView> view(SailfishApp::createView());
  QQmlContext *ctxt = view->rootContext();

  ctxt->setContextProperty("extractor", extractor.data());

  view->setSource(SailfishApp::pathTo("qml/harbour-newpipe.qml"));
  view->show();
  result = app->exec();

  return result;
}
