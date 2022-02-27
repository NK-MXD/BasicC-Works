#include "Snake.h"
#include "ui_snake.h"
#include<qrect.h>
#include<qtimer.h>
#include<QKeyEvent>

Snake::Snake(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::SnakeClass), blsRun(false), speed(1000)
{
    ui->setupUi(this);
    this->setGeometry(QRect(600, 300, 580, 620));
}

Snake::~Snake()
{
    delete ui;
}

void Snake::paintEvent(QPaintEvent* event) {
    //update()或程序第一次启动时执行,用于画图
    QPainter painter(this);
    if (!blsRun) {//只有blsRun为false时才需要初始化蛇
        InitSnake();//初始化蛇，此时蛇还没有画，只是初始化了矩形，没有显示
    }
    painter.setPen(Qt::black);
    painter.setBrush(Qt::gray);//设置填充颜色
    painter.drawRect(30, 30, 520, 520);//画矩形
    //相对于窗体的起点坐标为(15,15),大小为260*260
    //内墙的绘制
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawPixmap(40, 40, 500, 500, QPixmap(":/myImage/images/open.jpg"));
    //绘制矩形,相对于窗口的起点坐标为(20,20),大小为250*250
    //下面就要在黑色方框里面画小方格
    /*painter.setPen(Qt::blue);
    for(int i=2;i<=27;i++){
        painter.drawLine(40,i*20,540,i*20);
        painter.drawLine(i*20,40,i*20,540);
    }*/
    //显示游戏开始与结束
    QFont font1("Courier", 14);
    painter.setFont(font1);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    painter.drawText(80, 300, sDisplay);
    //游戏得分显示:
    QFont font2("Courier", 30);
    painter.setFont(font2);
    painter.setPen(Qt::blue);
    painter.setBrush(Qt::blue);
    painter.drawText(280, 600, scoreLabel);
    painter.drawText(460, 600, QString::number(nScore));
    //画蛇:
    painter.setPen(Qt::black);
    painter.setBrush(Qt::green);
    //painter.drawRect(food);
    painter.drawPixmap(food, QPixmap(":/myImage/images/food.png"));
    painter.drawRects(&vSnakeRect[0], vSnakeRect.size());
    //画n个小方块
    if (blsOver)
        timer->stop();//游戏结束,停止计时
}

void Snake::InitSnake() {
    nDirection = 2;//默认向下移动
    blsRun = true;
    blsOver = false;
    sDisplay = "游戏开始";//newline in constant
    scoreLabel = "得分:";
    nScore = 0;
    food = CreateRect();
    //QRect rect(160,140,20,20);//蛇的起始位置
    //vSnakeRect=rect;//将矩形赋值给蛇
    vSnakeRect.resize(5);
    //利用一个for循环初始化vector
    for (int i = 0; i < vSnakeRect.size(); i++)
    {
        QRect rect(160, 140 + 20 * i, 20, 20);
        //生成小方块
        vSnakeRect[vSnakeRect.size() - 1 - i] = rect;
    }
    SnakeHead = vSnakeRect.first();
    timer = new QTimer(this);
    timer->start(500);
    connect(timer, SIGNAL(timeout()), SLOT(Snake_update()));
    //信号连接槽,发出信号的是timer,信号是timeout,执行的槽是Snake_update

}

void Snake::keyPressEvent(QKeyEvent* event) {
    QKeyEvent* key = (QKeyEvent*)event;
    switch (key->key()) {
    case Qt::Key_W:nDirection = 1;
        break;
    case Qt::Key_S:nDirection = 2;
        break;
    case Qt::Key_A:nDirection = 3;
        break;
    case Qt::Key_D:nDirection = 4;
        break;
    default:;
    }
}

void Snake::Snake_update() {
    sDisplay = "";
    SnakeHead = vSnakeRect.first();
    IsEat();
    IsHit();
    for (int j = 0; j < vSnakeRect.size() - 1; j++) {
        vSnakeRect[vSnakeRect.size() - 1 - j] = vSnakeRect[vSnakeRect.size() - 2 - j];
    }
    switch (nDirection) {
    case 1:
        SnakeHead.setTop(SnakeHead.top() - 20);
        SnakeHead.setBottom(SnakeHead.bottom() - 20);
        break;
    case 2:
        SnakeHead.setTop(SnakeHead.top() + 20);
        SnakeHead.setBottom(SnakeHead.bottom() + 20);
        break;
    case 3:
        SnakeHead.setLeft(SnakeHead.left() - 20);
        SnakeHead.setRight(SnakeHead.right() - 20);
        break;
    case 4:
        SnakeHead.setLeft(SnakeHead.left() + 20);
        SnakeHead.setRight(SnakeHead.right() + 20);
        break;

    }
    vSnakeRect[0] = SnakeHead;
    //判断是否撞墙
    if (SnakeHead.left() < 40 || SnakeHead.right() > 540 || SnakeHead.top() < 40 || SnakeHead.bottom() > 540) {
        sDisplay = "游戏结束";
        blsOver = true;
    }
    update();

}

QRect Snake::CreateRect() {
    int x, y;
    x = qrand() % 25;//生成小于25的整数
    y = qrand() % 25;
    QRect rect(40 + x * 20, 40 + y * 20, 20, 20);
    return rect;
}

void Snake::IsEat() {
    if (SnakeHead == food) {
        SnakeHead = food;
        vSnakeRect.push_back(vSnakeRect.last());
        food = CreateRect();//吃到食物后再生成一个食物
        nScore += 10;
        if (speed > 50) {
            speed = speed - 10;
            timer->stop();//停止当前计时器
            timer->start(speed);//设置新的计时器
        }
    }
}

void Snake::IsHit() {
    for (int i = 1; i < vSnakeRect.size(); i++) {
        //遍历蛇身
        if (SnakeHead == vSnakeRect[i]) {
            sDisplay = "游戏结束";
            blsOver = true;
            update();
        }
    }
}
