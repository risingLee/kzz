#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
class HttpRequest : public QObject
{
    Q_OBJECT
public:
    HttpRequest();
    QByteArray getRequest(QString url);
    QByteArray postRequest(QString url,  QString jsondata);
    QNetworkAccessManager * rmanager;
    QNetworkAccessManager *manager;
    QList<QNetworkCookie> allcookies;
    QNetworkRequest request;
};

#endif // HTTPREQUEST_H
