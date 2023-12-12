#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QSettings>
#include <QTimer>
#include <QTextStream>
#include <mainwindow.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include <LoadingDialog.h>
#include <ConfigurationService.h>
//#pragma execution_character_set("utf-8")


namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    //初始化登录界面
    void InitLoginWidget();

    //验证码生成
    QString generateRandomCaptcha();

    //倒计时更新按钮文本
    void updateButtonText();

    //结束验证码计数
    void stopupdateButtonText();

    //清除界面数据
    void clearInterFace();

    //通配符设置
    void  Setwildcard(QRegExp userEditExp=QRegExp(),QRegExp emaillExp=QRegExp()
            ,QRegExp phoneExp=QRegExp(),QRegExp passwordExp=QRegExp());

    //注册表单有效验证
    virtual  bool isFormValid();

    //用户名验证
    virtual bool UserNameisValid(const QString&username);

    //邮箱格式验证
    virtual bool UserEmailisVaild(const QString&email);

    //手机号验证
    virtual bool UserPhoneisVaild(const QString &phone);

    //密码验证
    virtual bool UserPasswordisVaild(const QString &password,const QString&again);

    //验证码验证
    virtual bool validateVerificationCode(const QString &code);


    //保存账户信息
    virtual void saveUserAccountData(Account data);

    //读取账户
    virtual Account readUserAccountData(const QString &userName);

    //读取记录账户
    void Read_Login_Account();

    //记录登录账号写入
    void Write_Login_Account(const QString &id,const QString& username,const QString&password,int state);

    //检测登录条件是否满足
    virtual bool is_Login_Ok(const QString &username,  const QString &password);


    //检测网络连接
    bool isNetworkConnected();
    //获取ip
    QString getCurrentNetworkIp();
    //ip查询地址
    QString getCurrentDeviceMsg(QString ipAddress);

protected:

    void showEvent(QShowEvent *) override;

signals:

    //发送验证码信号
    void sendCode(const QString&);

    //登录账户
     void LoginAccount(const QString &id, const QString &username, const QString &password);

    //注册账户
    void registerAccount(Account data);

private slots:

    //忘记密码链接
    void on_Forget_linkActivated(const QString &link);

    //登录操作
    void on_Login_pressed();

    //注册选项
    void on_Register_pressed();

    //返回登录
    void on_ReturnButton_pressed();

    //注册账户操作
    void on_RegisterButton_pressed();

    //验证码发送
    void on_CodeButton_clicked();

    //记住登录
    void on_Remmber_stateChanged(int arg1);

private:
    Ui::LoginDialog *ui;
    MainWindow * window;
    ConfigurationService* ConfigServicer;
    //验证码倒计时 ，定时改变文本
    QTimer*Timer;
};

//ip查询地区类
class IPInfoFetcher : public QObject
{
    Q_OBJECT

public:
    IPInfoFetcher(QObject *parent = nullptr) : QObject(parent), manager(new QNetworkAccessManager(this)) {
        connect(manager, &QNetworkAccessManager::finished, this, &IPInfoFetcher::onReplyFinished);
    }

    QString fetchIPInfo(const QString &ipAddress) {
        QString apiUrl = "http://ip-api.com/json/" + ipAddress;
        QUrl URL = apiUrl;
        QNetworkRequest request(URL);

        QEventLoop eventLoop;

        connect(this, &IPInfoFetcher::ipInfoFetched, &eventLoop, &QEventLoop::quit);

        manager->get(request);

        // Run the event loop until the information is fetched
        QTimer::singleShot(10000, &eventLoop, &QEventLoop::quit);  // Set a timeout of 10 seconds
        eventLoop.exec();

        return fetchedInfo.isEmpty() ? QString() : fetchedInfo;
    }

signals:
    void ipInfoFetched(const QString &info);

private slots:
    void onReplyFinished(QNetworkReply *reply) {
        QString info;

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);

            if (jsonDocument.isObject()) {
                QJsonObject jsonObject = jsonDocument.object();

                if (jsonObject.contains("city") && jsonObject.contains("country")) {
                    info = QString("City: %1, Country: %2").arg(jsonObject["city"].toString(), jsonObject["country"].toString());
                }
            }
        } else {
            qDebug() << "Error:" << reply->errorString();
        }

        reply->deleteLater();
        fetchedInfo = info;
        emit ipInfoFetched(info);
    }

private:
    QNetworkAccessManager *manager;
    QString fetchedInfo;
};

#endif // LOGINDIALOG_H
