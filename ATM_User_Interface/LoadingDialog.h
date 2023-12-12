#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H
#include <QMovie>
#include <QLabel>
#include <QTimer>
#include <QDialog>
#include <QPainter>
#include <QProgressBar>
#include <QGraphicsDropShadowEffect>
#define USER_CANCEL -1
class LoadingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoadingDialog(QWidget *parent = nullptr);
    ~LoadingDialog();
    //设置提示文本
    void setTipsText(QString strTipsText);
    //移动到指定窗口中间显示
    void moveToCenter(QWidget* pParent);

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void initUi();

signals:

    void progress(int);
    void loadfinshed();

private:
    int Value =0;
    QTimer *Timer;
    QFrame *m_pCenterFrame;
    QLabel *m_pMovieLabel;
    QMovie *m_pLoadingMovie;
    QLabel *m_pTipsLabel;
    QProgressBar *ProgressBar;

};
#endif // LOADINGDIALOG_H
