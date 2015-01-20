#include "myWindow.h"
#include "BlurDialog.h"
#include "ui_myWindow.h"
#include "Histogramme.h"
#include "FiltreDialog.h"
#include "HistoDialog.h"
#include <QPixmap>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <QRectF>
#include "Calcul.h"

myWindow::myWindow() : QMainWindow(0), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    img = new QImage();

    filename = "";

    scene = new MyGraphicsScene(this);
    scene->setPixmapItem(itemPixmap);
    ui->graphicsView->setScene(scene);

    itemPixmap = scene->addPixmap(QPixmap::fromImage(*img));

    initMenu();
    ui->toolBar->toolButtonStyle();
    initBarreOutils();

    QDesktopWidget desktop;
    int xScreen = desktop.screenGeometry().width();
    int yScreen = desktop.screenGeometry().height();
    resize(xScreen / 2, yScreen / 2);
    move((xScreen - width()) / 2, (yScreen - height()) / 2);


    QObject::connect(scene, SIGNAL(redimensionnement(QRectF)), this, SLOT(redimensionnementIteractif(QRectF)));
}

bool myWindow::redimensionnementIteractif(QRectF rect) {
    QImage *tmp = Calcul::redimensionnementEnLargeur(img, rect.width());
    delete img;
    img = Calcul::redimensionnementEnHauteur(tmp, rect.height());
    delete tmp;
    repeindre();

    return true;
}

myWindow::myWindow(QString url) : myWindow()
{
    open(url);
}

myWindow::~myWindow()
{
    delete img;
    delete scene;
    delete ui;
}

void myWindow::repeindre()
{
    itemPixmap->setPixmap(QPixmap::fromImage(*img));

    ui->graphicsView->setImage(img);
    scene->setPixmapItem(itemPixmap);
    scene->setSceneRect(0, 0, img->width(), img->height());
    scene->update();
}

/* Ouvrir */
bool myWindow::openFilename()
{
    QString filename = QFileDialog::getOpenFileName(this,
        "Ouvrir une image", QDir::currentPath() + "/../Projet-Image-M1-info/ressources", "Image Files (*.png *.jpg *.gif)");
    if (filename != "")
    {
        return open(filename);
    }
    return false;
}

bool myWindow::open(QString url)
{
    if (img->load(url))
    {
        filename = url;
        if(img->format() <= QImage::Format_Indexed8){
            img->convertToFormat(QImage::Format_RGB32);
        }
        repeindre();
        return true;
    }
    return false;
}

/* Sauvegarder sous*/
bool myWindow::saveAsFilename()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save File",
                               "/../Projet-Image-M1-info/ressources/untitled.png",
                               "Images (*.png *.xpm *.jpg)");
    if (filename != "")
    {
        return save(filename);
    }
    return false;
}

bool myWindow::save(QString url)
{
    return img->save(url, 0, -1);
}

void myWindow::initMenu()
{
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");

    actionOuvrir = new QAction("&Ouvrir",this);
    actionSauvegarder = new QAction("&Sauvegarder",this);
    actionSauvegarderSous = new QAction("&Sauvegarder sous...",this);
    actionQuitter = new QAction("&Quitter", this);
    menuFichier->addAction(actionOuvrir);
    menuFichier->addAction(actionSauvegarder);
    menuFichier->addAction(actionSauvegarderSous);
    menuFichier->addAction(actionQuitter);

    QMenu *menuEdition =  menuBar()->addMenu("&Edition");

    actionHistogramme = new QAction("&Histogramme",this);
    actionNiveauDeGris = new QAction("&NiveauDeGris",this);
    actionFlouter = new QAction("&Flouter",this);
    actionFusion = new QAction("&Fusion",this);
    actionRedimensionner = new QAction("&Redimensionner",this);
    actionFiltre = new QAction("&Filtre",this);
    actionContours = new QAction("&Contours",this);
    actionRedimIntell = new QAction("&RedimIntell",this);
    actionGrabCut = new QAction("&GrabCut",this);
    actionRogner = new QAction("&Rogner",this);
    menuEdition->addAction(actionHistogramme);
    menuEdition->addAction(actionNiveauDeGris);
    menuEdition->addAction(actionFlouter);
    menuEdition->addAction(actionFusion);
    menuEdition->addAction(actionRedimensionner);
    menuEdition->addAction(actionFiltre);
    menuEdition->addAction(actionContours);
    menuEdition->addAction(actionRedimIntell);
    menuEdition->addAction(actionGrabCut);
    menuEdition->addAction(actionRogner);

    menuBar()->addAction(menuBar()->addSeparator());


    QObject::connect(actionOuvrir,SIGNAL(triggered()),this,SLOT(openFilename()));
    QObject::connect(actionSauvegarder,SIGNAL(triggered()),this,SLOT(sauvegarder()));
    QObject::connect(actionSauvegarderSous,SIGNAL(triggered()),this,SLOT(saveAsFilename()));
    QObject::connect(actionQuitter,SIGNAL(triggered()),this,SLOT(quitter()));

    QObject::connect(actionHistogramme,SIGNAL(triggered()),this,SLOT(histo()));
    QObject::connect(actionNiveauDeGris,SIGNAL(triggered()),this,SLOT(gris()));
    QObject::connect(actionFlouter,SIGNAL(triggered()),this,SLOT(flouter()));
    QObject::connect(actionFusion,SIGNAL(triggered()),this,SLOT(fusionner()));
    QObject::connect(actionRedimensionner,SIGNAL(triggered()),this,SLOT(redimensionner()));
    QObject::connect(actionFiltre,SIGNAL(triggered()),this,SLOT(filtre()));
    QObject::connect(actionContours,SIGNAL(triggered()),this,SLOT(contours()));
    QObject::connect(actionRedimIntell,SIGNAL(triggered()),this,SLOT(redimIntell()));
    QObject::connect(actionGrabCut,SIGNAL(triggered()),this,SLOT(grabCut()));
    QObject::connect(actionRogner,SIGNAL(triggered()),this,SLOT(rogner()));
}

void myWindow::initBarreOutils()
{
    QObject::connect(ui->actionPipette,SIGNAL(triggered()),this,SLOT(pipette()));
    QObject::connect(ui->actionSelection,SIGNAL(triggered()),this,SLOT(selection()));
    QObject::connect(ui->actionRedimensionner,SIGNAL(triggered()),this,SLOT(redimension()));
    actionRogner->setEnabled(false);
}

/* Sauvegarder */
bool myWindow::sauvegarder()
{

    actionRogner->setEnabled(false);
    ui->graphicsView->cacherSelect();
    save(filename);
    return true;
}

void myWindow::quitter(){
    /*êtes vous sur ?*/
    qApp->quit();
}

/*affiche/edite l'histogramme*/
bool myWindow::histo()
{

    actionRogner->setEnabled(false);
    ui->graphicsView->cacherSelect();
    HistoDialog histoDial(img);
    if (histoDial.exec() == QDialog::Accepted)
    {
        repeindre();
        return true;
    }
    return false;
}

/*passe l'image en niveau de gris*/
bool myWindow::gris()
{

    actionRogner->setEnabled(false);
    ui->graphicsView->cacherSelect();
    QRgb pixel;
    int i, j;
    float tmp;
    int h = img->height();
    int w = img->width();
    for(i = 0; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            pixel = img->pixel(i, j);
            tmp = 0.299*qRed(pixel) + 0.587*qGreen(pixel) + 0.114*qBlue(pixel);
            pixel = qRgba(tmp, tmp, tmp, qAlpha(pixel));
            img->setPixel(i, j, pixel);
        }
    }
    repeindre();
    return true;
}

/*floute l'image*/
bool myWindow::flouter()
{

    actionRogner->setEnabled(false);
    ui->graphicsView->cacherSelect();
    BlurDialog blurDiag(img);
    if (blurDiag.exec() == QDialog::Accepted)
    {
        repeindre();
        return true;
    }
    return false;
}

/*permet de selectionner 2 images et de les fusionner*/
bool myWindow::fusionner()
{

    actionRogner->setEnabled(false);
    ui->graphicsView->cacherSelect();
    FusionDialog fusionDialog(img);
    if (fusionDialog.exec() == QDialog::Accepted)
    {
        repeindre();
        return true;
    }
    return false;
}

bool myWindow::redimensionner()
{

    actionRogner->setEnabled(false);
    ui->graphicsView->cacherSelect();
    ScaleDialog scaleDialog(img);;
    if (scaleDialog.exec() == QDialog::Accepted) {
        repeindre();
        return true;
    }
    return false;
}

bool myWindow::filtre()
{

    actionRogner->setEnabled(false);
    ui->graphicsView->cacherSelect();
    FiltreDialog filtreDialog(img);
    if (filtreDialog.exec() == QDialog::Accepted)
    {
        repeindre();
        return true;
    }
    return false;
}

bool myWindow::contours()
{

    actionRogner->setEnabled(false);
    ui->graphicsView->cacherSelect();
    Convolution c;
   c.redimensionnerMatrix(3,0);

   Matrix *noyau = new Matrix(3,0);
   noyau->insert_element(0,0,0);
    noyau->insert_element(0,1,-1);
    noyau->insert_element(0,2,0);
   noyau->insert_element(1,0,-1);
   noyau->insert_element(1,1,5);
   noyau->insert_element(1,2,-1);
    noyau->insert_element(2,0,0);
   noyau->insert_element(2,1,-1);
    noyau->insert_element(2,2,0);
    c.setNoyau(noyau);
/*
    c.modifierCaseMatrix(0,1,1);
    c.modifierCaseMatrix(2,1,1);
    c.modifierCaseMatrix(1,0,1);
    c.modifierCaseMatrix(2,2,1);
    c.modifierCaseMatrix(1,1,-4);*/
    c.convolution(img);
    repeindre();
    return true;
}

bool myWindow::redimIntell()
{

    actionRogner->setEnabled(false);
    ui->graphicsView->cacherSelect();
    return true;
}

bool myWindow::grabCut()
{

    actionRogner->setEnabled(false);
    ui->graphicsView->cacherSelect();
/*if(img->format() == QImage::Format_RGB32){
    img->convertToFormat(QImage::Format_Indexed8);
}*/


    //cv::grabCut();

    return true;
}

/* rogne la selection de l'image*/
bool myWindow::rogner()
{
    if(ui->graphicsView->modeSelection()){
        if(ui->graphicsView->getPret()){

            QPoint HG = ui->graphicsView->getHG();
            QPoint BD = ui->graphicsView->getBD();
            /*Si selection dépasse de l'image*/
            if(BD.x()<0){
                BD.setX(0);
            }
            if(HG.x()<0){
                HG.setX(0);
            }
            if(BD.y()<0){
                BD.setY(0);
            }
            if(HG.y()<0){
                HG.setY(0);
            }

            if(BD.x() > img->width()){
              /*on recadre à la limite*/
               BD.setX(img->width());
            }

            if(BD.y() > img->width()){
              /*on recadre à la limite*/
                BD.setY(img->width());
            }

            if(HG.x() > img->width()){
              /*on recadre à la limite*/
                HG.setX(img->width());
            }

            if(HG.y() > img->width()){
              /*on recadre à la limite*/
                HG.setY(img->width());
            }
            QRect rect(HG,BD);
            *img = img->copy(rect);
            ui->graphicsView->cacherSelect();
            ui->graphicsView->setPret(false);
            repeindre();
            return true;

        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool myWindow::pipette()
{
    if (ui->actionPipette->isChecked()) {
        ui->graphicsView->setModePipette();
    } else {
        ui->graphicsView->resetMode();
    }

    if(ui->actionSelection->isChecked()){
        actionRogner->setEnabled(false);
        ui->graphicsView->cacherSelect();
        ui->actionSelection->setChecked(false);
    }
    if (ui->actionRedimensionner->isChecked()) {
        ui->actionRedimensionner->setChecked(false);
        scene->disableRedimension();
    }

    return true;
}

bool myWindow::selection()
{
    if (ui->actionSelection->isChecked()) {
        ui->graphicsView->setModeSelection();
        actionRogner->setEnabled(true);
    } else {
        actionRogner->setEnabled(false);
        ui->graphicsView->cacherSelect();
    }

    if(ui->actionPipette->isChecked()){
        ui->actionPipette->setChecked(false);
    }
    if (ui->actionRedimensionner->isChecked()) {
        ui->actionRedimensionner->setChecked(false);
        scene->disableRedimension();
    }
    return true;
}

bool myWindow::redimension()
{
    if (ui->actionRedimensionner->isChecked()) {
        ui->graphicsView->setModeRedimension();
        scene->enableRedimension();
    } else {
        ui->graphicsView->resetMode();
        scene->disableRedimension();
    }

    if(ui->actionSelection->isChecked()){
        actionRogner->setEnabled(false);
        ui->graphicsView->cacherSelect();
        ui->actionSelection->setChecked(false);
    }
    if(ui->actionPipette->isChecked()){
        ui->actionPipette->setChecked(false);
    }

    return true;
}

/*
void myWindow::showMessage(const QString &message){
    statusBar()->showMessage(message);
}
*/

