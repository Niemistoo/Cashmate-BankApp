#ifndef WITHDRAWAL_H
#define WITHDRAWAL_H

#include <QDialog>
#include "environment.h"
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

namespace Ui {
class Withdrawal;
}

class Withdrawal : public QDialog
{
    Q_OBJECT

public:
    explicit Withdrawal(QWidget *parent = nullptr);
    ~Withdrawal();

    void setIdAccount(const QString &newIdAccount);


    void setWebToken(const QByteArray &newWebToken);

private slots:
    void on_btn20_clicked();
    void on_btn40_clicked();
    void on_btn50_clicked();
    void on_btn80_clicked();
    void on_btn100_clicked();
    void on_btnOther_clicked();
    void on_btnReturn_clicked();
    void on_btnLogOut_clicked();

    void withdrawalSlot(QNetworkReply *reply);
private:
    QString idAccount;
    QByteArray webToken;
    QNetworkReply *reply;
    QByteArray response_data;
    float amount;
    QNetworkAccessManager *withdrawalManager;
    Ui::Withdrawal *ui;
    void handleAmount(float);
};

#endif // WITHDRAWAL_H
