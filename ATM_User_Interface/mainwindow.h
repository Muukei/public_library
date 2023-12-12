#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QToolBar>
#include <QPushButton>
#include <QMainWindow>
#include <QListWidgetItem>
#include <ConfigurationService.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum Operation_type{withdraw_deposit=1,
                        Cash_Deposit=2,Fund_transfer=3};
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void InitWindow();

    virtual  QString createOrderNumber();

    void saveToFile(const QString &fileName, const QString &data);


protected:
    QListWidgetItem* createItem(QString ItemText,QIcon Icon);

    void paintEvent(QPaintEvent *event) override;

    virtual void mousePressEvent(QMouseEvent *event)override;

    virtual void keyPressEvent(QKeyEvent *event)override;

public slots:

    //自动退出登录
    void autoReturn();
    //获取登录信息
    void setUserData(const QString &id, const QString &username, const QString &password);

    //打印单据
    void printOrderForm();

    //执行操作
    void executeOperation();

    //取消操作
    void cancelOperation();

    //显示成功信息
    void  showHint(const QString &text,const QString& finshed);


private slots:
    void on_Balance_Button_pressed();

    void on_CashDeposit_Button_pressed();

    void on_DealActionCancelMsgButton_pressed();

    void on_CashTransferButton_pressed();

    void on_WithdrawDepositButton_pressed();

    void on_ChangePasswordButton_pressed();

    void on_pushButton_1_pressed();

    void on_pushButton_2_pressed();

    void on_pushButton_3_pressed();

    void on_pushButton_4_pressed();

    void on_pushButton_5_pressed();

    void on_pushButton_6_pressed();

    void on_pushButton_7_pressed();



    void on_pushButton_8_pressed();

    void on_pushButton_9_pressed();

    void on_PasswordbuttonBox_accepted();

    void on_PasswordbuttonBox_rejected();

    void on_ObjectID_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QTimer *Timer;
    Operation_type type;
    bool ExistedProgress = false;;
    QString GroupId,UserName,Password;
    ConfigurationService* ConfigService;

};

#endif // MAINWINDOW_H
