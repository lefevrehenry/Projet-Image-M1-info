#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QImage>
#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <QString>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QObject>
#include <QtWidgets>
#include <iostream>
#include <QApplication>
using namespace std;

class myWindow : public QMainWindow {

    Q_OBJECT
public:
    // Methodes
    myWindow(QWidget *parent = 0);
    myWindow(QString url, QWidget *parent = 0);
    ~myWindow();

    bool saveAsFilename();
    bool openFilename();
    bool open(QString url);
    bool save(QString url);
    void initMenu();

public slots:
    bool ouvrir();
    bool sauvegarder();
    bool sauvegarderSous();
    void quitter();
    bool histo();
    bool gris();
    bool flouter();
    bool fusionner();
    bool redimensionner();
    bool filtre();
    bool contours();
    bool redimIntell();
    bool grabCut();
    bool rogner();
    bool pipette();
    bool selection();

private:
    // Methodes
    void paintEvent(QPaintEvent *);
    // Attribut
    QImage *img;

};

#endif // MYWINDOW_H
