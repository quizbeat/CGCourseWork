#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    path = "/Users/air/Documents/Computer Graphics/CGCourseWork/doge2.jpg";
    loadImage(path);

    ui->setupUi(this);

    frameWidth = ui->frameWidthBox->value();
    frameHeight = ui->frameHeightBox->value();
    intensity = ui->filterIntensityBox->value();
}

Widget::~Widget() {
    delete ui;
}

bool lightnessComparator(const mLightness &light1, const mLightness &light2) {
    return light1.lightness > light2.lightness;
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), image);
}

void Widget::medianFiltration() {

    vector<mQColor> pixels(frameWidth * frameHeight);

    int frameOffsetByX = frameWidth / 2;
    int frameOffsetByY = frameHeight / 2;

    for (int k = 0; k < intensity; k++) {

        for (int x = frameOffsetByX; x < image.width() - frameOffsetByX; x++) {
            for (int y = frameOffsetByY; y < image.height() - frameOffsetByY; y++) {

                int pos = 0;
                for (int i = -frameOffsetByX; i < frameWidth - frameOffsetByX; i++) {
                    for (int j = -frameOffsetByY; j < frameHeight - frameOffsetByY; j++) {
                        pixels[pos].color = QColor(image.pixel(QPoint(x + i, y + j)));
                        pixels[pos].num = pos;
                        pos++;
                    }
                }

                QColor avg = getMedian(pixels);
                image.setPixel(x, y, avg.rgb());
            }
        }

    }
}

QColor Widget::getMedian(vector<mQColor> &pixels) {

    int amount = pixels.size();
    QColor median;
    vector<mLightness> lights(amount);

    for (int i = 0; i < amount; i++) {
        lights[i].lightness = pixels[i].color.lightness();
        lights[i].num = pixels[i].num;
    }

    sort(lights.begin(), lights.end(), lightnessComparator);

    int middle = amount / 2 + 1;

    for (int i = 0; i < amount; i++) {
        if (pixels[i].num == lights[middle].num) {
            median = pixels[i].color;
        }
    }

    return median;
}

void Widget::loadImage(string path) {
    image.load(path.c_str());
    image.scaled(image.width(), image.height());
    originalImage = image;
}

void Widget::on_applyFilterButton_clicked() {
    if ((frameWidth * frameHeight) % 2 == 0) {
        QMessageBox error;
        error.setText("Amount of pixels in the frame must be odd.");
        error.setStandardButtons(QMessageBox::Ok);
        if(error.exec() == QMessageBox::Ok){
            error.close();
            return;
        }
    }
    medianFiltration();
    update();
}

void Widget::on_quitButton_clicked() {
    close();
}

void Widget::on_filterIntensityBox_valueChanged() {
    intensity = ui->filterIntensityBox->value();
}

void Widget::on_frameWidthBox_valueChanged() {
    frameWidth = ui->frameWidthBox->value();
}

void Widget::on_frameHeightBox_valueChanged() {
    frameHeight = ui->frameHeightBox->value();
}

void Widget::on_revertImageButton_clicked() {
    image = originalImage;
    update();
}
