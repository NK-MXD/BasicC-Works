#pragma once
#pragma execution_character_set("utf-8")  

#include <QtWidgets/QMainWindow>
#include "ui_Snake.h"
#include<qpainter.h>
#include<vector>
#include<qstring.h>

namespace Ui {
    class Snake;
}


class Snake : public QMainWindow
{
    Q_OBJECT

public:
    Snake(QWidget *parent = Q_NULLPTR);
    ~Snake();
    void paintEvent(QPaintEvent* event);
    void InitSnake();//初始化蛇
    QRect CreateRect();
    void IsEat();
    void IsHit();//蛇是否撞到了自己

private slots:
    void Snake_update();//定义槽函数

private:
    Ui::SnakeClass *ui;
    QVector<QRect> vSnakeRect;
    QRect SnakeHead;//蛇头
    void keyPressEvent(QKeyEvent* key);//定义键盘事件
    QTimer* timer;//计时器
    bool blsRun;//是否开始
    bool blsOver;//是否结束
    QString sDisplay;//开始结束提示
    int nDirection;//蛇移动的方向
    QRect food;//食物
    int nScore;
    QString scoreLabel;//显示得分两个字
    int speed;//移动速度
};
