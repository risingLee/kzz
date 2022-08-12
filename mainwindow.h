#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QWindow>
#include "windows.h"
#include "httprequest.h"
#pragma comment(lib, "user32.lib")
namespace Ui {
class MainWindow;
}

struct Bs
{
    bool isChicang;
    double count;
    double price;
    double money;
};

struct Pk
{
    QString symbol;
    QString timestamp;
    double current;
    double bp1;
    double bc1;
    double bp2;
    double bc2;
    double bp3;
    double bc3;
    double bp4;
    double bc4;
    double bp5;
    double bc5;
    double sp1;
    double sc1;
    double sp2;
    double sc2;
    double sp3;
    double sc3;
    double sp4;
    double sc4;
    double sp5;
    double sc5;
    double buypct;
    double sellpct;
    double diff;
    double ratio;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void update();
    void on_pbPankou_clicked();

private:
    Ui::MainWindow *ui;
    HMODULE hDll;
    QString m_title;
    HWND m_hnd;
    HWND m_sxhnd;
    HWND hd = GetDesktopWindow();
    HWND findWinds(HWND hd);
    QTimer * timer;
    HttpRequest req;
    Pk pk;
    Bs bs;
    int bda;
    int mda;
    bool isBuy;
    double initMoney;
    double money;
    qulonglong buytimes;
    double bPrice;
    double sPrice;
    QString m_currentSy;
    void moniBs();
    int bsCal();
    void panKou();
    void showLog();
};

#endif // MAINWINDOW_H
