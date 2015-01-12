#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    path = "/Users/air/Documents/Computer Graphics/CGCourseWork/doge4.bmp";
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
    mQColor item;

    int frameOffsetByX = frameWidth / 2;
    int frameOffsetByY = frameHeight / 2;

    /*
    for (int k = 0; k < intensity; k++) {

        for (int x = 0; x < image.width() - frameWidth + 1; x++) {
            for (int y = 0; y < image.height() - frameHeight + 1; y++) {

                int currentPixel = 0;
                for (int i = 0; i < frameWidth; i++) {
                    for (int j = 0; j < frameHeight; j++) {
                        pixels[currentPixel++] = QColor(image.pixel(QPoint(x + i, y + j)));
                    }
                }

                QColor newPixel = getMedian(pixels);
                image.setPixel(x + frameOffsetByX, y + frameOffsetByY, newPixel.rgb());
            }
        }

    }
    */
    /*
    for (int k = 0; k < intensity; k++) {

        for (int x = 0; x < image.width() - 2; x++) {
            for (int y = 0; y < image.height() - 2; y++) {

                //int currentPixel = 0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        pixels.push_back(QColor(image.pixel(QPoint(x + i, y + j))));
                    }
                }

                QColor newPixel = getMedian(pixels);
                image.setPixel(x + 1, y + 1, newPixel.rgb());
                pixels.clear();
            }
        }

    }
    */

    for (int i = 0; i < intensity; i++) {

            for (int x = 1; x < image.width(); x++) {
                for (int y = 1; y < image.height(); y++) {

                    int pos = 0;
                    for (int i = 0; i < 3; i++)
                        for (int j = 0; j < 3; j++) {
                             pixels[pos].color = QColor(image.pixel(QPoint(x + i, y + j)));
                             pixels[pos].num = pos;
                             pos++;
                        }
                    QColor avg = getMedian(pixels);
                    image.setPixel(x, y, avg.rgb());
                }
            }

        }

}

qreal Widget::getLightness(QColor pixel) {
    return 0.21 * pixel.red() + 0.72 * pixel.green() + 0.07 * pixel.blue();
}

QColor Widget::getMedian(vector<mQColor> &pixels) {
    /*
    QColor median;

    int amount = pixels.size();
    int middle = amount / 2 + 1;

    int medianR = 0;
    int medianG = 0;
    int medianB = 0;

    vector<int> pixelsR(amount);
    for (int i = 0; i < amount; i++) {
        pixelsR[i] = pixels[i].red();
    }
    sort(pixelsR.begin(), pixelsR.end());
    medianR = pixelsR[middle];

    vector<int> pixelsG(amount);
    for (int i = 0; i < amount; i++) {
        pixelsG[i] = pixels[i].green();
    }
    sort(pixelsG.begin(), pixelsG.end());
    medianG = pixelsG[middle];

    vector<int> pixelsB(amount);
    for (int i = 0; i < amount; i++) {
        pixelsB[i] = pixels[i].blue();
    }
    sort(pixelsB.begin(), pixelsB.end());
    medianB = pixelsB[middle];

    median = qRgb(medianG, medianR, medianB);

    return median;
    */

    int amount = pixels.size();
    QColor median;
    vector<mLightness> lights(amount);

    for (int i = 0; i < 9; i++) {
        lights[i].lightness = pixels[i].color.lightness();
        lights[i].num = pixels[i].num;
    }

    sort(lights.begin(), lights.end(), lightnessComparator);

    int middle = (amount % 2) ? (amount / 2 + 1) : (amount / 2);

    for (int i = 0; i < 9; i++) {
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
