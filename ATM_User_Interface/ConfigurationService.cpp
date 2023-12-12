#include "ConfigurationService.h"
#include <QDebug>
#include <QSettings>
#include <QRandomGenerator>
//锁定时间24小时
const int LOCK_DURATION_HOURS = 24;

ConfigurationService*ConfigurationService ::configure =Q_NULLPTR;

ConfigurationService::~ConfigurationService()
{
    configure->deleteLater();
}

ConfigurationService *ConfigurationService::getInstance()
{
    if(configure == Q_NULLPTR)
    {
        configure =  new ConfigurationService;
    }
    return configure;
}


//查询某个账户是否存在
bool ConfigurationService::isGroupExists(const QString &UserName)
{
    // 指定操作文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    // 获取所有的组
    QStringList allGroups = settings.childGroups();

    // 检查特定的组名是否在组列表中
    return allGroups.contains(UserName);
}

bool ConfigurationService::isItemWithValue(const QString &UserName, const QString &key, const QString &expectedValue)
{
    // 指定操作文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    // 开始指定的组
    settings.beginGroup(UserName);

    // 获取特定键对应的值
    QString actualValue = settings.value(key).toString();

    // 结束指定的组
    settings.endGroup();

    // 检查值是否等于期望的值
    return (actualValue == expectedValue);
}

//返回所有账户列表
const QStringList ConfigurationService::Existed_ID_List()
{
    QSettings settings(PATH, QSettings::IniFormat);
    return settings.childGroups();
}

QList<Account> ConfigurationService::GroupName_isExisted(const QString &userName)
{
    QList<Account> result;

    // 创建 QSettings 实例，指定配置文件路径
    QSettings settings(PATH, QSettings::IniFormat);

    // 获取所有组的名称
    QStringList allGroups = settings.childGroups();
    qDebug()<<"All UserName"<<allGroups;

    // 遍历所有组
    for (const QString &groupName : allGroups)
    {
        settings.beginGroup(groupName);
        // 检查是否包含指定用户名
        if (groupName==userName && settings.value("UserName").toString() == userName)
        {
             QString AccoutID = inquireKey(groupName,"GroupID");
             Account account = readUserAccountData(userName,AccoutID);

            result.append(account);
        }

        settings.endGroup();
    }

    return result;
}

//在账户中查找某个数据项
QString ConfigurationService::inquireKey(const QString &UserName, const QString &key)
{
    QSettings settings(PATH, QSettings::IniFormat);
    settings.beginGroup(UserName);
    QString value = settings.value(key).toString();
    settings.endGroup();
    return  value;
}


//返回所有账户中的某项数据
QMap<QString, QString> ConfigurationService::getAllGroupsByItem(const QString &key)
{
    QMap<QString, QString> groupsWithItem;

    // 指定操作文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    // 获取所有的账户
    QStringList allGroups = settings.childGroups();

    // 遍历所有账户
    for (const QString &groupName : allGroups) {
        // 开始当前账户
        settings.beginGroup(groupName);

        // 检查当前组是否包含指定的键
        if (settings.contains(key)) {
            // 将组名和对应的值添加到 QMap 中
            groupsWithItem.insert(groupName, settings.value(key).toString());
        }

        // 结束当前组
        settings.endGroup();
    }

    return groupsWithItem;
}

QString ConfigurationService::findGroupByItemAndValue(const QString &key, const QString &value)
{
    // 指定操作文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    // 获取所有的组
    QStringList allGroups = settings.childGroups();

    // 遍历所有组
    for (const QString &groupName : allGroups) {
        // 开始当前组
        settings.beginGroup(groupName);

        // 检查当前组是否包含指定的键，并且值匹配
        if (settings.contains(key) && settings.value(key).toString() == value) {
            // 结束当前组
            settings.endGroup();
            return groupName;
        }

        // 结束当前组
        settings.endGroup();
    }

    // 如果未找到，返回空字符串表示未找到
    return Q_NULLPTR;
}

//在所有账户中筛选某项满足特定数据的账户，返回账户列表
QStringList ConfigurationService::findGroupsByItemAndValue(const QString &key, const QString &expectedValue)
{
    QStringList matchingGroups;

    // 指定操作文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    // 获取所有的组
    QStringList allGroups = settings.childGroups();

    // 遍历所有组
    for (const QString &groupName : allGroups) {
        // 开始当前组
        settings.beginGroup(groupName);

        // 获取特定键对应的值
        QString actualValue = settings.value(key).toString();

        // 结束当前组
        settings.endGroup();

        // 检查值是否等于期望的值
        if (actualValue == expectedValue) {
            matchingGroups.append(groupName);
        }
    }

    return matchingGroups;
}

Account ConfigurationService::readUserAccountData(const QString &userName, const QString &groupId)
{
    Account data;
    QSettings settings(PATH, QSettings::IniFormat);
    QStringList allGroups = settings.childGroups();

    // 遍历所有组，查找符合用户名和组ID的组
    foreach (const QString &groupName, allGroups)
    {
        settings.beginGroup(groupName);

        // 判断用户名和组ID是否匹配
        if (settings.value("UserName").toString() == userName && settings.value("GroupID").toString() == groupId)
        {
            // 读取该组的数据

            data.userName = settings.value("UserName").toString();
            data.groupid = settings.value("GroupID").toString();
            data.email = settings.value("Email").toString();
            data.phoneNumber = settings.value("PhoneNumber").toString();
            data.password = settings.value("Password").toString();
            data.balance = settings.value("Balance").toString();
            data.currency = settings.value("Currency").toString();
            data.loginTime = QDateTime::fromString(settings.value("LoginTime").toString(), Qt::ISODate);
            data.address = settings.value("Address").toString();
        }
        settings.endGroup();
    }

    return data;
}


//生成唯一id
QString ConfigurationService::generateUniqueId(const QStringList &existingIds, int length)
{

    while (true) {
        // 生成一个新的数字字符串
        QString newId;
        for (int i = 0; i < length; ++i) {
            int digit = QRandomGenerator::global()->bounded(10); // 生成 0 到 9 之间的随机数字
            newId.append(QString::number(digit));
        }

        // 检查新生成的数字字符串是否在现有列表中存在
        if (!existingIds.contains(newId)) {
            // 如果不存在，则返回该数字字符串
            return newId;
        }
    }
}

//记录错误操作
void ConfigurationService::incrementErrorCount(const QString &username)
{
    QSettings settings(PATH, QSettings::IniFormat);

    // 获取当前错误次数
    int errorCount = settings.value(username + "_errorCount").toInt();

    // 增加错误次数
    errorCount++;

    // 保存新的错误次数
    settings.setValue(username + "_errorCount", errorCount);
}

//获取错误操作次数
int ConfigurationService::getErrorCount(const QString &username)
{
    QSettings settings(PATH, QSettings::IniFormat);
    return settings.value(username + "_errorCount").toInt();
}

//设置上锁状态
void ConfigurationService::lockUser(const QString &username)
{
    QSettings settings(PATH, QSettings::IniFormat);

    // 保存锁定状态和锁定时间
    settings.setValue(username + "_locked", true);
    settings.setValue(username + "_lockTime", QDateTime::currentDateTime());
}


//设置解锁状态
void ConfigurationService::unlockUser(const QString &username)
{
    QSettings settings(PATH, QSettings::IniFormat);

    // 解锁用户
    settings.setValue(username + "_locked", false);
    settings.remove(username + "_errorCount");
}


//判断是否上锁
bool ConfigurationService::isUserLocked(const QString &username)
{
    QSettings settings(PATH, QSettings::IniFormat);

    // 获取锁定状态
    bool locked = settings.value(username + "_locked", false).toBool();

    if (locked) {
        // 获取锁定时间
        QDateTime lockTime = settings.value(username + "_lockTime").toDateTime();

        // 计算锁定时长
        int lockDurationHours = lockTime.secsTo(QDateTime::currentDateTime()) / 3600;

        // 如果锁定时间超过设定的锁定时长，解锁用户
        if (lockDurationHours >= LOCK_DURATION_HOURS) {
            unlockUser(username);
            locked = false;
        }
    }

    return locked;
}


//删除账户
void ConfigurationService::removeGroup(const QString &groupId)
{

    // 指定读取文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    // 删除特定组
    settings.remove(groupId);
}

//删除某个账户中的某个数据项
void ConfigurationService::removeItemInGroup(const QString &groupId, const QString &key)
{
    // 指定操作文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    // 开始特定组
    settings.beginGroup(groupId);

    // 删除特定键
    settings.remove(key);

    // 结束组
    settings.endGroup();
}

//删除所有组中的某个数据项
void ConfigurationService::removeItemInAllGroups(const QString &key)
{

    // 指定读取文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    // 获取所有的组
    QStringList allGroups = settings.childGroups();

    // 遍历所有组
    for (const QString &groupName : allGroups) {
        // 开始当前组
        settings.beginGroup(groupName);

        // 删除特定键
        settings.remove(key);

        // 结束当前组
        settings.endGroup();
    }
}

//清空所有账户
void ConfigurationService::clearAllGroups()
{
    // 指定读取文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    settings.clear();
}

//在某个账户中插入某个数据项
void ConfigurationService::insertItemInGroup(const QString &groupName, const QString &key, const QVariant &value)
{

    // 指定读取文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    // 开始特定组
    settings.beginGroup(groupName);

    // 插入键值对
    settings.setValue(key, value);

    qDebug()<<groupName<<"Insert:"<<key<<":"<<value;

    // 结束组
    settings.endGroup();
}

//在所有账户中插入某个数据项
void ConfigurationService::insertItemInAllGroups(const QString &key, const QVariant &value)
{
    // 指定操作文件在当前目录下，以INI格式保存
    QSettings settings(PATH, QSettings::IniFormat);

    // 获取所有的组
    QStringList allGroups = settings.childGroups();

    // 遍历所有组
    for (const QString &groupName : allGroups) {
        // 开始当前组
        settings.beginGroup(groupName);

        // 插入键值对
        settings.setValue(key, value);

        // 结束当前组
        settings.endGroup();
    }
}
