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
    //update()������һ������ʱִ��,���ڻ�ͼ
    QPainter painter(this);
    if (!blsRun) {//ֻ��blsRunΪfalseʱ����Ҫ��ʼ����
        InitSnake();//��ʼ���ߣ���ʱ�߻�û�л���ֻ�ǳ�ʼ���˾��Σ�û����ʾ
    }
    painter.setPen(Qt::black);
    painter.setBrush(Qt::gray);//���������ɫ
    painter.drawRect(30, 30, 520, 520);//������
    //����ڴ�����������Ϊ(15,15),��СΪ260*260
    //��ǽ�Ļ���
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawPixmap(40, 40, 500, 500, QPixmap(":/myImage/images/open.jpg"));
    //���ƾ���,����ڴ��ڵ��������Ϊ(20,20),��СΪ250*250
    //�����Ҫ�ں�ɫ�������滭С����
    /*painter.setPen(Qt::blue);
    for(int i=2;i<=27;i++){
        painter.drawLine(40,i*20,540,i*20);
        painter.drawLine(i*20,40,i*20,540);
    }*/
    //��ʾ��Ϸ��ʼ�����
    QFont font1("Courier", 14);
    painter.setFont(font1);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    painter.drawText(80, 300, sDisplay);
    //��Ϸ�÷���ʾ:
    QFont font2("Courier", 30);
    painter.setFont(font2);
    painter.setPen(Qt::blue);
    painter.setBrush(Qt::blue);
    painter.drawText(280, 600, scoreLabel);
    painter.drawText(460, 600, QString::number(nScore));
    //����:
    painter.setPen(Qt::black);
    painter.setBrush(Qt::green);
    //painter.drawRect(food);
    painter.drawPixmap(food, QPixmap(":/myImage/images/food.png"));
    painter.drawRects(&vSnakeRect[0], vSnakeRect.size());
    //��n��С����
    if (blsOver)
        timer->stop();//��Ϸ����,ֹͣ��ʱ
}

void Snake::InitSnake() {
    nDirection = 2;//Ĭ�������ƶ�
    blsRun = true;
    blsOver = false;
    sDisplay = "��Ϸ��ʼ";//newline in constant
    scoreLabel = "�÷�:";
    nScore = 0;
    food = CreateRect();
    //QRect rect(160,140,20,20);//�ߵ���ʼλ��
    //vSnakeRect=rect;//�����θ�ֵ����
    vSnakeRect.resize(5);
    //����һ��forѭ����ʼ��vector
    for (int i = 0; i < vSnakeRect.size(); i++)
    {
        QRect rect(160, 140 + 20 * i, 20, 20);
        //����С����
        vSnakeRect[vSnakeRect.size() - 1 - i] = rect;
    }
    SnakeHead = vSnakeRect.first();
    timer = new QTimer(this);
    timer->start(500);
    connect(timer, SIGNAL(timeout()), SLOT(Snake_update()));
    //�ź����Ӳ�,�����źŵ���timer,�ź���timeout,ִ�еĲ���Snake_update

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
    //�ж��Ƿ�ײǽ
    if (SnakeHead.left() < 40 || SnakeHead.right() > 540 || SnakeHead.top() < 40 || SnakeHead.bottom() > 540) {
        sDisplay = "��Ϸ����";
        blsOver = true;
    }
    update();

}

QRect Snake::CreateRect() {
    int x, y;
    x = qrand() % 25;//����С��25������
    y = qrand() % 25;
    QRect rect(40 + x * 20, 40 + y * 20, 20, 20);
    return rect;
}

void Snake::IsEat() {
    if (SnakeHead == food) {
        SnakeHead = food;
        vSnakeRect.push_back(vSnakeRect.last());
        food = CreateRect();//�Ե�ʳ���������һ��ʳ��
        nScore += 10;
        if (speed > 50) {
            speed = speed - 10;
            timer->stop();//ֹͣ��ǰ��ʱ��
            timer->start(speed);//�����µļ�ʱ��
        }
    }
}

void Snake::IsHit() {
    for (int i = 1; i < vSnakeRect.size(); i++) {
        //��������
        if (SnakeHead == vSnakeRect[i]) {
            sDisplay = "��Ϸ����";
            blsOver = true;
            update();
        }
    }
}
