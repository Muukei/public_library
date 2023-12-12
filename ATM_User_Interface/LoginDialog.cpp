#include "ui_Logindialog.h"
#include "LoginDialog.h"
#include <QAction>
#include <QRegExpValidator>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QHostAddress>
#include <QHostInfo>
#include <QRandomGenerator>
#include <QHBoxLayout>
//Logo图片
QString LOGO = ":/Image/gold.png";
//Login background-image
QString LOGIN_BACKGROUND_IMAGE =":/Image/preview.jpg";
//验证码
QString CODE_NUMBER =NULL;
//最大操作次数
int MAX_OPERATION_NUMBER =3;
//外部变量设置配置文件路径
QString PATH ="./config.ini";

// 用户名设置验证器
QRegExp regExp1("^[a-zA-Z0-9_\u4e00-\u9fa5]*$");
//手机号设置仅支持输入数字
QRegExp regExp2("[0-9]*");
//秘密验证器
QRegExp regExp3("^[a-z0-9A-Z]");
//邮箱格式验证器
QRegExp regExp4("^[a-z0-9A-Z]+[- | a-z0-9A-Z . _]+@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-z]{2,}$");

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent,Qt::WindowCloseButtonHint),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    setWindowTitle("ATM_User_Interface");
    setWindowIcon(QIcon(LOGO));
    InitLoginWidget();

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::InitLoginWidget()
{
    QPixmap pixMap(LOGO);
    ui->LoginIcon->setFixedSize(QSize(100,100));
    pixMap.scaled(ui->LoginIcon->width(),ui->LoginIcon->height());
    ui->LoginIcon->setPixmap(pixMap);
    ui->LoginIcon->setWindowOpacity(0.1);
    ui->LoginIcon->setScaledContents(true);
    ui->RegisterIcon->setPixmap(pixMap);
    ui->RegisterIcon->setWindowOpacity(0.1);
    ui->RegisterIcon->setScaledContents(true);

    ui->ReturnButton->setIcon(QIcon(":/Image/return.png"));
    ui->ReturnButton->setIconSize(QSize(20,20));
    ui->stackedWidget->setCurrentIndex(0);

    window = new MainWindow(this);
    QAction*NameAction = new QAction(ui->NameEdit);
    NameAction->setIconText("account");
    NameAction->setIcon(QIcon(":/Image/user.png"));

    ui->NameEdit->setInputMethodHints(Qt::ImhDigitsOnly);
    ui->NameEdit->setPlaceholderText(tr("用户名"));
    ui->NameEdit->setMaxLength(20);
    ui->NameEdit->addAction(NameAction,QLineEdit::LeadingPosition);

    QPushButton* PasswordButton = new QPushButton(this);
    PasswordButton->setFixedSize(QSize(20,20));
    PasswordButton->setStyleSheet("background:transparent;border:none;");
    PasswordButton->setIcon(QIcon(":/Image/eye-close.png"));
    PasswordButton->setIconSize(QSize(20,20));
    PasswordButton->setFlat(true);
    QHBoxLayout*Layout = new QHBoxLayout(ui->NumEdit);
    Layout->addWidget(PasswordButton,0,Qt::AlignRight|Qt::AlignTop);
    ui->NumEdit->setLayout(Layout);

    connect(PasswordButton,&QPushButton::pressed,[=]
    {ui->NumEdit->setEchoMode(QLineEdit::Normal);
        PasswordButton->setIcon(QIcon(":/Image/eye-open.png"));});

    connect(PasswordButton,&QPushButton::released,[=]
    {ui->NumEdit->setEchoMode(QLineEdit::Password);
        PasswordButton->setIcon(QIcon(":/Image/eye-close.png"));});


    Setwildcard(regExp1,regExp4,regExp2,regExp3);


    QAction*NumAction = new QAction(ui->NumEdit);
    NumAction->setIconText("password");
    NumAction->setIcon(QIcon(":/Image/key.png"));
    ui->NumEdit->setPlaceholderText("密码");
    ui->NumEdit->setMaxLength(20);
    QRegExpValidator* regexValidator = new QRegExpValidator(QRegExp("[0-9]*"), ui->NumEdit);
    ui->NumEdit->setValidator(regexValidator);
    ui->NumEdit->addAction(NumAction,QLineEdit::LeadingPosition);

    Read_Login_Account();
    //启用外部链接后会调整打开外部链接，不启用则会进入槽函数做处理
    ui->Forget->setOpenExternalLinks(true);
    ui->Forget->setTextFormat(Qt::RichText);
    ui->Forget->setText("<a href=\"http://www.example.com\">忘记密码</a>");


    Timer = new QTimer(ui->CodeButton);

    connect(ui->NameEdit,&QLineEdit::textChanged,[=]
    {ui->HintEdit->text().clear();if(ui->NameEdit->text().isEmpty())ui->Login->setEnabled(false);else ui->Login->setEnabled(true);});
    connect(ui->NumEdit,&QLineEdit::textChanged,[=]
    {ui->HintEdit->text().clear();if(ui->NumEdit->text().isEmpty())ui->Login->setEnabled(false);else ui->Login->setEnabled(true);});

    connect(Timer, &QTimer::timeout, this, &LoginDialog::updateButtonText);

    connect(this,&LoginDialog::rejected,this,&LoginDialog::Read_Login_Account);
}

//生成验证码
QString LoginDialog::generateRandomCaptcha()
{
    // 设置随机种子
    qsrand(static_cast<uint>(QTime::currentTime().msec()));

    const int captchaLength = 6;
    QString captchaText;

    for (int i = 0; i < captchaLength; ++i) {
        int digit = qrand() % 10; // 生成0到9的随机数字
        captchaText.append(QString::number(digit));
    }
    CODE_NUMBER = captchaText;
    ui->CodeEdit->setText(CODE_NUMBER);
    emit sendCode(CODE_NUMBER);
    return CODE_NUMBER;
}
//验证码倒计时
void LoginDialog::updateButtonText()
{
    int currentValue = ui->CodeButton->text().toInt();
    if (currentValue > 0) {
        ui->CodeButton->setText(QString::number(currentValue - 1));
    } else {
        // 定时器到达60秒，停止定时器或执行其他操作
        Timer->stop();
        ui->CodeButton->setText("发送验证码");
        ui->CodeButton->setEnabled(true);
    }
}

void LoginDialog::stopupdateButtonText()
{
    Timer->stop();
    ui->CodeButton->setText("发送验证码");
    ui->CodeButton->setEnabled(true);
}

//清理界面
void LoginDialog::clearInterFace()
{
    stopupdateButtonText();
    ui->UserEdit->clear();
    ui->EmailEdit->clear();
    ui->PhoneNumberEdit->clear();
    ui->CodeEdit->clear();
    ui->NewPasswordEdit->clear();
    ui->ApplyPasswordEdit->clear();
}

void LoginDialog::Setwildcard(QRegExp userEditExp, QRegExp emaillExp, QRegExp phoneExp, QRegExp passwordExp)
{
    QRegExpValidator *validator1 = new QRegExpValidator(userEditExp, this);
    ui->NameEdit->setValidator(validator1);

    QRegExpValidator *validator2 = new QRegExpValidator(emaillExp, this);
    ui->EmailEdit->setValidator(validator2);

    QRegExpValidator *validator3 = new QRegExpValidator(phoneExp, this);
    ui->PhoneNumberEdit->setValidator(validator3);

    QRegExpValidator *validator4 = new QRegExpValidator(passwordExp, this);
    ui->NewPasswordEdit->setValidator(validator4);
    ui->ApplyPasswordEdit->setValidator(validator4);
}


//注册表单有效验证
bool LoginDialog::isFormValid()
{

    if (ui->UserEdit->text().isEmpty() || ui->EmailEdit->text().isEmpty() ||ui->PhoneNumberEdit->text().isEmpty() ||
            ui->CodeEdit->text().isEmpty() || ui->NewPasswordEdit->text().isEmpty()|| ui->ApplyPasswordEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"WARNING","Incomplete form information can not register an account!");
        return false;
    }

    return true;
}


//用户名验证
bool LoginDialog::UserNameisValid(const QString &username)
{
    if(username.length()>50)
    {
        QMessageBox::warning(this,"Validation","User name length out of limit!");
        return false;
    } else if(username.isEmpty())
    {
        QMessageBox::warning(this,"Validation","You didn't fill in the username");
        return false;
    }

    return true;

}


//邮箱验证
bool LoginDialog::UserEmailisVaild(const QString &email)
{

    if (!regExp4.exactMatch(email)) {
        QMessageBox::warning(this, "Validation", "Invalid email format.");
        return  false;
    }else if (email.isEmpty()) {
        QMessageBox::warning(this,"Validation","You didn't fill out your email!");
        return false;
    }
    return true;

}


//手机号验证
bool LoginDialog::UserPhoneisVaild(const QString &phone)
{
    if(phone.length()>12)
    {
        QMessageBox::warning(this,"Validation","Phone Number length out of limit.Unrecognized number!");
        return false;
    }else if (phone.isEmpty()) {
        QMessageBox::warning(this,"Validation","You didn't fill out your phone information!");
        return false;
    }

    return true;
}

//密码验证
bool LoginDialog::UserPasswordisVaild(const QString &password,const QString&again)
{
    if(password.length()>20)
    {
        QMessageBox::warning(this,"Validation","Passwoed length out of limit!");
        return false;
    }else if(password!=again)
    {
        QMessageBox::warning(this,"Validation","The two passwords are different!");
        return false;
    }else if (password.isEmpty()||again.isEmpty()) {

        QMessageBox::warning(this,"Validation","You did not confirm setting the password information!");
        return false;
    }
    return true;

}


//验证码验证
bool LoginDialog::validateVerificationCode(const QString &code)
{
    if (CODE_NUMBER != code) {
        QMessageBox::warning(this,"WARNING","The verification code does not match!\n验证码填写错误！");
        return false;
    }else if (code.isEmpty()) {
        QMessageBox::warning(this,"WARNING","You did not fill in the CAPTCHA information！");
        return false;
    }
    return  true;
}


//注册数据写入
void LoginDialog::saveUserAccountData(Account data)
{
    QSettings settings(PATH, QSettings::IniFormat);
    settings.beginGroup(data.userName);
    // 存储数据
    settings.setValue("UserName",data.userName);
    settings.setValue("GroupID", data.groupid);
    settings.setValue("Email", data.email);
    settings.setValue("PhoneNumber", data.phoneNumber);
    settings.setValue("Password", data.password);
    settings.setValue("Balance", data.balance);
    settings.setValue("Currency", data.currency);

    settings.setValue("LoginTime", data.loginTime.toString(Qt::ISODate));
    settings.setValue("Address", data.address);
    settings.endGroup();
}


//读取账户
Account LoginDialog::readUserAccountData(const QString &userName)
{
    Account data;

    QSettings settings(PATH, QSettings::IniFormat);
    if (settings.contains(userName)) {
        settings.beginGroup(userName);

        // 读取数据
        data.userName = settings.value("UserName").toString();;
        data.groupid = settings.value("GroupID").toString();
        data.email = settings.value("Email").toString();
        data.phoneNumber = settings.value("PhoneNumber").toString();
        data.password = settings.value("Password").toString();
        data.balance = settings.value("Balance").toString();
        data.currency = settings.value("Currency").toString();
        data.loginTime = QDateTime::fromString(settings.value("LoginTime").toString(), Qt::ISODate);
        data.address = settings.value("Address").toString();

        settings.endGroup();
    }

    return data;
}

//读取登录账户
void LoginDialog::Read_Login_Account()
{
    QStringList GroupList  = ConfigServicer->getInstance()-> Existed_ID_List();
    for (QString Group:GroupList) {
        if(Group == "LOGO")
        {
            QString username = ConfigServicer->getInstance()->inquireKey(Group,"UserName");
            ui->NameEdit->setText(username);
            QString key = ConfigServicer->getInstance()->inquireKey(Group,"Password");
            ui->NumEdit->setText(key);
            bool state =ConfigServicer->getInstance()->inquireKey(Group,"state").toInt();
            ui->Remmber->setChecked(state);
        }
    }
}

//登录验证(用户与密码校验)确保用户名为唯一标识
bool LoginDialog::is_Login_Ok(const QString &username, const QString &password)
{

    if(username.isEmpty()||password.isEmpty())
    {
        ui->HintEdit->setText("Please enter the login account!");
        return false;
    }
    QStringList GroupList  =ConfigServicer->getInstance()-> Existed_ID_List();
    for (QString Group:GroupList)
    {
         QString key =ConfigServicer->getInstance()->inquireKey(Group,"Password");
        if(Group.contains(username)&&key.contains(password))
            return  true;
    }
    return  false;
}
//记录登录账号
void LoginDialog::Write_Login_Account(const QString &id,const QString &username, const QString &password,int state)
{
    QSettings settings(PATH, QSettings::IniFormat);
    settings.beginGroup("LOGO");
    settings.setValue("GroupId", id);
    settings.setValue("UserName", username);
    settings.setValue("Password", password);
    settings.setValue("state",state);
    settings.endGroup();
}


//检测网络是否连接
bool LoginDialog::isNetworkConnected()
{
    QNetworkAccessManager networkManager;
    QNetworkRequest request(QUrl("http://www.baidu.com"));  // 使用一个可靠的网络地址

    QNetworkReply *reply = networkManager.head(request);
    QCoreApplication::processEvents();  // 处理事件，等待请求完成

    bool connected = false;

    if (reply->error() == QNetworkReply::NoError) {
        // 如果请求成功，表示设备连接到了网络
        connected = true;
        qDebug() << "Network is connected.";
    } else {
        qDebug() << "Network is not connected:" << reply->errorString();
    }

    reply->deleteLater();  // 释放资源

    return connected;
}

//获取网络ip
QString LoginDialog::getCurrentNetworkIp()
{
    QString ipAddress;
    if(!isNetworkConnected())
        return Q_NULLPTR;

    // 获取本地主机信息
    QHostInfo hostInfo = QHostInfo::fromName(QHostInfo::localHostName());

    // 遍历主机的地址列表
    foreach (const QHostAddress &address, hostInfo.addresses()) {
        // 判断地址是否为IPv4
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            // 返回IPv4地址作为QString
            qDebug()<<"get current network IP address"<<address.toString();
            return address.toString();
        }
    }

    // 如果没有找到IPv4地址，返回空字符串
    qDebug() << "Failed to get current network IP address.";
    return Q_NULLPTR;
}

//根据ip查询当前设备的位置
QString LoginDialog::getCurrentDeviceMsg(QString ipAddress)
{

    IPInfoFetcher ipInfoFetcher;

    QString info = ipInfoFetcher.fetchIPInfo(ipAddress);

    if (info.isEmpty()) {
        qDebug() << "Failed to fetch information for IP:" << ipAddress;
        return  Q_NULLPTR;
    } else {
        qDebug() << "IP Information:" << info;
        return  info;
    }
}

void LoginDialog::showEvent(QShowEvent *event)
{
    Read_Login_Account();
    QDialog::showEvent(event);
}


void LoginDialog::on_Forget_linkActivated(const QString &link)
{
    qDebug()<<"启用链接"<<link;
}
//登录操作
void LoginDialog::on_Login_pressed()
{

    // 判断是否上锁，上锁直接退出
    if (ConfigServicer->getInstance()->isUserLocked(ui->NameEdit->text())) {
        qDebug() << "User is locked. Please try again later.";
        return;
    }

    //正确登录直接登录
    qDebug()<<"登录选项操作";
    if(is_Login_Ok(ui->NameEdit->text(),ui->NumEdit->text()))
    {
        connect(this,&LoginDialog::LoginAccount,window,&MainWindow::setUserData);
        QString GroupName = ConfigServicer->getInstance()->inquireKey(ui->NameEdit->text(),"UserName");
        QString GroupId =  ConfigServicer->getInstance()->inquireKey(GroupName,"GroupID");
        QString PassWord = ConfigServicer->getInstance()->inquireKey(GroupName,"Password");
        if(!GroupId.isEmpty()&&!GroupName.isEmpty()&&!PassWord.isEmpty())
            if(GroupName == ui->NameEdit->text()&&PassWord==ui->NumEdit->text())
            {
                hide();
                window->show();
                emit LoginAccount(GroupId,GroupName,PassWord);
                qDebug()<<"登录成功！";
                return;
            }

    }

    //错误计次
    int currentErrorCount = ConfigServicer->getInstance()->getErrorCount(ui->NameEdit->text());
    qDebug()<<currentErrorCount;

    // 登录失败，增加错误次数
    ConfigServicer->getInstance()->incrementErrorCount(ui->NameEdit->text());
    qDebug() << "Login failed. Error count: " << currentErrorCount;
    ui->HintEdit->setText("Login failed. Error count:"+QString::number(currentErrorCount+1));

    // 模拟登录尝试
    if (currentErrorCount >=MAX_OPERATION_NUMBER) {
        ConfigServicer->getInstance()->lockUser(ui->NameEdit->text());
        qDebug() << "User locked. Please try again later.";
        ui->HintEdit->setText("User locked. Please try again later.");
    }
}
//注册选项
void LoginDialog::on_Register_pressed()
{
    qDebug()<<"注册选项操作";
    ui->stackedWidget->setCurrentIndex(1);
}
//返回登录
void LoginDialog::on_ReturnButton_pressed()
{
    ui->stackedWidget->setCurrentIndex(0);
    clearInterFace();
}

//注册操作
void LoginDialog::on_RegisterButton_pressed()
{
    if( isFormValid())
    {

        if(UserNameisValid(ui->UserEdit->text())&&UserEmailisVaild( ui->EmailEdit->text())
                &&UserPhoneisVaild(ui->PhoneNumberEdit->text())&&UserPasswordisVaild(ui->NewPasswordEdit->text(),
                                                                                     ui->ApplyPasswordEdit->text())&&validateVerificationCode(ui->CodeEdit->text()))
        {
            Account Data;
            Data. userName = ui->UserEdit->text();
            Data. email = ui->EmailEdit->text();
            Data. phoneNumber = ui->PhoneNumberEdit->text();
            Data. password = ui->ApplyPasswordEdit->text();
            Data. loginTime = QDateTime::currentDateTime();
            Data. address = getCurrentDeviceMsg(getCurrentNetworkIp());

            //检测所有账户是否存在该用户名
            QStringList Name = ConfigServicer->getInstance()->findGroupsByItemAndValue("UserName", Data. userName);

            //检测邮箱是否存在
            QStringList Email = ConfigServicer->getInstance()->findGroupsByItemAndValue("Email",  Data. email);

            //检测手机号是否存在
            QStringList Phone = ConfigServicer->getInstance()->findGroupsByItemAndValue("PhoneNumber", Data. phoneNumber);

            if(!Name.isEmpty()&&!Email.isEmpty()&&!Phone.isEmpty())
            {
                QMessageBox::warning(this,"WARNING","注册失败！该账户已经存在！");
                stopupdateButtonText();
                return;
            }
            qDebug()<<Name<<Email<<Phone;
            Data.groupid = ConfigServicer->getInstance()->generateUniqueId(ConfigServicer->getInstance()->Existed_ID_List());
            qDebug()<<"Register:"
                   <<"ID编号"   <<  Data.groupid <<"用户名"<<Data. userName
                  <<"邮箱："   << Data. email<<"手机号码："<<Data. phoneNumber
                 <<"密码："   << Data. password <<"余额："<<Data.balance
                <<"操作时间："<<Data.loginTime<<"登录地址："<<Data.address;

            saveUserAccountData(Data);

            //注册信号发送
            emit  registerAccount(Data);

            //加载等待弹窗
            LoadingDialog *loadingDialog =new LoadingDialog;

            connect(loadingDialog,&LoadingDialog::loadfinshed,[=]
            {loadingDialog->close();ui->stackedWidget->setCurrentIndex(0);});
            loadingDialog->exec();
            loadingDialog->deleteLater();
            clearInterFace();
        }

    }

}

//验证码发送接口
void LoginDialog::on_CodeButton_clicked()
{
    Timer->start(1000);
    ui->CodeButton->setEnabled(false);
    generateRandomCaptcha();
    ui->CodeButton->setText(QString::number(60));
}


//记住密码
void LoginDialog::on_Remmber_stateChanged(int arg1)
{
    QStringList GroupIDs = ConfigServicer->getInstance()->findGroupsByItemAndValue("UserName",ui->NameEdit->text());
    if(!GroupIDs.isEmpty())
    {
        for (QString id: GroupIDs) {

            QString key = ConfigServicer->getInstance()->inquireKey(id,"Password");
            if(key==ui->NumEdit->text())
            {
                if(arg1)
                {
                    Write_Login_Account(id,ui->NameEdit->text(),key,arg1);
                    qDebug()<<"记住密码";
                }

                else {
                    ConfigServicer->getInstance()->removeGroup("LoGo");
                    qDebug()<<"取消记录";
                }
            }
        }
    }
}
