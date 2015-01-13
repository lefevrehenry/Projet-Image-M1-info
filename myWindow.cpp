#include "myWindow.h"
#include "BlurDialog.h"
#include "ui_myWindow.h"
#include "Histogramme.h"
#include <QPixmap>

myWindow::myWindow() : QMainWindow(0), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img = new QImage();
    filename = "";

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    //ui->graphicsView->setMouseTracking(true);
    //ui->graphicsView->show();
    initMenu();
    //repeindre();
    ui->toolBar->toolButtonStyle();
    initBarreOutils();
    QDesktopWidget desktop;// = new QDesktopWidget;
    int xScreen = desktop.screenGeometry().width();
    int yScreen = desktop.screenGeometry().height();
    resize(xScreen / 2, yScreen / 2);
    move((xScreen - width()) / 2, (yScreen - height()) / 2);

}

myWindow::myWindow(QString url) : myWindow()
{
    open(url);
}

myWindow::~myWindow()
{
    delete img;
    delete ui;
}

void myWindow::repeindre()
{
    scene->clear();
    //ui->graphicsView->setBackgroundBrush(Qt::red);
    ui->graphicsView->setImage(img);
    scene->addPixmap(QPixmap::fromImage(*img));
    scene->setSceneRect(0,0,img->width(),img->height());
    //ui->graphicsView->setScene(scene);
    //ui->graphicsView->show();
}

/* Ouvrir */
bool myWindow::openFilename()
{
    QString filename = QFileDialog::getOpenFileName(this,
        "Ouvrir une image", QDir::currentPath() + "/../Projet-Image-M1-info/ressources", "Image Files (*.png *.jpg)");
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

void myWindow::paintEvent(QPaintEvent *)
{
    /*QPainter painter(this);

    int x = 0;
    int y = 0;
    if (width() > img->width())
    {
        x = (width() - img->width()) / 2;
    }
    if (height() > img->height())
    {
        y = (height() - img->height()) / 2;
    }

    painter.drawImage(x,y, *img);
    painter.end();*/

    //repeindre();
}

void myWindow::initMenu()
{
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");

    QAction *actionOuvrir = new QAction("&Ouvrir",this);
    QAction *actionSauvegarder = new QAction("&Sauvegarder",this);
    QAction *actionSauvegarderSous = new QAction("&Sauvegarder sous...",this);
    QAction *actionQuitter = new QAction("&Quitter", this);
    menuFichier->addAction(actionOuvrir);
    menuFichier->addAction(actionSauvegarder);
    menuFichier->addAction(actionSauvegarderSous);
    menuFichier->addAction(actionQuitter);

    QMenu *menuEdition =  menuBar()->addMenu("&Edition");

    QAction *actionHistogramme = new QAction("&Histogramme",this);
    QAction *actionNiveauDeGris = new QAction("&NiveauDeGris",this);
    QAction *actionFlouter = new QAction("&Flouter",this);
    QAction *actionFusion = new QAction("&Fusion",this);
    QAction *actionRedimensionner = new QAction("&Redimensionner",this);
    QAction *actionFiltre = new QAction("&Filtre",this);
    QAction *actionContours = new QAction("&Contours",this);
    QAction *actionRedimIntell = new QAction("&RedimIntell",this);
    QAction *actionGrabCut = new QAction("&GrabCut",this);
    QAction *actionRogner = new QAction("&Rogner",this);
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

    QMenu *menuOutils =  menuBar()->addMenu("&Outils");

    QAction *actionPipette = new QAction("&Pipette",this);
    QAction *actionSelection = new QAction("&Selection",this);

    menuOutils->addAction(actionPipette);
    menuOutils->addAction(actionSelection);
    //this->setMenuBar(topLevelWidget());
    menuBar()->addAction(menuBar()->addSeparator());
   // menuBar()->setNativeMenuBar(true);


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

    QObject::connect(actionPipette,SIGNAL(triggered()),this,SLOT(pipette()));
    QObject::connect(actionSelection,SIGNAL(triggered()),this,SLOT(selection()));
}

void myWindow::initBarreOutils()
{
    QObject::connect(ui->actionPipette,SIGNAL(triggered()),this,SLOT(pipette()));
    QObject::connect(ui->actionSelection,SIGNAL(triggered()),this,SLOT(selection()));

}

/* Sauvegarder */
bool myWindow::sauvegarder()
{
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
    Histogramme h(ui->graphicsView);
    return true;
}

/*passe l'image en niveau de gris*/
bool myWindow::gris()
{
    QRgb pixel;
    int i, j, tmp;
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
    return true;
}

bool myWindow::filtre()
{
    return true;
}

bool myWindow::contours()
{
    return true;
}

bool myWindow::redimIntell()
{
    return true;
}

bool myWindow::grabCut()
{
    return true;
}

/* rogne la selection de l'image*/
bool myWindow::rogner()
{
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
        repeindre();
        return true;
    }else{
        return false;
    }
}

bool myWindow::pipette()
{
   ui->graphicsView->setModePipette();
   if(ui->actionSelection->isChecked()){
       ui->actionSelection->setChecked(false);
    }

    return true;
}

bool myWindow::selection()
{
    ui->graphicsView->setModeSelection();
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
