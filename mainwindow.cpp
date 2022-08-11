#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#define DURATION 9682
#define KUISERI 0.001
#define YINGSERI 0.003
#define MAXCOUNT 500
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_currentSy = "SZ123123";
    isBuy = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1500);
    initMoney = 10000;
}

void MainWindow::update()
{
    panKou();
    //    showLog();
    moniBs();

}

void MainWindow::moniBs()
{
    int ret = bsCal();
    if(isBuy && pk.timestamp.toULongLong()-buytimes>DURATION)
    {
        if( (bPrice - pk.current) / bPrice >  KUISERI && ret != 1)
        {
            // q z s h
            sPrice = pk.current;
            initMoney += sPrice *10;
            isBuy = false;
            qDebug()<<"qzsh==>"<<"total:"<<initMoney<<"sp:"<<sPrice;
            return;
        }
        if( (pk.current - bPrice  ) / bPrice >  YINGSERI)
        {
            // q z z y
            sPrice = pk.current;
            initMoney += sPrice *10;
            isBuy = false;
            qDebug()<<"qzzy==>"<<"total:"<<initMoney<<"sp:"<<sPrice;
            return;
        }
    }

    if (ret == 0)
    {

    }
    else if(ret == 1)
    {
        if(isBuy == false)
        {
            isBuy =true;
            bPrice = pk.current;
            initMoney -= bPrice*10;
            qDebug()<<"buy==>"<<"total:"<<initMoney<<"bp:"<<bPrice;
            buytimes = pk.timestamp.toULongLong();
        }
    }
    else if(ret == 2)
    {
        if(isBuy && pk.timestamp.toULongLong()-buytimes>DURATION)
        {
            if( (pk.current - bPrice  ) / bPrice >=  0)
            {
                // q z z y
                sPrice = pk.current;
                initMoney += sPrice *10;
                isBuy = false;
                qDebug()<<"wxyj==>"<<"total:"<<initMoney<<"sp:"<<sPrice;
                return;
            }
        }
    }

}

int MainWindow::bsCal()
{
    double bcsum = pk.bc1+pk.bc2+pk.bc3+pk.bc4+pk.bc5;
    double scsum = pk.sc1+pk.sc2+pk.sc3+pk.sc4+pk.sc5;
    QList<double> lstbc;
    lstbc<<pk.bc1<<pk.bc2<<pk.bc3<<pk.bc4<<pk.bc5;
    QList<double> lstsc;
    lstsc<<pk.sc1<<pk.sc2<<pk.sc3<<pk.sc4<<pk.sc5;
    bool smax = false;
    bool bmax = false;
    for(int i = 0; i < lstbc.count(); ++i)
    {
        if (lstbc[i] >= MAXCOUNT + 300)
        {
            bmax = true;
            break;
        }
    }

    for(int i = 0; i < lstsc.count(); ++i)
    {
        if (lstsc[i] >= MAXCOUNT)
        {
            smax = true;
            break;
        }
    }

    if( smax && ! bmax)
        return 1;
    else if(bmax && !smax)
        return 2;
    else
        return 0;
    //    qDebug()<<bcsum<<scsum<<bcsum-scsum;
}

void MainWindow::showLog()
{
    QDateTime time = QDateTime::fromMSecsSinceEpoch(pk.timestamp.toULongLong());
    //    qDebug()<<pk.symbol<<time;
    //    qDebug()<<pk.current;
    qDebug()<<"====B5====";
    qDebug()<<"S5"<<pk.sp5<<pk.sc5;
    qDebug()<<"S4"<<pk.sp4<<pk.sc4;
    qDebug()<<"S3"<<pk.sp3<<pk.sc3;
    qDebug()<<"S2"<<pk.sp2<<pk.sc2;
    qDebug()<<"S1"<<pk.sp1<<pk.sc1;
    qDebug()<<"====S5====";
    qDebug()<<"B1"<<pk.bp1<<pk.bc1;
    qDebug()<<"B2"<<pk.bp2<<pk.bc2;
    qDebug()<<"B3"<<pk.bp3<<pk.bc3;
    qDebug()<<"B4"<<pk.bp4<<pk.bc4;
    qDebug()<<"B5"<<pk.bp5<<pk.bc5;
    //    qDebug()<<"buypct:"<<pk.buypct<<"sellpct:"<<pk.sellpct<<"diff:"<<pk.diff<<"ratio:"<<pk.ratio;
}
void MainWindow::panKou()
{
    QByteArray bytes = req.getRequest("https://stock.xueqiu.com/v5/stock/realtime/pankou.json?symbol="+m_currentSy);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes);
    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject dataObj = jsonObj.value("data").toObject();
    pk.symbol = dataObj.value("symbol").toString();
    pk.timestamp = dataObj.value("timestamp").toVariant().toString();
    pk.current = dataObj.value("current").toDouble();
    pk.bp1 = dataObj.value("bp1").toDouble();
    pk.bc1 = dataObj.value("bc1").toDouble();
    pk.bp2 = dataObj.value("bp2").toDouble();
    pk.bc2 = dataObj.value("bc2").toDouble();
    pk.bp3 = dataObj.value("bp3").toDouble();
    pk.bc3 = dataObj.value("bc3").toDouble();
    pk.bp4 = dataObj.value("bp4").toDouble();
    pk.bc4 = dataObj.value("bc4").toDouble();
    pk.bp5 = dataObj.value("bp5").toDouble();
    pk.bc5 = dataObj.value("bc5").toDouble();
    pk.sp1 = dataObj.value("sp1").toDouble();
    pk.sc1 = dataObj.value("sc1").toDouble();
    pk.sp2 = dataObj.value("sp2").toDouble();
    pk.sc2 = dataObj.value("sc2").toDouble();
    pk.sp3 = dataObj.value("sp3").toDouble();
    pk.sc3 = dataObj.value("sc3").toDouble();
    pk.sp4 = dataObj.value("sp4").toDouble();
    pk.sc4 = dataObj.value("sc4").toDouble();
    pk.sp5 = dataObj.value("sp5").toDouble();
    pk.sc5 = dataObj.value("sc5").toDouble();
    pk.buypct = dataObj.value("buypct").toDouble();
    pk.sellpct = dataObj.value("sellpct").toDouble();
    pk.diff = dataObj.value("diff").toDouble();
    pk.ratio = dataObj.value("ratio").toDouble();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbPankou_clicked()
{
    panKou();
    showLog();
    bsCal();
}
