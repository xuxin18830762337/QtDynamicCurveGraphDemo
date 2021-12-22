#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(false);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));

    QObject::connect(ui->btnClear, SIGNAL(clicked(bool)), this, SLOT(slotBtnClear()));
    QObject::connect(ui->btnStartAndStop, SIGNAL(clicked(bool)), this, SLOT(slotBtnStartAndStop()));


    //
    // 创建横纵坐标轴并设置显示范围
    //
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_axisX->setTitleText("X-label");
    m_axisY->setTitleText("Y-label");
    m_axisX->setMin(0);
    m_axisY->setMax(0);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);

    m_lineSeries = new QLineSeries();                             // 创建曲线绘制对象
    m_lineSeries->setPointsVisible(true);                         // 设置数据点可见
    m_lineSeries->setName("随机数曲线");                            // 图例名称

    m_chart = new QChart();                                        // 创建图表对象
    m_chart->addAxis(m_axisX, Qt::AlignLeft);                      // 将X轴添加到图表上
    m_chart->addAxis(m_axisY, Qt::AlignBottom);                    // 将Y轴添加到图表上
    m_chart->addSeries(m_lineSeries);                              // 将曲线对象添加到图表上
    m_chart->setAnimationOptions(QChart::SeriesAnimations);        // 动画：能使曲线绘制显示的更平滑，过渡效果更好看

    m_lineSeries->attachAxis(m_axisX);                             // 曲线对象关联上X轴，此步骤必须在m_chart->addSeries之后
    m_lineSeries->attachAxis(m_axisY);                             // 曲线对象关联上Y轴，此步骤必须在m_chart->addSeries之后

    ui->graphicsView->setChart(m_chart);                           // 将图表对象设置到graphicsView上进行显示
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);       // 设置渲染：抗锯齿，如果不设置那么曲线就显得不平滑
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slotBtnClear()
{
    m_lineSeries->clear();
    m_chart->axisX()->setMin(0);
    m_chart->axisX()->setMax(AXIS_MAX_X);
    pointCount = 0;
}

void Widget::slotBtnStartAndStop()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
        ui->btnStartAndStop->setText("启动定时器");
    }else
    {
        pointCount = 0;
        m_timer->start(200);
        ui->btnStartAndStop->setText("停止定时器");
    }
}

//void Widget::slotTimeout()
//{
//    int count = m_lineSeries->points().size();                  // 获得当前数据序列点数
//    if(count > AXIS_MAX_X)
//    {
//        m_chart->axisX()->setMax(count);                        // 更新X轴范围
//    }
//    m_lineSeries->append(QPointF(count, rand() % AXIS_MAX_Y));  // 更新显示（随机生成10以内的一个数）
//}

void Widget::slotTimeout()
{
    if(pointCount > AXIS_MAX_X)
    {
        m_lineSeries->remove(0);
        m_chart->axisX()->setMin(pointCount - AXIS_MAX_X);
        m_chart->axisX()->setMax(pointCount);                    // 更新X轴范围
    }
    m_lineSeries->append(QPointF(pointCount, rand() % AXIS_MAX_Y));  // 更新显示（随机生成10以内的一个数）
    pointCount++;
}
