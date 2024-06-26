#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "usermenu.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    RFID_dll = new RFIDReader(this);

    //Connect closeSerialSignal to RFIDReader
    connect(this, SIGNAL(closeSerialSignal()),
            RFID_dll, SLOT(handleCloseSerial()));

    //Connect openSerialSignal to RFIDReader
    connect(this, SIGNAL(openSerialSignal()),
            RFID_dll, SLOT(handleOpenSerial()));

    QString url = Environment::getBaseUrl();
    LoginHandler_dll = new LoginHandler(url, this);

    //Connection for receiving response from LoginHandler
    connect(LoginHandler_dll, SIGNAL(responseSignal(QByteArray)),
            this,SLOT(handleLoginResponse(QByteArray)));

    //Connection for sending users login parameters to LoginHandler library
    connect(this, SIGNAL(loginSignal(QString,QString)),
            LoginHandler_dll, SLOT(handleLogin(QString,QString)));

    // these 2 lines are for bypassing rfid reader in case you don't have it for testing, change test_id value to the id_card you want
    //QString test_id = "0B00320D2B"; // debit only card
    //QString test_id = "0600062E1F"; // credit only card
    //QString test_id = "06000D5C69"; // combination card
    //setSerialID(test_id);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSerialID(QString data)
{
    emit closeSerialSignal();

    serialID = data;

    PinUI_dll = new PinUI(this);
    PinUI_dll->open();

    connect(PinUI_dll, SIGNAL(pincodeSignal(QString)),
            this, SLOT(setPincode(QString)));
}

void MainWindow::setPincode(QString data)
{
    pincode = data;

    //Emit signal with parameters to loginHandler
    emit loginSignal(serialID, pincode);
}

void MainWindow::handleLoginResponse(QByteArray response)
{
    //Note for below: error -61 is macos equivalent of widows error -4078
    if (response == "-61" || response == "-4078" || response.length()==0){

        qDebug() << "Database connection problem";

        QString msg = "Problem with connection";
        emit loginResponseTextSignal(msg);
    }
    else{
        //If login success, token is returned
        if(response!="false"){
            qDebug() << "Login successful";
            qDebug() << response;
            QString msg = "Login Successful";
            emit loginResponseTextSignal(msg);

            delete PinUI_dll;
            this->setWebToken(response);
            //Request for checking is there 1 or 2 accounts in card
            QString site_url=Environment::getBaseUrl()+"/accountsincard/"+serialID;
            QNetworkRequest request((site_url));
            QByteArray myToken="Bearer "+webToken;
            request.setRawHeader(QByteArray("Authorization"), myToken);

            checkAccountsManager = new QNetworkAccessManager(this);
            connect(checkAccountsManager, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(checkAccountsSlot(QNetworkReply*)));

            reply = checkAccountsManager->get(request);
        }
        //If login fails
        else{
            qDebug() << "Card ID or Pincode doesn't match";
            qDebug() << response;

            QString msg = "Incorrect, Enter Pin Code";
            emit loginResponseTextSignal(msg);
        }
    }
}

void MainWindow::checkAccountsSlot(QNetworkReply *reply)
{
    response_data=reply->readAll();

    qDebug() << "Account data received: " << response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray jsonArray = json_doc.array();

    int objectCount = jsonArray.size();

    /*Check number of accounts in card
    If one account -> UserMenu
    If 2 accounts -> AccountSelect*/
    qDebug() << "Number of accounts in card: " << objectCount;
    if(objectCount > 1) {
        AccountSelect *accountSelectPtr = new AccountSelect(this);
        accountSelectPtr->setWebToken(webToken);
        accountSelectPtr->setAccountIDs(jsonArray);
        accountSelectPtr->open();
        connect(accountSelectPtr, SIGNAL(logoutSignal()),
                this, SLOT(handleLogoutSlot()));
    }
    else{

        //Receive info as index within jsonarray
        qDebug() << "One account found: " << jsonArray[0];

        //Pluck out jsonobject from array
        QJsonObject jsonObj = jsonArray[0].toObject();
        qDebug() << jsonObj;

        //Convert to qstring
        QString accountID = jsonObj["id_account"].toString();
        qDebug() << "Account number in QString: " << accountID;

        UserMenu *userMenuPtr = new UserMenu(this);
        userMenuPtr->open();
        userMenuPtr->setWebToken(webToken);
        userMenuPtr->setIdAccount(accountID);

        connect(userMenuPtr, SIGNAL(logoutSignal()),
                this, SLOT(handleLogoutSlot()));
    }

    reply->deleteLater();
    checkAccountsManager->deleteLater();
}

void MainWindow::handleLogoutSlot()
{
    qDebug() << "Handling logout...";
    setWebToken(nullptr);
    emit openSerialSignal();
}

void MainWindow::setWebToken(const QByteArray &newWebToken)
{
    webToken = newWebToken;
}
