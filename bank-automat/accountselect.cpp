#include "accountselect.h"
#include "ui_accountselect.h"
#include "usermenu.h"

AccountSelect::AccountSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountSelect)
{
    ui->setupUi(this);

    qDebug() << "AccountSelect created";

    connect(this, SIGNAL(accountIDsChangedSignal()),
            this,SLOT(handleCardTypeRequest()));
}

AccountSelect::~AccountSelect()
{
    delete ui;
}

void AccountSelect::setWebToken(const QByteArray &newWebToken)
{
    webToken = newWebToken;
    qDebug() << "AccountSelect WebToken set";
}

void AccountSelect::setAccountIDs(const QJsonArray &newAccountIDs)
{
    accountIDs = newAccountIDs;
    qDebug() << "AccountSelect AccountIDs set: " << accountIDs;

    emit accountIDsChangedSignal();
}

//Request for getting account data
void AccountSelect::handleCardTypeRequest()
{
    //Loop for accounts in QArray
    for (const QJsonValue &value : accountIDs) {
        if (value.isObject()) {
            QJsonObject jsonObject = value.toObject();
            QString id_account = jsonObject["id_account"].toString();
            QString site_url=Environment::getBaseUrl()+"/bankaccount/"+id_account;
            QNetworkRequest request((site_url));
            QByteArray myToken="Bearer "+webToken;
            request.setRawHeader(QByteArray("Authorization"), myToken);

            accountManager = new QNetworkAccessManager(this);
            connect(accountManager, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(setAccountsSlot(QNetworkReply*)));
            reply = accountManager->get(request);
        }
        else {
            qDebug() << "Error in received data";
        }
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, &QEventLoop::quit);
        loop.exec(); // This will pause the execution until the event loop is quit
    }
}

//Read
void AccountSelect::setAccountsSlot(QNetworkReply *reply)
{
    //Read response and convert datatype
    response_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject jsonObj = json_doc.object();
    QString accountID = jsonObj["id_account"].toString();
    QString limit = jsonObj.value("credit_limit").toString();

    //Handle account set by credit limit
    if (!jsonObj.isEmpty()) {
        if (limit != "0.00") {
            setCreditAccount(accountID);
        }
        else {
            setDebitAccount(accountID);
        }
    }
    else {
        qDebug() << "Problem with handling account data";
    }
    reply->deleteLater();
    accountManager->deleteLater();
}

void AccountSelect::setCreditAccount(const QString &newCreditAccount)
{
    creditAccount = newCreditAccount;
    qDebug() << "creditAccount set: " << creditAccount;
}

void AccountSelect::setDebitAccount(const QString &newDebitAccount)
{
    debitAccount = newDebitAccount;
    qDebug() << "debitAccount set: " << debitAccount;
}

void AccountSelect::on_btnDebit_clicked()
{
    qDebug() << "Account ID: " << debitAccount;
    UserMenu *userMenuPtr = new UserMenu(this);
    userMenuPtr->open();
    userMenuPtr->setWebToken(webToken);
    userMenuPtr->setIdAccount(debitAccount);
}


void AccountSelect::on_btnCredit_clicked()
{
    qDebug() << "Account ID: " << creditAccount;
    UserMenu *userMenuPtr = new UserMenu(this);
    userMenuPtr->open();
    userMenuPtr->setWebToken(webToken);
    userMenuPtr->setIdAccount(creditAccount);
}


void AccountSelect::on_btnLogout_clicked()
{
    emit logoutSignal();
    delete this;
}

