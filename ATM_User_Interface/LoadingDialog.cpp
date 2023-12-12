#include "LoadingDialog.h"

LoadingDialog::LoadingDialog(QWidget *parent) : QDialog(parent)
{
    //如果需要显示任务栏对话框则删除Qt::Tool
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    initUi();
}

/**
 * @brief LoadingDialog::initUi UI元素初始化
 */
void LoadingDialog::initUi()
{
    this->setFixedSize(250, 250);
    m_pCenterFrame = new QFrame(this);
    m_pCenterFrame->setGeometry(10, 10 ,230, 230);

    //加载Loading动画
    m_pLoadingMovie = new QMovie(":/Image/load.gif");
    m_pLoadingMovie->setScaledSize(QSize(120, 120));
    m_pMovieLabel = new QLabel(m_pCenterFrame);
    m_pMovieLabel->setGeometry(55, 10, 120, 120);
    m_pMovieLabel->setScaledContents(true);
    m_pMovieLabel->setMovie(m_pLoadingMovie);
    m_pLoadingMovie->start();

    //提示文本
    m_pTipsLabel = new QLabel(m_pCenterFrame);
    m_pTipsLabel->setGeometry(5, 130, 220, 50);
    m_pTipsLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    m_pTipsLabel->setObjectName("tips");
    m_pTipsLabel->setText("加载中,请稍候...");
    m_pTipsLabel->setStyleSheet("QLabel#tips{font-family:\"Microsoft YaHei\";font-size: 20px;color: #333333;}");

    //进度条
    ProgressBar = new QProgressBar(m_pCenterFrame);
    ProgressBar->setObjectName("ProgressBar");
    ProgressBar->setStyleSheet("QProgressBar{height:22px; text-align:center; font-size:14px; color:white; border-radius:11px; background:#1D5573;}"
                                      "QProgressBar::chunk{border-radius:11px;background:qlineargradient(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 #99ffff,stop:1 #9900ff);}");
    ProgressBar->setGeometry(25, 180, 180, 35);
    ProgressBar->setRange(0,100);
    ProgressBar->setValue(0);
    ProgressBar->setEnabled(true);

    Timer = new QTimer(ProgressBar);
    connect(Timer,&QTimer::timeout,[=]{if(Value<=100)ProgressBar->setValue(Value++);else
        {Value=0; m_pTipsLabel->setText("创建完毕");Timer->stop();emit loadfinshed();}});
    connect(ProgressBar, &QProgressBar::valueChanged,[=](int value){emit progress(value);});
    Timer->start(80);

    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(32, 101, 165));
    shadow->setBlurRadius(10);
    this->setGraphicsEffect(shadow);
}

/**
 * @brief LoadingDialog::setTipsText 设置提示文本
 * @param strTipsText 提示文本
 */
void LoadingDialog::setTipsText(QString strTipsText)
{
    m_pTipsLabel->setText(strTipsText);
}


/**
 * @brief LoadingDialog::moveToCenter 移动对话框到指定窗口中间
 * @param pParent 指定窗口指针
 */
void LoadingDialog::moveToCenter(QWidget *pParent)
{
    if(pParent != nullptr && pParent != NULL)
    {
        int nParentWidth = pParent->width();
        int nParentHeigth = pParent->height();

        int nWidth = this->width();
        int nHeight = this->height();

        int nParentX = pParent->x();
        int nParentY = pParent->y();

        int x = (nParentX + (nParentWidth - nWidth) / 2);
        int y = (nParentY + (nParentHeigth - nHeight) / 2);

        this->move(x, y);
    }
}


/**
 * @brief LoadingDialog::paintEvent 界面绘制
 * @param event
 */
void LoadingDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); //反锯齿
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setLeft(9);
    rect.setTop(9);
    rect.setWidth(rect.width() - 9);
    rect.setHeight(rect.height() - 9);
    painter.drawRoundedRect(rect, 8, 8);
    QWidget::paintEvent(event);
}

LoadingDialog::~LoadingDialog()
{
    delete m_pLoadingMovie;
    delete m_pMovieLabel;
    delete m_pTipsLabel;
    delete ProgressBar;
    delete m_pCenterFrame;
}
