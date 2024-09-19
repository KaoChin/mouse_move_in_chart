#ifndef CHART_H
#define CHART_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QMouseEvent>
#include <QGraphicsTextItem>
#include <QPointF>
#include <QTime>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


class Chart : public QChartView
{
    Q_OBJECT
public:
    explicit Chart(QWidget *parent = nullptr);
    ~Chart();

    void setupChart();
    void plotSpline();
protected:
    void mouseMoveEvent(QMouseEvent *event) override;  // 處理滑鼠移動事件
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool sceneEvent(QEvent *event);

private:
    QSplineSeries *series;             // 用於繪製樣條曲線
    QScatterSeries *highlightedPointSeries;
    //QGraphicsEllipseItem *highlightedPoint;
    QGraphicsLineItem *tangentLine; // 切線
    QVector<QPointF> dataPoints;        // 曲線的數據點

    bool is_pickup_a_point = false;
    int selectedPointIndex =0xFFFF;
    QTime mouse_l_press_start_Time;
    int mouse_l_press_elapsed_time =0;// 被拖動的點的索引GraphicsTextItem *mousePositionText;  // 顯示滑鼠位置的文本
    int dx=10;

    QGraphicsTextItem *mousePositionText;

    void updateSpline();                       // 更新曲線
    void drawTangent(int index);
    int findClosestPoint(const QPointF &pos);  // 找到離滑鼠最近的點
    void showContextMenu(const QPoint &pos); // 顯示右鍵菜單

    //bool gestureEvent(QGestureEvent *event);
    //void treatment_for_mouse_btn_l_is_hold();



};

#endif // CHART_H
