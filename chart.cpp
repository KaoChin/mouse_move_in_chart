#include "chart.h"

#include <QtWidgets/QGesture>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>

#include <QtCharts/QValueAxis>
#include <cmath>
#include <QDebug>
#include <QMenu>


Chart::Chart(QWidget *parent)
    : QChartView(parent),
      series(new QSplineSeries),
      highlightedPointSeries(new QScatterSeries),
      //highlightedPoint(nullptr),
      tangentLine(nullptr)
      //isDragging(false)
      //draggedPointIndex(-1)
{

    // 初始化 QChart
        QChart *chart = new QChart();
        setChart(chart);
        chart->setTitle("y = sin(2 * π / 60 * x)");
        chart->legend()->hide();

        // 設定 X 軸和 Y 軸
        QValueAxis *axisX = new QValueAxis;
        axisX->setRange(0, 900);
        axisX->setTitleText("X");

        QValueAxis *axisY = new QValueAxis;
        axisY->setRange(-1.2, 1.2);
        axisY->setTitleText("Y");

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);



        // 添加曲線到圖表
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);


        // 設置選中點的樣式

        highlightedPointSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        highlightedPointSeries->setMarkerSize(10.0);
        highlightedPointSeries->setBorderColor(Qt::red);
        highlightedPointSeries->setBrush(Qt::red);
        chart->addSeries(highlightedPointSeries);
        highlightedPointSeries->attachAxis(axisX);
        highlightedPointSeries->attachAxis(axisY);

        // 創建顯示滑鼠位置的文本

        mousePositionText = new QGraphicsTextItem(chart);
        mousePositionText->setPos(10, -1.5);

        // 繪製曲線
        plotSpline();
}

Chart::~Chart()
{

}


void Chart::plotSpline() {
    dataPoints.clear();
    const double pi = 3.141592653589793;

    for (int x = 0; x <= 900; x+=dx) {
        double y = sin(2 * pi / 360 * x);
        dataPoints.append(QPointF(x, y));
        //series->append(x, y);
    }

    series->replace(dataPoints);

    series->setName("y = sin(2 * π / 60 * x)");
    series->setColor(Qt::red);
}

void Chart::mouseMoveEvent(QMouseEvent *event) {
    // 將滑鼠的視口座標轉換為場景座標
    QPointF chartPos = chart()->mapToValue(event->pos());

    if (is_pickup_a_point && selectedPointIndex != -1 && selectedPointIndex<0xFFFF) {
            //拖動點時更新點的座標
            highlightedPointSeries->clear();
            highlightedPointSeries->append(chartPos.x(),chartPos.y());

            dataPoints[selectedPointIndex].setX(chartPos.x());
            dataPoints[selectedPointIndex].setY(chartPos.y());
            updateSpline();

        }

    // 更新文本內容，顯示滑鼠位置
    mousePositionText->setPlainText(QString("X: %1, Y: %2").arg(chartPos.x()).arg(chartPos.y()));

    // 更新文本位置，顯示在圖表底部
   // mousePositionText->setPos(chart()->plotArea().left(), chart()->plotArea().bottom() + 10);
    mousePositionText->setPos(event->pos().x() + 10, event->pos().y());

    QChartView::mouseMoveEvent(event);  // 繼續處理預設事件
}

//void Chart::treatment_for_mouse_btn_l_is_hold()
//{
//    qDebug("the mouse left btn pressing elapse : %d msec", mouse_l_press_elapsed_time);
//}

void Chart::mousePressEvent(QMouseEvent *event) {

    //QPointF scenePos = chart()->mapToValue(event->pos());
    //int closestIndex = findClosestPoint(scenePos);

    if (event->button() == Qt::LeftButton) {
        qDebug("mousePressEvent");

        QPointF scenePos = chart()->mapToValue(event->pos());
        selectedPointIndex = findClosestPoint(scenePos);

        qDebug("mouse_btn_l_pressing_elapse : %d msec",mouse_l_press_elapsed_time);

        qDebug("draggedPointIndex=%d",selectedPointIndex);
        if (selectedPointIndex < 0xFFFF) {
            highlightedPointSeries->clear();
            //qDebug("draw high circle at x=%f, y=%f",dataPoints[selectedPointIndex].x(),dataPoints[selectedPointIndex].y());
            highlightedPointSeries->append(dataPoints[selectedPointIndex]);
            //drawTangent(closestIndex);
        }
       /*  qDebug("mouse btn_l pressed !! mouse_l_press_elapsed_time=%d", mouse_l_press_elapsed_time);
       if(mouse_l_press_elapsed_time==0)
        {
            mouse_l_press_start_Time = QTime::currentTime();
            mouse_l_press_elapsed_time=0xFFFF;
            qDebug("mouse btn_l is press, mouse_l_press_elapse_time=0xFFFF");
        }  else if (mouse_l_press_elapsed_time==0xFFFF)
        {
            mouse_l_press_elapsed_time = mouse_l_press_start_Time.msecsTo(QTime::currentTime());
            qDebug("mouse_btn_l_pressing_elapse : %d msec",mouse_l_press_elapsed_time);
            if (mouse_l_press_elapsed_time >500)
            {
                is_mouse_btn_l_hold =true;
                qDebug("mouse_l_btn is hold, elapsed time = %d", mouse_l_press_elapsed_time);
                //treatment_for_mouse_btn_l_is_hold();
            }
        }*/
    } else
    if(event->button() == Qt::RightButton) //clear select
    {
        \
        showContextMenu(event->pos());

        /*

    */}


    QChartView::mousePressEvent(event);
}



void Chart::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {

    } else
    if(event->button() == Qt::LeftButton)
    {
        mouse_l_press_elapsed_time =0;
        qDebug("mouse btn_l is released ");

    }

    QChartView::mouseReleaseEvent(event);
}

void Chart::updateSpline() {
    series->replace(dataPoints);
}

int Chart::findClosestPoint(const QPointF &pos) {
    //int closestIndex = -1;
    //double minDistance = std::numeric_limits<double>::max();
    qDebug("findClosestPoint");

    uint16_t i_data=pos.x()/dx;
    float r_search=0.5;
    uint16_t i_pickup = 0xFFFF;
    uint16_t i_min;
    uint16_t i_max;
    float dy,dx;
    float r2, r2_min=0xFFFF;

    i_min=((i_data)<r_search)?0:i_data-r_search;
    i_max=((i_data)<(900-r_search))?i_data+r_search:900;
    qDebug("i_data=%d i_min=%d i_max=%d", i_data, i_min, i_max);

    for (uint16_t i=i_min;i<i_max+1;i++)
    {
        dy=abs(pos.y()-series->at(i).y());
        dx=abs(series->at(i).x()-series->at(i_data).x());

        if (dy<r_search)
        {
            r2=pow(dx,2)+pow(dy,2);
            qDebug("i=%d x_i=%f y_i=%f dy=%f  r^2=%f", i, series->at(i).x(), series->at(i).y(), dy, r2);
            if (r2<r2_min)
            {
                i_pickup = i;
                r2_min = r2;
                qDebug("i_pickup=%d y=%f ", i_pickup, series->at(i).y());
            }
        }
    }

    /*for (int i = 0; i < dataPoints.size(); ++i) {
        double distance = std::hypot(pos.x() - dataPoints[i].x(), pos.y() - dataPoints[i].y());
        if (distance < minDistance) {
            minDistance = distance;
            closestIndex = i;
        }
    }*/
    if(i_pickup<0xFFFE)
    {
        //highlightedPointSeries->clear();
        qDebug("the minDistance is:%f i_pick=%d pickup_y=%f ",r2_min, i_pickup, series->at(i_pickup).y());
        //highlightedPointSeries->append(dataPoints[i_pickup]);
       // drawTangent(i_pickup);
    }

    //return (minDistance < 5.0) ? closestIndex : -1;  // 5.0 為選取的距離閾值
    return (i_pickup);
}



void Chart::showContextMenu(const QPoint &pos) {
    QMenu *menu = new QMenu(this);
    QAction *pickupAction = menu->addAction("PickUp");
    QAction *setAction = menu->addAction("Set");
    QAction *clearAction = menu->addAction("Clear");
    //menu->addAction(action);
    //menu->exec(mapToGlobal(pos));//QCursor::pos()
    QAction *selectedAction = menu->exec(mapToGlobal(pos));
    if (selectedAction == pickupAction)
    {
        is_pickup_a_point = true;
        /*int selectedPointIndex_now = findClosestPoint(pos);
        if (selectedPointIndex!=findClosestPoint(pos))
        {
            qDebug("Mouse is not on the pick up point !! dtat_i:%d => %d ", selectedPointIndex, selectedPointIndex_now);
            selectedPointIndex=0xFFFF;
        } else
        {
            qDebug("is_pick_a_point = true");
            is_pickup_a_point = true;
        }*/

    } else
    if (selectedAction == setAction)
    {
        qDebug("setPoint()");
        if(is_pickup_a_point==true) is_pickup_a_point=false;
        selectedPointIndex = 0xFFFF;
        highlightedPointSeries->clear();

    } else
    if (selectedAction == clearAction)
    {
        qDebug("clearSelection()");
        if(is_pickup_a_point==true) is_pickup_a_point=false;
        highlightedPointSeries->clear();
    }
}


void Chart::drawTangent(int index) {
    if (index <= 0 || index >= dataPoints.size() - 1) return;

    // 計算切線
    QPointF prevPoint = dataPoints[index - 1];
    QPointF nextPoint = dataPoints[index + 1];
    double slope = (nextPoint.y() - prevPoint.y()) / (nextPoint.x() - prevPoint.x());

    double tangentX1 = dataPoints[index].x() - 10;
    double tangentX2 = dataPoints[index].x() + 10;
    double tangentY1 = dataPoints[index].y() - slope * 10;
    double tangentY2 = dataPoints[index].y() + slope * 10;

    if (!tangentLine) {
        tangentLine = new QGraphicsLineItem(chart());
        tangentLine->setPen(QPen(Qt::blue, 1, Qt::DashLine));
    }
    tangentLine->setLine(tangentX1, tangentY1, tangentX2, tangentY2);
    tangentLine->show();
}


