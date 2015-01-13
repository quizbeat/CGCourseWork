#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    path = "/Users/air/Documents/Computer Graphics/CGCourseWork/taj_mahal.jpg";
    loadImage();

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

        for (int x = 0; x < image.width() - frameWidth + 1; x++) {
            for (int y = 0; y < image.height() - frameHeight + 1; y++) {

                int currentPixel = 0;
                for (int i = 0; i < frameWidth; i++) {
                    for (int j = 0; j < frameHeight; j++) {
                        pixels[currentPixel].color = QColor(prevImage.pixel(QPoint(x + i, y + j)));
                        pixels[currentPixel].num = currentPixel;
                        currentPixel++;
                    }
                }

                QColor newPixel = getMedian(pixels);
                image.setPixel(x + frameOffsetByX, y + frameOffsetByY, newPixel.rgb());
            }
        }
        prevImage = image;

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

void Widget::loadImage() {
    image.load(path.c_str());
    image.scaled(image.width(), image.height());
    originalImage = image;
    prevImage = image;
}

void Widget::on_applyFilterButton_clicked() {
    if ((frameWidth * frameHeight) % 2 == 0) {
        QMessageBox error;
        error.setText("Amount of pixels in the frame must be odd.");
        error.setStandardButtons(QMessageBox::Ok);
        if (error.exec() == QMessageBox::Ok){
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
    prevImage = originalImage;
    update();
}

void Widget::on_selectImageBox_currentIndexChanged(int img) {
    if (img == 0) {
        path = "/Users/air/Documents/Computer Graphics/CGCourseWork/taj_mahal.jpg";
    }
    else if (img == 1) {
        path = "/Users/air/Documents/Computer Graphics/CGCourseWork/doge.jpg";
    }
    else if (img == 2) {
        path = "/Users/air/Documents/Computer Graphics/CGCourseWork/dog.png";
    }
    else if (img == 3) {
        path = "/Users/air/Documents/Computer Graphics/CGCourseWork/flower.bmp";
    }
    else if (img == 4) {
        path = "/Users/air/Documents/Computer Graphics/CGCourseWork/coins.png";
    }
    else if (img == 5) {
        path = "/Users/air/Documents/Computer Graphics/CGCourseWork/people2.jpg";
    }
    loadImage();
    update();
}
