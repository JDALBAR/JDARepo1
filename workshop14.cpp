
#include "workshop14.h"
#include "ui_WorkShop14.h"
#include <chrono>
#include "windows.h"
#include <iostream>
#ifdef _WIN32
#include <Windows.h>           // for windows
#else
#include <unistd.h>               // for linux
#endif
#include <cstdlib>
#include <cmath>
//#include <unistd.h>
#include <QtConcurrent>



WorkShop14::WorkShop14(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WorkShop14)
{
    ui->setupUi(this);

    QObject::connect(this,SIGNAL(updatePlot( QVector<double>, QVector<double>, QColor)),this,SLOT(plotAnimation(QVector<double>, QVector<double>, QColor)));

    myBrush1 = new QBrush(negro,Qt::SolidPattern);
    myBrush2 = new QBrush(rojo,Qt::SolidPattern);


    ui->graphicsWidget_3->addGraph();
    ui->graphicsWidget_3->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    ui->graphicsWidget_3->graph(0)->setPen(QPen(Qt::blue, 3));
    ui->graphicsWidget_3->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->graphicsWidget_3->xAxis->setVisible(false);
    ui->graphicsWidget_3->yAxis->setVisible(false);

    connect(ui->btn_Close, &QPushButton::clicked, this, &WorkShop14::endProg);
    connect(ui->btn_Animate,&QPushButton::clicked,this,&WorkShop14::animateConcurrent);
    connect(this,&WorkShop14::updateGUISignal,this,&WorkShop14::updateGUI);

    connect(ui->btn_Continue,&QPushButton::clicked,this,&WorkShop14::contConcurrent);
    connect(ui->spinBox_tMax,&QSpinBox::textChanged,this,&WorkShop14::setMaxTime);


    connect(ui->btn_Stop, &QPushButton::clicked, this, &WorkShop14::StopFunc);
    connect(ui->btn_Clear, &QPushButton::clicked, this, &WorkShop14::ClearAll);

    connect(ui->graphicsWidget_3,SIGNAL(mousePress(QMouseEvent*)),SLOT(clickedGraph_3(QMouseEvent*)));
    connect(ui->graphicsWidget_3,SIGNAL(mouseRelease(QMouseEvent*)),SLOT(releasedGraph_3(QMouseEvent*)));
    connect(ui->graphicsWidget_3,SIGNAL(mouseMove(QMouseEvent*)),SLOT(mouseIsMoving_3(QMouseEvent*)));


    gLatticeVectorA.push_back(m_latticeconst*cos(m_latticeangle));
    gLatticeVectorA.push_back(m_latticeconst*sin(m_latticeangle));
    gLatticeVectorB.push_back(m_latticeconst*cos(m_latticeangle));
    gLatticeVectorB.push_back(-m_latticeconst*sin(m_latticeangle));

    double offsetx = 0.0;
    double offsety = 0.0;
    m_nCells = CalculateNumberOfCells(nAtoms);
    int nCellsInSide = CalculateNumberOfCellsInSide(nAtoms);
    double displacement1 = 0.333333*(gLatticeVectorA[0]+gLatticeVectorB[0]);
    double displacement2 = 0.666666*(gLatticeVectorA[0]+gLatticeVectorB[0]);
    for(int i = 0; i<m_nCells;i++)
    {
        g_x.push_back(i*gLatticeVectorA[0] + offsetx+displacement1);
        g_y.push_back(i*gLatticeVectorA[1] + offsety);


        g_x.push_back(i*gLatticeVectorA[0] + offsetx + displacement2);
        g_y.push_back(i*gLatticeVectorA[1] + offsety);



        if((i+1)%nCellsInSide == 0 && i>0)
        {
            offsetx = offsetx -nCellsInSide*gLatticeVectorA[0]+gLatticeVectorB[0];
            offsety = offsety -nCellsInSide*gLatticeVectorA[1]+gLatticeVectorB[1];
        }

    }

    double sum_x = 0, sum_y = 0;
    for (int i = 0; i<2*m_nCells;i++)
    {
        sum_x = sum_x + g_x[i];
        sum_y = sum_y + g_y[i];
    }

    m_gxcenter = sum_x/(2*m_nCells);
    m_gycenter = sum_y/(2*m_nCells);

    for (int i = 0; i<2*m_nCells;i++)
    {
        g_x[i] = g_x[i] - m_gxcenter + m_xcenter;
        g_y[i] = g_y[i] - m_gycenter + m_ycenter;
    }

    g_xSaved = g_x;
    g_ySaved = g_y;

    for (int i = 0; i<g_x.size(); i++)
    {
        g_xNew.push_back(0.0);
        g_yNew.push_back(0.0);
    }






    hBNLatticeVectorA.push_back(m_latticeconsthBN*cos(m_latticeanglehBN));
    hBNLatticeVectorA.push_back(m_latticeconsthBN*sin(m_latticeanglehBN));
    hBNLatticeVectorB.push_back(m_latticeconsthBN*cos(m_latticeanglehBN));
    hBNLatticeVectorB.push_back(-m_latticeconsthBN*sin(m_latticeanglehBN));


    double offsetxhBN = 0.0;
    double offsetyhBN = 0.0;

    m_nCellshBN = CalculateNumberOfCells(nAtomshBN);
    int nCellsInSidehBN = CalculateNumberOfCellsInSide(nAtomshBN);
    double displacementhBN1 = 0.333333*(hBNLatticeVectorA[0]+hBNLatticeVectorB[0]);
    double displacementhBN2 = 0.666666*(hBNLatticeVectorA[0]+hBNLatticeVectorB[0]);
    for(int i = 0; i<m_nCellshBN;i++)
    {
        hBN_x.push_back(i*hBNLatticeVectorA[0] + offsetxhBN + displacementhBN1);
        hBN_y.push_back(i*hBNLatticeVectorA[1] + offsetyhBN);

        hBN_x.push_back(i*hBNLatticeVectorA[0] + offsetxhBN + displacementhBN2);
        hBN_y.push_back(i*hBNLatticeVectorA[1] + offsetyhBN);



        if((i+1)%nCellsInSidehBN == 0 && i>0)
        {
            offsetxhBN = offsetxhBN -nCellsInSidehBN*hBNLatticeVectorA[0]+hBNLatticeVectorB[0];
            offsetyhBN = offsetyhBN -nCellsInSidehBN*hBNLatticeVectorA[1]+hBNLatticeVectorB[1];
        }

    }

    double sum_hBNx = 0, sum_hBNy = 0;
    for (int i = 0; i<2*m_nCellshBN;i++)
    {
        sum_hBNx = sum_hBNx + hBN_x[i];
        sum_hBNy = sum_hBNy + hBN_y[i];
    }

    m_hBNxcenter = sum_hBNx/(2*m_nCellshBN);
    m_hBNycenter = sum_hBNy/(2*m_nCellshBN);

    for (int i = 0; i<2*m_nCellshBN;i++)
    {
        hBN_x[i] = hBN_x[i] - m_hBNxcenter + m_xcenter;
        hBN_y[i] = hBN_y[i] - m_hBNycenter + m_ycenter;
    }







    ui->btn_Continue->setEnabled(false);
    ui->btn_Stop->setEnabled(false);

    setLinesBetweenAtomshBN(hBN_x, hBN_y);
    setLinesBetweenAtoms( g_x, g_y);
    ui->graphicsWidget_3->replot();
    ui->graphicsWidget_3->update();
    //plotAnimation(g_x, g_y,hBN_x, hBN_y, Qt::black);


}

WorkShop14::~WorkShop14()
{
    delete ui;
}

int WorkShop14::CalculateNumberOfCells(int nAtomsLocal)
{
    int nCellsInPrinciple = floor(nAtomsLocal/2);
    int nCellsInSide = floor(sqrt(nCellsInPrinciple));
    return (nCellsInSide*nCellsInSide);
}

int WorkShop14::CalculateNumberOfCellsInSide(int nAtomsLocal)
{
    int nCellsInPrinciple = floor(nAtomsLocal/2);
    return (floor(sqrt(nCellsInPrinciple)));
}


void WorkShop14::setCoordsOnEllipses(QVector<double>& pos_x, QVector<double>& pos_y)
{
    if(q_Ellipse.size() == 0){
        for(int i=0;i<m_nCells*2;i++)
        {
            q_Ellipse.push_back(new QCPItemEllipse(ui->graphicsWidget_3));
            q_Ellipse[i]->bottomRight->setCoords(pos_x[i]+Rad,pos_y[i]-Rad);
            q_Ellipse[i]->topLeft->setCoords(pos_x[i]-Rad,pos_y[i]+Rad);
            q_Ellipse[i]->setBrush(QBrush(Qt::SolidPattern));

            //q_Ellipse[i]->setBrush(*myBrush1);
        }
    }else
    {
        for(int i =0; i<m_nCells*2;i++)
        {

            q_Ellipse[i]->bottomRight->setCoords(pos_x[i]+Rad,pos_y[i]-Rad);
            q_Ellipse[i]->topLeft->setCoords(pos_x[i]-Rad,pos_y[i]+Rad);
            q_Ellipse[i]->setBrush(QBrush(Qt::SolidPattern));
        }
    }
}

void WorkShop14::setCoordsOnEllipseshBN(QVector<double>& pos_x, QVector<double>& pos_y)
{
    if(q_EllipsehBN.size() == 0){
        for(int i=0;i<m_nCellshBN*2;i++)
        {
            q_EllipsehBN.push_back(new QCPItemEllipse(ui->graphicsWidget_3));
            q_EllipsehBN[i]->bottomRight->setCoords(pos_x[i]+Rad,pos_y[i]-Rad);
            q_EllipsehBN[i]->topLeft->setCoords(pos_x[i]-Rad,pos_y[i]+Rad);
            q_EllipsehBN[i]->setBrush(*myBrush2);
        }
    }else
    {

        for(int i =0; i<m_nCellshBN*2;i++)
        {

            q_EllipsehBN[i]->bottomRight->setCoords(pos_x[i]+Rad,pos_y[i]-Rad);
            q_EllipsehBN[i]->topLeft->setCoords(pos_x[i]-Rad,pos_y[i]+Rad);
        }
    }



}
void WorkShop14::setLinesBetweenAtoms(QVector<double>& pos_x, QVector<double>& pos_y)
{




 int k = 0;
    if(q_Line.isEmpty())
    {

        for(int i=0;i<m_nCells*2;i++)
        {
            for(int j=i+1;j<m_nCells*2;j++)
            {
                double distanceBetweenAtoms = pow(  pow(pos_x[j]-pos_x[i],2) + pow(pos_y[j]-pos_y[i],2),   0.5 );
                if(  distanceBetweenAtoms< m_latticeconst*0.9 )
                {

                    q_Line.push_back( new QCPItemLine(ui->graphicsWidget_3));
                    //QCPItemLine* testLine;
                    //testLine->setPen(pen);
                    q_Line[k]->setPen(QPen(Qt::red, 5));;
                    q_Line[k]->start->setCoords(pos_x[i],pos_y[i]);
                    q_Line[k]->end->setCoords(pos_x[j],pos_y[j]);
                    q_Line[k]->setLayer("main");

                    AtomMap1[k]=i;
                    AtomMap2[k]=j;
                    k++;
                }
            }
        }
    }
    else{

        for(int l=0;l<q_Line.size();l++)
        {
            AtomMap1[l];
            AtomMap2[l];

            q_Line[l]->setPen(QPen(Qt::red,5));
            q_Line[l]->start->setCoords(pos_x[ AtomMap1[l]],pos_y[AtomMap1[l]]);
            q_Line[l]->end->setCoords(pos_x[AtomMap2[l]],pos_y[AtomMap2[l]]);
            q_Line[l]->setLayer("main");
        }


        /*for(int i=0;i<m_nCells*2;i++)
        {
            for(int j=i+1;j<m_nCells*2;j++)
            {
                double distanceBetweenAtoms = pow(  pow(pos_x[j]-pos_x[i],2) + pow(pos_y[j]-pos_y[i],2),   0.5 );
                if(  distanceBetweenAtoms< m_latticeconst*0.9 )
                {
                    if (k<q_Line.size())
                    {
                    q_Line[k]->setPen(QPen(Qt::black));
                    q_Line[k]->start->setCoords(pos_x[i],pos_y[i]);
                    q_Line[k]->end->setCoords(pos_x[j],pos_y[j]);
                    k++;
                    }

                }
            }
        }*/
    }
}

void WorkShop14::setLinesBetweenAtomshBN(QVector<double>& pos_x, QVector<double>& pos_y)
{
    int k = 0;
    if(q_LinehBN.isEmpty())
    {
        for(int i=0;i<m_nCellshBN*2;i++)
        {
            for(int j=i+1;j<m_nCellshBN*2;j++)
            {
                double distanceBetweenAtoms = pow(  pow(pos_x[j]-pos_x[i],2) + pow(pos_y[j]-pos_y[i],2),   0.5 );
                if(  distanceBetweenAtoms< m_latticeconsthBN*0.9 )
                {
                    q_LinehBN.push_back( new QCPItemLine(ui->graphicsWidget_3));

                    q_LinehBN[k]->setPen(QPen(Qt::black,5));
                    q_LinehBN[k]->start->setCoords(pos_x[i],pos_y[i]);
                    q_LinehBN[k]->end->setCoords(pos_x[j],pos_y[j]);
                    q_LinehBN[k]->setLayer("background");

                    AtomMaphBN1[k]=i;
                    AtomMaphBN2[k]=j;
                    k++;
                }
            }
        }
    }
    /*else
    {

        for(int l=0;l<q_LinehBN.size();l++)
        {
            AtomMaphBN1[l];
            AtomMaphBN2[l];

            q_LinehBN[l]->setPen(QPen(Qt::black,5));
            q_LinehBN[l]->start->setCoords(pos_x[AtomMaphBN1[l]],pos_y[AtomMaphBN1[l]]);
            q_LinehBN[l]->end->setCoords(pos_x[AtomMaphBN2[l]],pos_y[AtomMaphBN2[l]]);
            q_LinehBN[l]->setLayer("background");
        }

    }*/







}






void WorkShop14::animateConcurrent(){
    QtConcurrent::run(&WorkShop14::AnimateFunc, this);
}

void WorkShop14::contConcurrent(){
    QtConcurrent::run(&WorkShop14::ContinueFunc, this);
}


void WorkShop14::mouseIsMoving_3(QMouseEvent *event)
{

    if (m_clicked_3 == true && m_drawActivated == true && !m_threadIsOn)
    {
        double xFinal;
        double yFinal;

        double xInitial;
        double yInitial;

        if (m_firstMove)
        {
            m_xOldMove = m_xClickedMoire;
            m_yOldMove = m_yClickedMoire;
        }

        QPoint point2 = event->pos();
        m_xActualMove = ui->graphicsWidget_3->xAxis->pixelToCoord(point2.x());
        m_yActualMove = ui->graphicsWidget_3->yAxis->pixelToCoord(point2.y());

        xFinal = m_xActualMove - m_xcenter;
        yFinal = m_yActualMove - m_ycenter;

        xInitial = m_xOldMove - m_xcenter;
        yInitial = m_yOldMove - m_ycenter;

        m_xOldMove = m_xActualMove;
        m_yOldMove = m_yActualMove;


        double RadInitial =  pow(  pow(xInitial,2) + pow(yInitial,2),   0.5 );
        double RadFinal =  pow(  pow(xFinal,2) + pow(yFinal,2),   0.5 );


        double alphaInitial =atan(yInitial/xInitial);
        double alphaFinal = atan(yFinal/xFinal);
        m_deltaAlpha = alphaFinal - alphaInitial;
        m_deltaRad = RadFinal - RadInitial;
        m_expansionCoef = RadFinal/RadInitial;

        RotateAndExpand(m_deltaAlpha, m_expansionCoef, g_x, g_y, g_xNew, g_yNew);
        //plotAnimation( g_xNew, g_yNew,hBN_x, hBN_y, Qt::black);
        setLinesBetweenAtoms( g_xNew, g_yNew);
        ui->graphicsWidget_3->replot();
        ui->graphicsWidget_3->update();
    }


}






void WorkShop14::plotAnimation(QVector<double> g_x, QVector<double> g_y, QColor color)
{
    setLinesBetweenAtoms(g_x, g_y);
    ui->graphicsWidget_3->replot();
    ui->graphicsWidget_3->update();
    m_processedFrame = true;
    qDebug()<<"FinishedPlotting";
}






void WorkShop14::StopFunc()
{
    qDebug()<<"Stop function works!";
    m_stopThread =true;
    //m_threadIsOn = false;

}

void WorkShop14::ClearAll()
{
    ui->btn_Animate->setEnabled(true);

    ui->btn_Continue->setEnabled(false);
    qDebug()<<"Clear function works!";
    if(m_threadIsOn){
    m_stopThread =true;
    m_resetValues = true;
    }else{resetFunction(); }

    //t1->wait();
    //t1->exit(); //This one causes crash*/
}

void WorkShop14::resetFunction()
{

    m_setVelocities = false;

    emit updateGUISignal(4);

    g_x = g_xSaved;
    g_y = g_ySaved;

    emit updatePlot(g_x, g_y, rojo);
    m_resetValues = false;
    m_threadIsOn = false;
}





void WorkShop14::clickedGraph_3(QMouseEvent *event)
{

    m_clicked_3 = true;

    if (m_threadIsOn){
    m_stopThread = true;
    }

    QPoint point1 = event->pos();
    m_xClickedMoire = ui->graphicsWidget_3->xAxis->pixelToCoord(point1.x());
    m_yClickedMoire = ui->graphicsWidget_3->yAxis->pixelToCoord(point1.y());


}





void WorkShop14::releasedGraph_3(QMouseEvent *event)
{
    m_clicked_3 = false;

    if (!m_threadIsOn)
    {

        for(int i =0; i<g_x.size();i++)
        {
        g_x[i]=g_xNew[i];
        g_y[i]=g_yNew[i];
        }

    }

}

void WorkShop14::RotateAndExpand( double m_deltaAlpha, double m_expansionCoef, QVector<double>& g_x, QVector<double>& g_y, QVector<double>& g_xNew, QVector<double>& g_yNew)
{
    double g_xAux, g_yAux;
    int size = g_x.size();
    int size1 = g_xNew.size();
    m_latticeconst = m_latticeconstInitial*m_expansionCoef;



    for(int i = 0; i< size; i++)
    {
    g_xAux = m_expansionCoef*((g_x[i]-m_xcenter )*cos(m_deltaAlpha)- (g_y[i]-m_ycenter )*sin(m_deltaAlpha));
    g_yAux = m_expansionCoef*((g_x[i]-m_xcenter)*sin(m_deltaAlpha)+(g_y[i]-m_ycenter )*cos(m_deltaAlpha));

    g_xNew[i] = g_xAux + m_xcenter;
    g_yNew[i] = g_yAux + m_ycenter;
    }

}





void WorkShop14::setMaxTime()
{
    m_tmax = ui->spinBox_tMax->text().toInt();
}


void WorkShop14::updateGUI(int updateType){

    switch(updateType){
    case 0:
        //ui->btn_Line->setEnabled(false);
        //ui->btn_CoarsePen->setEnabled(false);
        ui->btn_Continue->setEnabled(false);
        ui->btn_Animate->setEnabled(false);
        //ui->btn_Clear->setEnabled(false);
        ui->btn_Stop->setEnabled(true);
        ui->spinBox_tMax->setEnabled(false);
        break;
    case 1:
        //ui->btn_Line->setEnabled(true);
        //ui->btn_CoarsePen->setEnabled(true);
        ui->btn_Continue->setEnabled(true);
        ui->btn_Animate->setEnabled(true);
        //ui->btn_Clear->setEnabled(true);
        ui->btn_Stop->setEnabled(false);
        ui->spinBox_tMax->setEnabled(true);
        break;
    case 2:
        //ui->btn_Line->setEnabled(false);
        //ui->btn_CoarsePen->setEnabled(false);
        ui->btn_Continue->setEnabled(false);
        ui->btn_Animate->setEnabled(false);
        //ui->btn_Clear->setEnabled(false);
        ui->btn_Stop->setEnabled(true);
        ui->spinBox_tMax->setEnabled(false);
        break;
    case 3:
        //ui->btn_Line->setEnabled(true);
        //ui->btn_CoarsePen->setEnabled(true);
        ui->btn_Continue->setEnabled(true);
        ui->btn_Animate->setEnabled(true);
        //ui->btn_Clear->setEnabled(true);
        ui->btn_Stop->setEnabled(false);
        ui->spinBox_tMax->setEnabled(true);
        break;
    case 4:
        //ui->btn_SetVelocities->setText("Set Velocities");
        break;
    case 5:
        ui->btn_Continue->setEnabled(false);

    }
}

void WorkShop14::AnimateCore(int AnimOrCont)
{


    if(AnimOrCont == 1)
    {
        g_x = g_xNew;
        g_y = g_yNew;
    }


    for(m_t=0;m_t<m_tmax;m_t++)
    {

        RotateAndExpand(0.02*m_t, 1/*1 + 0.2*sin(0.333*m_t)*/, g_x, g_y, g_xNew, g_yNew);
        //plotAnimation( g_xNew, g_yNew,hBN_x, hBN_y, Qt::black);

        //ui->graphicsWidget_3->replot();
        //ui->graphicsWidget_3->update();



        //RotateAndExpand(0.1,1,g_x, g_y);

        qDebug()<<"emit"<<QString::number(m_t);
        m_processedFrame = false;

        emit updatePlot(g_xNew, g_yNew, negro);

        while(!m_processedFrame && !m_stopThread){};



        if(m_stopThread && AnimOrCont == 0){
            ExitAnimation();
            m_stopThread = false;
            m_threadIsOn = false;
            if(m_resetValues){
            emit updateGUISignal(5);
            resetFunction();

            }
            qDebug()<<"Exiting AnimateFunc";
            return;

        }else if(m_stopThread && AnimOrCont == 1){
            resetContinue();
            m_stopThread = false;
            m_threadIsOn = false;
            if(m_resetValues){
            emit updateGUISignal(5);
            resetFunction();
            }
            qDebug()<<"Exiting ContFunc";
            return;

        }


    }

    if(!m_stopThread && AnimOrCont == 0)
    {
        //We have to subtract 1 to m_t when exiting this function if the
        //thread is not stopped given that in such case the loop sets it
        //to m_tmax althouth m_t doesn't enter the loop with such value.
        m_t = m_t -1;
        ExitAnimation();
        m_threadIsOn = false;
    }else if(!m_stopThread && AnimOrCont == 1){
        m_t = m_t -1;
        resetContinue();
        m_threadIsOn = false;
    }
}












void WorkShop14::AnimateFunc()
{
    m_threadIsOn = true;
    emit updateGUISignal(0);

    m_stopThread = false;
    m_tabsolute = 0;
    AnimateCore(0);

}

void WorkShop14::ContinueFunc()
{
    m_threadIsOn = true;
    emit updateGUISignal(2);

    m_setVelocities = false;
    m_stopThread = false;
    AnimateCore(1);
}

void WorkShop14::ExitAnimation()
{
    m_tabsolute = m_t;
    emit updateGUISignal(1);
    g_x = g_xNew;
    g_y = g_yNew;

    emit updatePlot(g_xNew, g_yNew,rojo);
}

void WorkShop14::resetContinue()
{
    m_tabsolute = m_tabsolute + m_t;
    g_x = g_xNew;
    g_y = g_yNew;
    emit updateGUISignal(3);
    emit updatePlot(g_xNew, g_yNew,negro);

}




void WorkShop14::closeEvent(QCloseEvent *bar)
{
    exit(0);
    bar->accept();
}


void WorkShop14::endProg(){
    //m_stopThread = true;
    exit(0);
}
