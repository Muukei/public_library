#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QHeaderView>
#include <QPainter>
#include <LoginDialog.h>
#include <QMessageBox>
#include <LoadingDialog.h>
#include <QFileDialog>
//背景图片
QString ATM_BACKGROUBD_IMAGE = ":/Image/background.jpg";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitWindow();


    // 设置计时器
    Timer = new QTimer(this);
    Timer->setInterval(30000);  // 设置为300秒
    connect(Timer, &QTimer::timeout, this, &MainWindow::autoReturn);

    // 开始计时器
    Timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化
void MainWindow::InitWindow()
{

    //窗口标志
   // showFullScreen();
    ui->tableWidget->clear();

    ui->Window->setCurrentIndex(0);
    ui->tableWidget->setFont(QFont("Microsoft YaHei UI Light",20));
    ui->tableWidget->horizontalHeader()->setFixedHeight(120);

    // 设置表头背景颜色
    ui->tableWidget->horizontalHeader()->setStyleSheet("background-color: lightblue;");


    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->setShowGrid(true);//显示表格线

    ui->tableWidget->setSpan(0,0, 2, 1);
    ui->tableWidget->setSpan(0,1, 2, 1);
    ui->tableWidget->setSpan(0,2, 2, 1);
    ui->tableWidget->setSpan(0,3, 2, 1);
    ui->tableWidget->setRowHeight(0,100);

    ui->MoneyEdit->setReadOnly(true);

    QPushButton*UserButton = new QPushButton(ui->lineEdit_user);
    QPushButton*keyButton = new QPushButton(ui->lineEdit_key);
    QPushButton* PasswordButton = new QPushButton(ui->lineEdit_password);
    UserButton->setFixedSize(QSize(50,50));
    keyButton->setFixedSize(QSize(50,50));
    PasswordButton->setFixedSize(QSize(50,50));

    UserButton->setStyleSheet("background:transparent;border:none;");
    keyButton->setStyleSheet("background:transparent;border:none;");
    PasswordButton->setStyleSheet("background:transparent;border:none;");

    UserButton->setIcon(QIcon(":/Image/user.png"));
    keyButton->setIcon(QIcon(":/Image/key.png"));
    PasswordButton->setIcon(QIcon(":/Image/luck.png"));

    UserButton->setIconSize(QSize(50,50));
    keyButton->setIconSize(QSize(50,50));
    PasswordButton->setIconSize(QSize(50,50));

    UserButton->setFlat(true);
    keyButton->setFlat(true);
    PasswordButton->setFlat(true);

    QHBoxLayout*Layout_user = new QHBoxLayout(ui->lineEdit_user);
    QHBoxLayout*Layout_key = new QHBoxLayout(ui->lineEdit_key);
    QHBoxLayout*Layout_password = new QHBoxLayout(ui->lineEdit_password);
    Layout_user->addWidget(UserButton,0,Qt::AlignLeft|Qt::AlignTop);
    Layout_key->addWidget(keyButton,0,Qt::AlignLeft|Qt::AlignTop);
    Layout_password->addWidget(PasswordButton,0,Qt::AlignLeft|Qt::AlignTop);

    ui->lineEdit_user->setLayout(Layout_user);
    ui->lineEdit_key->setLayout(Layout_key);
    ui->lineEdit_password->setLayout(Layout_password);

    ui->lineEdit_key->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->lineEdit_password->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    ui->lineEdit_user->setTextMargins(80,0,0,0);
    ui->lineEdit_key->setTextMargins(80,0,0,0);
    ui->lineEdit_password->setTextMargins(80,0,0,0);

    ui->ObjectUser_Box->setIconSize(QSize(50,50));
    ui->ObjectUser_Box->setEditable(false);

    QAction*UserIDAction = new QAction(ui->ObjectID_lineEdit);
    UserIDAction->setIconText("Accout ID");
    UserIDAction->setIcon(QIcon(":/Image/id card.png"));
    ui->ObjectID_lineEdit->setInputMethodHints(Qt::ImhDigitsOnly);
    ui->ObjectID_lineEdit->setPlaceholderText(tr("账号"));
    ui->ObjectID_lineEdit->setMaxLength(20);
    ui->ObjectID_lineEdit->addAction(UserIDAction,QLineEdit::LeadingPosition);

    QPushButton*PrintButton = new QPushButton("打印",ui->DealButtonBox);
    QPushButton*CancleButton = new  QPushButton("取消",ui->DealButtonBox);
    QPushButton*ApplyButton = new  QPushButton("确认",ui->DealButtonBox);
    ui->DealButtonBox->addButton(PrintButton,QDialogButtonBox::NoRole);
    ui->DealButtonBox->addButton(CancleButton,QDialogButtonBox::NoRole);
    ui->DealButtonBox->addButton(ApplyButton,QDialogButtonBox::NoRole);
    connect(PrintButton,&QPushButton::pressed,this,&MainWindow::printOrderForm);
    connect(CancleButton,&QPushButton::pressed,this,&MainWindow::cancelOperation);
    connect(ApplyButton,&QPushButton::pressed,this,&MainWindow::executeOperation);

    ui->PasswordbuttonBox->addButton("取消",QDialogButtonBox::RejectRole);
    ui->PasswordbuttonBox->addButton("确定",QDialogButtonBox::AcceptRole);

    QStringList HStrList;
    HStrList.push_back(QString("用户名/User Name"));
    HStrList.push_back(QString("账户/Account"));
    HStrList.push_back(QString("余额/Balance"));
    HStrList.push_back(QString("币种/Currency"));
    //设置行列数(只有列存在的前提下，才可以设置列标签)
    int HlableCnt = HStrList.count();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(HlableCnt);



    //设置列标签
    ui->tableWidget->setHorizontalHeaderLabels(HStrList);

    for ( int i = 0;i<ui->tableWidget->rowCount();i++)
    {
        for (int j = 0;j<ui->tableWidget->columnCount();j++)
        {
            QTableWidgetItem*Item = new QTableWidgetItem();
            if(j ==3)
                Item->setIcon(QIcon(":/Image/doller.png"));
            Item->setSizeHint(QSize(200,400));
            Item->setTextAlignment(Qt::AlignCenter);
            Item->setFont(QFont("Microsoft YaHei UI Light",20));
            ui->tableWidget->setItem(i,j,Item);
        }
    }
}

QString MainWindow::createOrderNumber()
{
    // 获取当前日期和时间
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // 使用日期和时间信息生成订单号
    QString orderNumber = "ORD" +
            currentDateTime.toString("yyyyMMddhhmmss") +
            QString::number(qrand() % 1000);  // 添加随机数确保唯一性

    return orderNumber;
}

void MainWindow::saveToFile(const QString &fileName, const QString &data)
{
    QFile file(fileName);

    // 打开文件，如果文件不存在则创建，如果文件已存在则覆盖内容
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // 写入数据到文件
        out << data;

        // 关闭文件
        file.close();
        qDebug() << "File saved successfully.";
    } else {
        qDebug() << "Failed to save file.";
    }
}

//背景绘制
void MainWindow::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(ATM_BACKGROUBD_IMAGE));
}
//鼠标事件中断自动退出操作
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    Timer->stop();
    Timer->start();
    QMainWindow::mousePressEvent(event);
}
//键盘事件中断自动退出操作
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    Timer->stop();
    Timer->start();
    QMainWindow::keyPressEvent(event);
}

//自动退出操作30秒返回操作首页，60秒退出登录
void MainWindow::autoReturn()
{
    if(!ExistedProgress)
    {
        ui->Window->setCurrentIndex(0);
        ExistedProgress = true;
        qDebug()<<"等待退出状态！";
        return;
    }


    close();
    LoginDialog *dialog = qobject_cast<LoginDialog*>(parentWidget());
    dialog->show();
    ui->Window->setCurrentIndex(0);
}

//接收登录的用户数据账户id，用户名，密码
void MainWindow::setUserData(const QString &id, const QString &username, const QString &password)
{
    GroupId = id;
    UserName= username;
    Password = password;
}

//选择路径打印订单
void MainWindow::printOrderForm()
{
    QString Filepath = QFileDialog::getSaveFileName(nullptr, "Save File", QDir::currentPath(), "Text Files (*.txt)");

    if (!Filepath.isEmpty()) {

        QString user = ui->User_lineEdit->text();
        QString id =ui->ID_lineEdit->text();
        QString deal_type = ui->Type_lineEdit->text();
        QString balance = ui->Balance_lineEdit->text();
        QString time = ui->Time_lineEdit->text();
        QString deal_object = ui->ObjectUser_Box->currentText();
        QString deal_objectid = ui->ObjectID_lineEdit->text();
        QString Order = ui->Num_lineEdit->text();
        QString text = ui->Msg_TextEdit->toPlainText();

        QString dataToSave = tr("用户名称：%1\n账户编号：%2\n交易类型：%3\n交易额度：%4\n"
                                "交易时间：%5\n交易对象：%6\n交易账户：%7\n备注信息：%8\n").
                arg(user).arg(id).arg(deal_type).arg(balance).arg(time).
                arg(deal_object).arg(deal_objectid).arg(Order).arg(text);

        saveToFile(Filepath, dataToSave);
    } else {
        qDebug() << "Save operation canceled.";
    }
}


void MainWindow::executeOperation()
{

    //账户信息获取
    Account Data = ConfigService->getInstance()->readUserAccountData(UserName,GroupId);
    //余额
    QString balance_str = Data.balance;

    double  result=0;

    //取款
    if(type ==Operation_type::withdraw_deposit)
    {
        //取款
        double withdrawal = ui->Balance_lineEdit->text().remove("$").toDouble();
        result = balance_str.toDouble()-withdrawal;
        if (result < 0.0)
        {
            QMessageBox::warning(this,"WARNING","Your balance is insufficient and cannot be withdrawn!");
            qDebug() << "The withdrawal amount is greater than the current balance. Transaction not allowed.";
        } else {
            ConfigService->getInstance()->insertItemInGroup(UserName,"Balance",result);
            ui->MoneyEdit->clear();
            qDebug() <<"Transaction successful,The current balance is: "<<result;
        }

    }else if (type ==Operation_type::Cash_Deposit) {   //存款

        double deposit = ui->Balance_lineEdit->text().remove("$").toDouble();
        result = deposit+balance_str.toDouble();
        ConfigService->getInstance()->insertItemInGroup(UserName,"Balance",result);
        showHint("存款中...","存款成功！");
        qDebug() <<"Transaction successful,The current balance is: "<<result;
    }else if (type ==Operation_type::Fund_transfer) {   //转账
        if(ui->ObjectUser_Box->currentText().isEmpty()||ui->ObjectID_lineEdit->text().isEmpty())
        {
            QMessageBox::warning(this,"WARING","You did not fill in the account information of the person you want to transfer money to! Or the specified user is not found according to the account information you filled in.");
            return;
        }else {

            QString str = tr("Are you sure you want to transfer money to this user %1?").arg(ui->ObjectUser_Box->currentText());
            QMessageBox::StandardButton button =QMessageBox::warning(this,"WARING",str,QMessageBox::Yes|QMessageBox::No);
            if(button==QMessageBox::Yes)
            {
                double amount = ui->Balance_lineEdit->text().remove("$").toDouble();
                result = balance_str.toDouble()-amount;
                if (result < 0.0)
                {
                    QMessageBox::warning(this,"WARNING","Your balance is insufficient and cannot be withdrawn!");
                    qDebug() << "The withdrawal amount is greater than the current balance. Transaction not allowed.";
                } else {
                    ConfigService->getInstance()->insertItemInGroup(ui->ObjectUser_Box->currentText(),"Balance",result);
                    showHint("转账中...","转账成功！");
                    qDebug() <<"Transaction successful,The current balance is: "<<result;
                }
            }
        }
    }
}
//取消操作
void MainWindow::cancelOperation()
{
    ui->Window->setCurrentIndex(0);
}

void MainWindow::showHint(const QString &text,const QString& finshed)
{
    //加载等待弹窗
    LoadingDialog *loadingDialog =new LoadingDialog;
    loadingDialog->setTipsText(text);
    connect(loadingDialog,&LoadingDialog::loadfinshed,[=]
    {loadingDialog->close(); loadingDialog->setTipsText(finshed);});

    loadingDialog->exec();
    loadingDialog->deleteLater();
    ui->Window->setCurrentIndex(0);
}

//余额查询接口
void MainWindow::on_Balance_Button_pressed()
{
    ui->Window->setCurrentIndex(1);
    Account Data = ConfigService->getInstance()->readUserAccountData(UserName,GroupId);
    ui->tableWidget->item(0,0)->setText(Data.userName);
    ui->tableWidget->item(0,0)->setToolTip(Data.userName);
    ui->tableWidget->item(0,1)->setText(Data.groupid);
    ui->tableWidget->item(0,0)->setToolTip(Data.groupid);
    ui->tableWidget->item(0,2)->setText(Data.balance);
    ui->tableWidget->item(0,0)->setToolTip(Data.balance);
    ui->tableWidget->item(0,3)->setText(Data.currency);
    ui->tableWidget->item(0,0)->setToolTip(Data.currency);

}

//现金储蓄操作接口(跳转到输入金额界面)
void MainWindow::on_CashDeposit_Button_pressed()
{
    type = Operation_type::Cash_Deposit;
    ui->Window->setCurrentIndex(2);
}

//取消交易操作接口(跳转到输入金额界面)
void MainWindow::on_DealActionCancelMsgButton_pressed()
{
    int index = ui->Window->currentIndex();
    if(index--)
        ui->Window->setCurrentIndex(0);
    else {
        index =0;
        close();
        LoginDialog *dialog = qobject_cast<LoginDialog*>(parentWidget());
        dialog->show();
    }
}

//转账操作接口(跳转到输入金额界面)
void MainWindow::on_CashTransferButton_pressed()
{
    type = Operation_type::Fund_transfer;
    ui->Window->setCurrentIndex(2);
}

//提取存款操作接口(跳转到输入金额界面)
void MainWindow::on_WithdrawDepositButton_pressed()
{
    type = Operation_type::withdraw_deposit;
    ui->Window->setCurrentIndex(2);

}

//修改密码操作接口(跳转到修改密码操作界面)
void MainWindow::on_ChangePasswordButton_pressed()
{
    ui->lineEdit_user->setText(UserName);
    ui->Window->setCurrentIndex(4);

}

//输入金额按钮（100$）
void MainWindow::on_pushButton_1_pressed()
{
    ui->MoneyEdit->setText(ui->pushButton_1->text());
}

//输入金额按钮（200$）
void MainWindow::on_pushButton_2_pressed()
{
    ui->MoneyEdit->setText(ui->pushButton_2->text());
}

//输入金额按钮（300$）
void MainWindow::on_pushButton_3_pressed()
{
    ui->MoneyEdit->setText(ui->pushButton_3->text());
}
//输入金额按钮（500$）
void MainWindow::on_pushButton_4_pressed()
{
    ui->MoneyEdit->setText(ui->pushButton_4->text());
}

//输入金额按钮（1000$）
void MainWindow::on_pushButton_5_pressed()
{
    ui->MoneyEdit->setText(ui->pushButton_5->text());
}

//输入金额按钮（3000$）
void MainWindow::on_pushButton_6_pressed()
{
    ui->MoneyEdit->setText(ui->pushButton_6->text());
}

//清除输入操作接口
void MainWindow::on_pushButton_7_pressed()
{
    ui->MoneyEdit->clear();
}

//
void MainWindow::on_pushButton_8_pressed()
{
    if(!ui->MoneyEdit->text().isEmpty())
    {
        ui->Window->setCurrentIndex(3);
        ui->User_lineEdit->setText(UserName);
        ui->ID_lineEdit->setText(GroupId);
        if(type ==Operation_type::Cash_Deposit){
            ui->Object_label->hide();
            ui->ObjectID_lineEdit->hide();
            ui->ObjectUser_Box->hide();
            ui->Type_lineEdit->setText("Cash Deposit/储蓄现金");
        }
        else if (type ==Operation_type::withdraw_deposit){

            ui->Object_label->hide();
            ui->ObjectID_lineEdit->hide();
            ui->ObjectUser_Box->hide();
            ui->Type_lineEdit->setText("Withdraw Deposit/提取存款");
        }
        else if (type ==Operation_type::Fund_transfer) {
            ui->Object_label->show();
            ui->ObjectID_lineEdit->show();
            ui->ObjectUser_Box->show();
            ui->Type_lineEdit->setText("Fund Transfer/转账交易");
        }
        ui->Balance_lineEdit->setText(ui->MoneyEdit->text());
        ui->Time_lineEdit->setText(QDateTime::currentDateTime().toString(Qt::ISODate));
        ui->Num_lineEdit->setText(createOrderNumber());
        ui->Msg_TextEdit->setText("无");
    }
    else {
        QMessageBox::warning(this,"WARNING","Please enter the amount of cash required!");
    }

    ui->MoneyEdit->clear();
}

//输入金额按钮（100$）
void MainWindow::on_pushButton_9_pressed()
{
    // 设置输入法提示，开启数字虚拟键盘
    ui->MoneyEdit->setInputMethodHints(Qt::ImhDigitsOnly);

    // 弹出虚拟键盘
    QInputMethod *inputMethod = QGuiApplication::inputMethod();
    inputMethod->show();

}

//设置密码
void MainWindow::on_PasswordbuttonBox_accepted()
{
    QString key =ui->lineEdit_key->text();
    QString Newpassword = ui->lineEdit_password->text();
    if(key.isEmpty())
    {
        QMessageBox::warning(this,"WARNING","Please enter the password that needs to be reset!");
        return;
    }else if (Newpassword.isEmpty()) {
        QMessageBox::warning(this,"WARNING","Please enter your password again!");
        return;
    }else if (key!=Newpassword) {
        QMessageBox::warning(this,"ERRO","The two entered passwords are inconsistent, please make sure that you reset the password correctly!");
        return;
    }else {
        QMessageBox::StandardButton button =QMessageBox::information(this,"QUESTION","Are you sure you want to change your password?You may need to re-log in to your account after the setup is successful.");
        if(button ==QMessageBox::Ok)
        {
            if(ConfigService->getInstance()->isGroupExists("LOGO"))
                ConfigService->getInstance()->insertItemInGroup("LOGO","Password",Newpassword);
            if(ConfigService->getInstance()->isGroupExists(UserName))
            {
                QList<Account> List = ConfigService->getInstance()->GroupName_isExisted(UserName);
                if(!List.isEmpty())
                {
                    for (Account account:List)
                    {
                        QString AccoutID = ConfigService->getInstance()->inquireKey(UserName,"GroupID");
                        QString Accoutpassword = ConfigService->getInstance()->inquireKey(UserName,"Password");
                        if(AccoutID==GroupId&&Accoutpassword==Password)
                        {
                            ConfigService->getInstance()->insertItemInGroup(UserName,"Password",Newpassword);

                            showHint("密码修改中...","修改成功！");
                            ui->lineEdit_user->clear();ui->lineEdit_key->clear();ui->lineEdit_password->clear();
                            close();
                            LoginDialog *dialog = qobject_cast<LoginDialog*>(parentWidget());
                            dialog->show();

                        }


                    }
                }
            }

        }else {
            return;
        }
    }
}

//取消设置密码
void MainWindow::on_PasswordbuttonBox_rejected()
{
    ui->Window->setCurrentIndex(0);
}

void MainWindow::on_ObjectID_lineEdit_textChanged(const QString &arg1)
{
    qDebug()<<"查找中...";
    ui->lineEdit_hint->clear();
    QString user = ConfigService->getInstance()->findGroupByItemAndValue("GroupID",arg1);
    if(user.isEmpty())
    {
      qDebug()<< "No related user information found! Unable to transfer transactio!";
    }else {
         ui->ObjectUser_Box->addItem(QIcon(":/Image/user.png"),user);
    }
}
