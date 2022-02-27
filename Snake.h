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
    void InitSnake();//��ʼ����
    QRect CreateRect();
    void IsEat();
    void IsHit();//���Ƿ�ײ�����Լ�

private slots:
    void Snake_update();//����ۺ���

private:
    Ui::SnakeClass *ui;
    QVector<QRect> vSnakeRect;
    QRect SnakeHead;//��ͷ
    void keyPressEvent(QKeyEvent* key);//��������¼�
    QTimer* timer;//��ʱ��
    bool blsRun;//�Ƿ�ʼ
    bool blsOver;//�Ƿ����
    QString sDisplay;//��ʼ������ʾ
    int nDirection;//���ƶ��ķ���
    QRect food;//ʳ��
    int nScore;
    QString scoreLabel;//��ʾ�÷�������
    int speed;//�ƶ��ٶ�
};
