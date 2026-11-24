
#ifndef WORKSHop14_H
#define WORKSHop14_H

#include <QMainWindow>
#include "qpen.h"
class QCPItemEllipse;
class QCPItemLine;
class QColor;
class QPen;

QT_BEGIN_NAMESPACE
namespace Ui { class WorkShop14; }
QT_END_NAMESPACE

class WorkShop14 : public QMainWindow

{
    Q_OBJECT

public:
    WorkShop14(QWidget *parent = nullptr);
    ~WorkShop14();

    void addPoint(double x, double y);

    void closeEvent(QCloseEvent *bar) override;
    void calculateCoefs();
    void resetFunction();
    void resetContinue();
    void animateConcurrent();
    void contConcurrent();
    void AnimateCore(int AnimOrCont);

private slots:

    void mouseIsMoving_3(QMouseEvent *event);
    void ClearAll();
    void clickedGraph_3(QMouseEvent *event);
    void releasedGraph_3(QMouseEvent *event);
    void AnimateFunc();
    void ExitAnimation();
    void endProg();
    void StopFunc();
    void setMaxTime();

    void ContinueFunc();
    //void setLine();
    void plotAnimation(QVector<double>, QVector<double>,  QColor color);
    void updateGUI(int updateType);

    int CalculateNumberOfCells(int nAtomsLocal);
    int CalculateNumberOfCellsInSide(int nAtomsLocal);

    void setCoordsOnEllipses(QVector<double>&, QVector<double>&);
    void setLinesBetweenAtoms(QVector<double>&, QVector<double>&);

    void setCoordsOnEllipseshBN(QVector<double>&, QVector<double>&);
    void setLinesBetweenAtomshBN(QVector<double>&, QVector<double>&);

    void RotateAndExpand( double m_deltaAlpha, double m_expansionCoef, QVector<double>& g_x, QVector<double>& g_y,QVector<double>& g_xNew, QVector<double>& g_yNew);


    signals:
    void updatePlot( QVector<double>, QVector<double>, QColor color);
    void updateGUISignal(int updateType);

private:
    Ui::WorkShop14 *ui;
    double qv_x_Click = 0.0, qv_y_Click = 0.0, qv_x_Release = 0.0, qv_y_Release = 0.0, m_xClicked =0.0, m_yClicked = 0.0, m_xOldMove =0.0, m_yOldMove =0.0, m_xActualMove =0.0, m_yActualMove =0.0, m_xClickedMoire = 0.0, m_yClickedMoire = 0.0, m_xReleased =0.0, m_yReleased = 0.0, m_xReleasedMoire =0.0, m_yReleasedMoire = 0.0, m_xDeformation =0.0, m_yDeformation = 0.0;
    double m_deltaAlpha = 0.0, m_deltaRad = 0.0, m_expansionCoef = 0.0;

    QVector<double>   g_x, g_y, g_xSaved, g_ySaved,g_xNew,g_yNew, gLatticeVectorA, gLatticeVectorB, hBN_x, hBN_y, hBNLatticeVectorA, hBNLatticeVectorB;
    QVector<QCPItemEllipse*> q_Ellipse;
    QVector<QCPItemEllipse*> q_EllipsehBN;
    QVector<QCPItemLine*> q_Line;
    QVector<QCPItemLine*> q_LinehBN;
    std::map<int, int> AtomMap1;
    std::map<int, int> AtomMap2;
    std::map<int, int> AtomMaphBN1;
    std::map<int, int> AtomMaphBN2;
    bool m_stopThread = false;
    bool m_setVelocities = false;
    bool m_setLine = true;
    bool m_clicked = false;
    bool m_clicked_2 = false;
    bool m_clicked_3 = false;
    bool m_resetValues = false;
    bool m_threadIsOn = false;
    bool m_coarsePen = true;

    int m_imax =300;
    int m_nmax =200;
    double pi = 3.14159265358979323846;
    double v = 0.03;
    int m_tmax = 1000;

    int m_t = 0;
    int m_tabsolute = 0;
    int dotCounter = 0;
    int dotVCounter = 0;


    double m_L = 4.8;
    double m_xOffset = 0.1;
    double m_yOffset = 2.6;

    QColor negro =Qt::black;
    QColor rojo =Qt::red;
    QColor azul = Qt::blue;

    int nAtoms = 2000;
    int m_nCells;
    double m_latticeconstInitial = 0.1;
    double m_latticeconst = 0.1;
    double m_latticeangle = pi/6;

    int nAtomshBN = 2000;
    int m_nCellshBN;
    double m_latticeconsthBN = 0.11;
    double m_latticeanglehBN = pi/6;

    double m_xcenter = 2.6, m_ycenter = 2.6, m_gxcenter = 0, m_gycenter = 0, m_hBNxcenter = 0, m_hBNycenter = 0;

    double Rad = 0.025;
    const QBrush* myBrush1;
    const QBrush* myBrush2;

    bool m_firstMove = true;
    bool m_drawActivated =true;

    const QBrush* m_brush;
    const QPen* m_pen;
    bool m_processedFrame = false;

    //QThread* t1 = nullptr;
};

#endif // WORKSHOP7_H
