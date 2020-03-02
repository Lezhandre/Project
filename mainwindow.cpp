#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QDebug>
#include "function.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    pictHeight = 621;
    pictWidth = 1021;
    step = 0.1;
    leftX = -100; rightX = 100;
    leftY = -100; rightY = 100;
    drawGraph();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::recountPixels() {
    onePixelX = pictWidth / (rightX - leftX);
    onePixelY = pictHeight / (rightY - leftY);
    Ox = leftX; Oy = rightY;
}

void MainWindow::getData() {
    leftX = ui->inputLeftX->text().toDouble();
    rightX = ui->inputRightX->text().toDouble();
    f.get_fun((ui->inputLeftX_4->text()).toStdString());
    leftY = f.min(leftX, rightX);
    rightY = f.max(leftX, rightX);
    step = (rightX - leftX) / pictWidth;
}

void MainWindow::drawGraph(bool notEmpty) {
    QPixmap graph(pictWidth, pictHeight);
    QPainter paint;
    paint.begin(&graph);
    paint.eraseRect(0, 0, pictWidth, pictHeight);
    if (-Ox >= 0)
        paint.drawLine((-Ox) * onePixelX, 0, (-Ox) * onePixelX, pictHeight);
    if (Oy >= 0)
        paint.drawLine(0, Oy * onePixelY, pictWidth, Oy * onePixelY);

    paint.setPen(QPen(Qt::black, 2));
    for (double i = 0; i <= rightX && Oy >= 0; i += 2.0)
        paint.drawPoint((i - Ox) * onePixelX, Oy * onePixelY);
    for (double i = 0; i >= leftX && Oy >= 0; i -= 2.0)
        paint.drawPoint((i - Ox) * onePixelX, Oy * onePixelY);
    for (double i = 0; i <= rightY && -Ox >= 0; i += 2.0)
        paint.drawPoint((-Ox) * onePixelX, (-i + Oy) * onePixelY);
    for (double i = 0; i >= leftY && -Ox >= 0; i -= 2.0)
        paint.drawPoint((-Ox) * onePixelX, (-i + Oy) * onePixelY);

    if (!notEmpty) {
        paint.end();
        ui->outputGraph->setPixmap(graph);
        return;
    }

    paint.setPen(QPen(Qt::green, 1, Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    double i = leftX + step;
    path.moveTo((i - Ox) * onePixelX, (-f.fun_val(i) + Oy) * onePixelY);
    for (; i <= rightX; i += step) {
        path.lineTo((i - Ox) * onePixelX, (-f.fun_val(i) + Oy) * onePixelY);
    }
    paint.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    paint.drawPath(path);
    paint.end();
    ui->outputGraph->setPixmap(graph);
    return;
}

void MainWindow::on_exit_clicked() {
    this->close();
}

void MainWindow::on_clear_clicked() {
    recountPixels();
    drawGraph();
}

void MainWindow::on_draw_clicked() {
    getData();
    recountPixels();
    drawGraph(1);
}
