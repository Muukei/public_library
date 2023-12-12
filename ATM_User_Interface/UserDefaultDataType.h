#ifndef USERDEFAULTDATATYPE_H
#define USERDEFAULTDATATYPE_H

#endif // USERDEFAULTDATATYPE_H

#include <QString>
#include <QDateTime>
//配置文件生成或加载路径
extern QString PATH;

struct Account
{
    QString     userName;           //组名
    QString     groupid;            //编号
    QString     email;              //邮箱
    QString     phoneNumber;        //手机号
    QString     password;           //密码

    QString     balance;            //余额
    QString     currency="Dollars"; //币种
    QDateTime   loginTime;          //登录时间
    QString     address =" ";       //登录地址
};

