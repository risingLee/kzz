#include "httprequest.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QTextCodec>


HttpRequest::HttpRequest()
{
    rmanager = new QNetworkAccessManager();
    QNetworkRequest req;
    req.setUrl(QUrl("https://xueqiu.com/"));
    QNetworkAccessManager nManager;
    QNetworkReply* getreply=nManager.get(req);
    QEventLoop loop;
    //下载完成后，直接退出子进程
    connect(getreply, SIGNAL(finished()), &loop, SLOT(quit()));
    //子进程开始运行
    loop.exec();
    allcookies=nManager.cookieJar()->cookiesForUrl(QUrl("https://xueqiu.com/"));
}

QByteArray HttpRequest::getRequest(QString url)
{
    QNetworkRequest request;
    QNetworkAccessManager *naManager=new QNetworkAccessManager();
    QUrl strUrl = url;
    QVariant var;
    var.setValue(allcookies);
    request.setHeader(QNetworkRequest::CookieHeader,var);
    request.setUrl(strUrl);
    request.setRawHeader("Content-Type", "charset='utf-8'");
    request.setRawHeader("Content-Type", "application/json");

    QNetworkReply* reply = naManager->get(request);

    QEventLoop eventLoop;
    connect(naManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    //    QTextCodec* codec = QTextCodec::codecForName("utf8");
    //    QString strReply = codec->toUnicode(reply->readAll());
    QByteArray data = reply->readAll();
    reply->deleteLater();
    return data;
}

QByteArray HttpRequest::postRequest(QString url, QString jsondata)
{
    QByteArray array= jsondata.toLatin1();
    QNetworkRequest request;
    QNetworkAccessManager *naManager=new QNetworkAccessManager();
    QVariant var;
    var.setValue(allcookies);
    request.setHeader(QNetworkRequest::CookieHeader,var);
    QUrl strUrl = url;
    request.setUrl(strUrl);
    request.setRawHeader("Content-Type", "charset='utf-8'");
    request.setRawHeader("Content-Type", "application/json");

    QNetworkReply* reply = naManager->post(request, array);
    QEventLoop eventLoop;
    connect(naManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    //    QTextCodec* codec = QTextCodec::codecForName("utf8");
    //    QString strReply = codec->toUnicode(reply->readAll());
    QByteArray data = reply->readAll();
    reply->deleteLater();
    return data;
}
