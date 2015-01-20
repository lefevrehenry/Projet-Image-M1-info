#include "Convolution.h"

Convolution::Convolution(int taille, int init)
{
    //m = new Matrix(taille,init);
    sommeCoefficient = m.sommeCoefficient();
}

Convolution::Convolution(Matrix m){
    this->m = m;
    sommeCoefficient = m.sommeCoefficient();
}

Convolution::~Convolution(){

}

void Convolution::remplirMatrix(int val)
{
    m.remplir(val);
    sommeCoefficient = m.sommeCoefficient();
}

void Convolution::modifierCaseMatrix(int i, int j, int val)
{
    m.insert_element(i,j,val);
}

void Convolution::redimensionnerMatrix(int i, int val)
{
    m.resize(i);
    remplirMatrix(val);
}

void Convolution::retournerMatrix()
{
    m.retourner();
}

void Convolution::convolution(QImage *image)
{
    int sommeR;
    int sommeG;
    int sommeB;
    int nbPixel;
    QRgb pixel;
    QImage imageCopie(*image);
    retournerMatrix();
    if(m.estPaire()){
        int decalage = m.getSize()/2;
        for(int x = 0; x < image->width(); x++)
        {
            for(int y = 0; y < image->height(); y++)
            {
                sommeR = 0;
                sommeG = 0;
                sommeB = 0;
                nbPixel = 0;
                for(int i = -decalage; i <= decalage-1; i++)
                {
                    for(int j = -decalage; j <= decalage-1; j++)
                    {
                        if (x+j >= 0 && x+j < image->width() && y+i >= 0 && y+i < image->height()) {
                            pixel = image->pixel(x+j,y+i);
                            sommeR += qRed(pixel)*m.get_element(i+decalage,j+decalage);
                            sommeG += qGreen(pixel)*m.get_element(i+decalage,j+decalage);
                            sommeB += qBlue(pixel)*m.get_element(i+decalage,j+decalage);
                            nbPixel += m.get_element(i+decalage,j+decalage);
                        }
                    }
                }
                if (nbPixel != 0) {
                    sommeR/=nbPixel;
                    sommeG/=nbPixel;
                    sommeB/=nbPixel;
                }
                ajusterCouleur(&sommeR);
                ajusterCouleur(&sommeG);
                ajusterCouleur(&sommeB);
                imageCopie.setPixel(x, y, qRgb(sommeR,sommeG,sommeB));
            }
        }
    }else{
        int decalage = m.getSize()/2;
        for(int x = 0; x < image->width(); x++)
        {
            for(int y = 0; y < image->height(); y++)
            {
                sommeR = 0;
                sommeG = 0;
                sommeB = 0;
                nbPixel = 0;
                for(int i = -decalage; i <= decalage; i++)
                {
                    for(int j = -decalage; j <= decalage; j++)
                    {
                        if (x+j >= 0 && x+j < image->width() && y+i >= 0 && y+i < image->height()) {
                            pixel = image->pixel(x+j,y+i);
                            sommeR += qRed(pixel)*m.get_element(i+decalage,j+decalage);
                            sommeG += qGreen(pixel)*m.get_element(i+decalage,j+decalage);
                            sommeB += qBlue(pixel)*m.get_element(i+decalage,j+decalage);
                            nbPixel += m.get_element(i+decalage,j+decalage);
                        }
                    }
                }
                if (nbPixel != 0) {
                    sommeR/=nbPixel;
                    sommeG/=nbPixel;
                    sommeB/=nbPixel;
                }
                ajusterCouleur(&sommeR);
                ajusterCouleur(&sommeG);
                ajusterCouleur(&sommeB);
                imageCopie.setPixel(x, y, qRgb(sommeR,sommeG,sommeB));
            }
        }
    }

    image->swap(imageCopie);
    retournerMatrix();
}

void Convolution::ajusterCouleur(int *couleur)
{
    if(*couleur > 255)
        *couleur = 255;
    else if(*couleur < 0)
        *couleur = 0;
}

Matrix* Convolution::convolution(Matrix *mat){
    retournerMatrix();
    Matrix *res = mat->copie();
    int somme,nbCase;
    if(m.estPaire()){
        int decalage = m.getSize()-1;
        for(int x = 0; x < mat->getSize();x++){
            for(int y = 0;y<mat->getSize();y++){
                somme =0;
                nbCase=0;
                for(int i = -decalage; i <= 0; i++)
                {
                    for(int j = -decalage; j <= 0; j++)
                    {
                        if (y+j >= 0 && y+j < mat->getSize() && x+i >= 0 && x+i < mat->getSize())
                        {
                            if(i+decalage >=0 && j+decalage>=0)
                            {
                                somme += mat->get_element(y+j,x+i)*m.get_element(i+decalage,j+decalage);
                                nbCase+= m.get_element(i+decalage,j+decalage);
                            }
                        }
                    }
                }
                res->insert_element(x,y,somme);
            }
        }

    }else{
        int decalage = m.getSize()/2;
        for(int x = 0; x < mat->getSize(); x++)
        {
            for(int y = 0; y < mat->getSize(); y++)
            {
                somme = 0;
                nbCase = 0;
                for(int i = -decalage; i <= decalage; i++)
                {
                    for(int j = -decalage; j <= decalage; j++)
                    {
                        if (y+j >= 0 && x+j < mat->getSize() && x+i >= 0 && x+i < mat->getSize())
                        {
                            cout << m.get_element(i+decalage,j+decalage)<<endl;
                            somme += mat->get_element(y+j,x+i)*m.get_element(i+decalage,j+decalage);
                            nbCase+= m.get_element(i+decalage,j+decalage);

                        }
                    }
                }
                res->insert_element(y,x,somme);
            }
        }
    }

    retournerMatrix();
    mat = res->copie();
    delete res;
    return mat;
}

void Convolution::convolutionCascade(Matrix *noyau,Matrix *mat, int taille){
    m = *noyau;
    mat->agrandir(1);
    mat = convolution(mat);
    if(mat->getSize()!=taille){
        convolutionCascade(noyau,mat,taille);
    }else{
        m = *mat;
    }
}

void Convolution::convolutionMedian(QImage *image,int taille)
{
    QImage imageCopie(*image);
        for(int x = 0; x < image->width(); x++)
        {
            for(int y = 0; y < image->height(); y++)
            {
                if (x+taille <= image->width() && y+taille <= image->height()) {
                            int med = getMediane(y,x,taille,&imageCopie);
                            image->setPixel(x+(taille/2+1),y+(taille/2+1),qRgb(med,med,med));

                }
            }
        }


    //image->swap(imageCopie);
}

void Convolution::setNoyau(Matrix* noyau){
    m = *(noyau->copie());
}


int* Convolution::lineariser(int debI, int debJ,int taille,QImage* img){
    int* tableau = new int[taille*taille];
    int k=0;
    for(int i=debI;i<debI+taille;i++){
        for(int j=debJ;j<debJ+taille;j++){
            /*ON PREND LA VALEUR DE GRIS DE L'IMAGE*/
            tableau[k]= qGray(img->pixel(j,i));
            k++;
        }
    }
    return tableau;
}

int Convolution::getMedianeFromTab(int* tab,int taille){
    return tab[((taille*taille)/2)];
}

int Convolution::getMediane(int debI,int debJ,int taille,QImage *img){

    int* tab = lineariser(debI,debJ,taille,img);
    tab = trierTableau(tab,taille);

    bool tab_en_ordre = false;
    int N = taille*taille;
    while(!tab_en_ordre)
    {
        tab_en_ordre = true;
        for(int i=0 ; i < N-1 ; i++)
        {
            if(tab[i] > tab[i+1])
            {
                swap(tab[i],tab[i+1]);
                tab_en_ordre = false;
             }
        }
        N--;
    }

    return getMedianeFromTab(tab,taille);

}

int* Convolution::trierTableau(int* tab,int taille){

    bool tab_en_ordre = false;
    int N = taille*taille;
    while(!tab_en_ordre)
    {
        tab_en_ordre = true;
        for(int i=0 ; i < N-1 ; i++)
        {
            if(tab[i] > tab[i+1])
            {
                swap(tab[i],tab[i+1]);
                tab_en_ordre = false;
             }
        }
        N--;
    }
    return tab;

}
