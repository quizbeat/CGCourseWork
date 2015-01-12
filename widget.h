#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QDebug>
 #include <QMessageBox>
#include <algorithm>

using namespace std;

typedef struct {
    QColor color;
    int num;
} mQColor;

typedef struct {
    qreal lightness;
    int num;
} mLightness;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    void loadImage(string path);
    virtual void paintEvent(QPaintEvent *);
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_applyFilterButton_clicked();
    void on_revertImageButton_clicked();
    void on_quitButton_clicked();

    void on_filterIntensityBox_valueChanged();
    void on_frameWidthBox_valueChanged();
    void on_frameHeightBox_valueChanged();

private:
    string path;
    QImage image;
    QImage originalImage;

    int intensity;
    int frameWidth;
    int frameHeight;

    void medianFiltration();
    QColor getMedian(vector<mQColor> &pixels);
    void revertToOriginal();

    Ui::Widget *ui;
};

#endif // WIDGET_H
