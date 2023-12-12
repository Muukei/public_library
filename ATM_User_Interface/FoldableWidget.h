#ifndef FOLDABLEWIDGET_H
#define FOLDABLEWIDGET_H

#include <QWidget>
class QPropertyAnimation;
class FoldableWidget:public QWidget
{
    Q_OBJECT
public:
    FoldableWidget(QWidget*parent = Q_NULLPTR);
    ~FoldableWidget();

    void addWidget(QWidget*Widget);

    void setAnimationTime(int time);

    void setDirection(Qt::Orientation orientation);

    bool isState(){return  state;}

signals:
     void expanded();

     void collapse();

public slots:

    virtual void run_Animation();

    virtual void setexpanded();

    virtual void setcollapse();

protected:

    void resizeEvent(QResizeEvent *event) override;

private:

    int Value=0;
    int Time = 400;
    bool state = false;
    Qt::Orientation Direction;
    QPropertyAnimation*Animation;
};

#endif // FOLDABLEWIDGET_H
