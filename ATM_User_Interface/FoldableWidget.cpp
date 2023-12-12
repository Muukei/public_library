#include "FoldableWidget.h"
#include <QDebug>
#include <QSpacerItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QPropertyAnimation>
FoldableWidget::FoldableWidget(QWidget *parent):QWidget(parent)
  ,Animation(new QPropertyAnimation(this,""))
{
}

FoldableWidget::~FoldableWidget()
{

}

void FoldableWidget::addWidget(QWidget *Widget)
{
    this->layout()->addWidget(Widget);
}

void FoldableWidget::setAnimationTime(int time)
{
    Time =time;
}

void FoldableWidget::setDirection(Qt::Orientation orientation)
{
    if(orientation == Qt::Horizontal)
    {
        Direction =Qt::Horizontal;
        connect(Animation,&QPropertyAnimation::valueChanged,[this](const QVariant &varint)
        {this->setFixedWidth(varint.toInt());});
    }
    else if(orientation == Qt::Vertical)
    {
        Direction =Qt::Vertical;
        connect(Animation,&QPropertyAnimation::valueChanged,[this](const QVariant &varint)
        {this->setFixedHeight(varint.toInt());});
    }
}

void FoldableWidget::run_Animation()
{
    if(state)
      setexpanded();
    else {
        setcollapse();
    }
}

void FoldableWidget::setexpanded()
{
    Animation->setDuration(Time);  //展开状态
    Animation->setStartValue(0);
    if(Direction == Qt::Horizontal)
        Animation->setEndValue(layout()->sizeHint().width());
    if(Direction ==Qt::Vertical)
        Animation->setEndValue(layout()->sizeHint().height());
    Animation->start();
    emit expanded();
    state = false;
}

void FoldableWidget::setcollapse()
{
     Animation->setDuration(Time);  //收缩
    if(Direction == Qt::Horizontal)
        Animation->setStartValue(width());
    if(Direction ==Qt::Vertical)
        Animation->setStartValue(height());

    Animation->setEndValue(0);
    Animation->start();
    emit collapse();
    state = true;
}

void FoldableWidget::resizeEvent(QResizeEvent *event)
{
    if(Direction==Qt::Horizontal)
        Value = event->size().width();
    if(Direction == Qt::Vertical)
        Value = event->size().height();

    QWidget::resizeEvent(event);
}
