#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

#include "myWindow.h"
#include <QImage>
class Histogramme : public QImage
{

public:
    Histogramme(QImage *image);
    ~Histogramme();

    void afficheHisto();
    void compterPixel();
    void afficherLignes();
    void afficherLigne(int pixels[256], Qt::GlobalColor c);
    void etalement(int v0, int v1, int vMin = 0, int vMax = 255);

    QImage *getImg();

private:
    QImage *img;

    int distanceBord;
    int nombreEspacementAbscisse;
    int nombreEspacementOrdonnee;

    int valeurMax;

    int nbPixelRouge[256];
    int nbPixelBleu[256];
    int nbPixelVert[256];

    int longueurAbscisse;
    int longueurOrdonnee;



};

#endif // HISTOGRAMME_H