#include <QtQuick>

#include <sailfishapp.h>

#include "searchmodel.h"
#include "commentmodel.h"
#include "extractor.h"
#include "mediainfo.h"
#include "utils.h"
#include "imageprovider.h"
#include "pageref.h"

int main(int argc, char *argv[])
{
  int result;

  QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
  QCoreApplication::setOrganizationName("uk.co.flypig");
  QCoreApplication::setApplicationName("harbour-newpipe");
  QScopedPointer<QQuickView> view(SailfishApp::createView());
  QQmlContext* ctxt = view->rootContext();
  QQmlEngine* engine = view->engine();
  QScopedPointer<SearchModel> searchModel(new SearchModel(app.data()));
  QScopedPointer<Extractor> extractor(new Extractor(searchModel.data(), app.data()));
  Utils::instantiate();

  qmlRegisterType<Extractor>("harbour.newpipe.extractor", 1, 0, "Extractor");
  qmlRegisterType<CommentModel>("harbour.newpipe.extractor", 1, 0, "CommentModel");
  qmlRegisterType<MediaInfo>("harbour.newpipe.extractor", 1, 0, "MediaInfo");
  qmlRegisterType<PageRef>("harbour.newpipe.extractor", 1, 0, "PageRef");
  qmlRegisterType<SearchItem>("harbour.newpipe.extractor", 1, 0, "SearchItem");
  qmlRegisterSingletonType<Utils>("harbour.newpipe.extractor", 1, 0, "Utils", Utils::provider);

  engine->addImageProvider(QLatin1String("newpipe"), new ImageProvider());

  ctxt->setContextProperty("extractor", extractor.data());
  ctxt->setContextProperty("searchModel", searchModel.data());

  view->setSource(SailfishApp::pathTo("qml/harbour-newpipe.qml"));
  view->show();
  result = app->exec();

  return result;
}
