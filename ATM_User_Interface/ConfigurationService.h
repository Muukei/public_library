#ifndef CONFIGURATIONSERVICE_H
#define CONFIGURATIONSERVICE_H

#include <QObject>
#include <QDateTime>
#include <UserDefaultDataType.h>
class ConfigurationService :public QObject
{
    Q_OBJECT
public:

   ~ ConfigurationService();

    //单例
     static  ConfigurationService *getInstance();

    //查询账户是否存在
    bool isGroupExists(const QString &UserName);

    //查询某个组中某项数据是否为某值
    bool isItemWithValue(const QString &UserName, const QString &key, const QString &expectedValue);

    //获取所有账户
    const QStringList Existed_ID_List();

    //返回所有为该用户名的账户
    QList<Account> GroupName_isExisted(const QString &userName);

    //查询账户中的某个属性的属性值
    QString inquireKey(const QString&UserName,const QString&key);

    //在所有账户中根据属性查找值
    QMap<QString, QString> getAllGroupsByItem(const QString &key);

    //根据属性和属性值查找账户
    QString findGroupByItemAndValue(const QString &key, const QString &value);

    //查询所有账户中某个属性的属性值
    QStringList findGroupsByItemAndValue(const QString &key, const QString &expectedValue);

    //根据账户名和id确定唯一账户，并将数据返回
    Account readUserAccountData(const QString &userName, const QString &groupId);


    //随机生成唯一编号ID
    QString generateUniqueId(const QStringList &existingIds, int length = 10);

    //记录错误操作
    void incrementErrorCount(const QString &username);

    //获取错误次数
    int getErrorCount(const QString &username);

    //设置上锁状态
    void lockUser(const QString &username);

    //设置解锁状态
    void unlockUser(const QString &username);

    //判断上锁状态
    bool isUserLocked(const QString &username);

    //删除某一个组
    void removeGroup(const QString &groupId);

    //删除某一个组中的某个元素
    void removeItemInGroup(const QString &groupId, const QString &key);

    //删除所有组中的某个元素
    void removeItemInAllGroups(const QString &key);

    //清空操作
    void clearAllGroups();

    //在某个组中插入某个元素
    void insertItemInGroup(const QString &groupName, const QString &key, const QVariant &value);

    //在所有组中插入某个元素
    void insertItemInAllGroups(const QString &key, const QVariant &value);

private:

    static ConfigurationService*configure;
    ConfigurationService(){};

};

#endif // CONFIGURATIONSERVICE_H
