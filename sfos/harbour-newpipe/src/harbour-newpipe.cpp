#include <QtQuick>

#include <sailfishapp.h>

#include "searchmodel.h"
#include "commentmodel.h"
#include "extractor.h"
#include "mediainfo.h"
#include "utils.h"
#include "imageprovider.h"

int main(int argc, char *argv[])
{
  int result;

  QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
  QCoreApplication::setOrganizationName("uk.co.flypig");
  QCoreApplication::setApplicationName("harbour-newpipe");
  QScopedPointer<SearchModel> searchModel(new SearchModel(app.data()));
  QScopedPointer<CommentModel> commentModel(new CommentModel(app.data()));
  QScopedPointer<Extractor> extractor(new Extractor(searchModel.data(), commentModel.data(), app.data()));
  QScopedPointer<QQuickView> view(SailfishApp::createView());
  QQmlContext *ctxt = view->rootContext();
  Utils::instantiate();

  qmlRegisterType<Extractor>("harbour.newpipe.extractor", 1, 0, "Extractor");
  qmlRegisterType<MediaInfo>("harbour.newpipe.extractor", 1, 0, "MediaInfo");
  qmlRegisterSingletonType<Utils>("harbour.newpipe.extractor", 1, 0, "Utils", Utils::provider);

  view->engine()->addImageProvider(QLatin1String("newpipe"), new ImageProvider());

  ctxt->setContextProperty("extractor", extractor.data());
  ctxt->setContextProperty("searchModel", searchModel.data());
  ctxt->setContextProperty("commentModel", commentModel.data());

  view->setSource(SailfishApp::pathTo("qml/harbour-newpipe.qml"));
  view->show();
  result = app->exec();

  return result;
}
