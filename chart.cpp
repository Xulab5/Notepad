/****
 *
 * AUTHOR: Steven J Sub
 * LECTURER: Mr. Farshad Rabbani
 * Date: March 11 2019
 * DESCRIPTION: This notepad allows user to write and read text. Allows user to see text properties
 * and as well creates chart base on the frquency of the words and also have other function
 *
 * */
#include "chart.h"
#include "word.h"
#include <QList>
#include "ui_chart.h"
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QLegend>
#include <QBarCategoryAxis>
#include <QPieSeries>
#include <QPieSlice>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QLineSeries>
#include <QCategoryAxis>

QT_CHARTS_USE_NAMESPACE;

chart::chart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chart)
{
    ui->setupUi(this);
}

chart::~chart()
{
    delete ui;
}

void chart::piechart()
{
    QPieSeries *series= new QPieSeries();

        int i=0;
        int size=wordlist.count()-1;//set the size

            while(i < amountToShow && size >=0)//insert the correct number of data in the chart
            {
                    series->append(wordlist[size].getText(),wordlist[size].getFrequency());
                    --size;
                i++;
            }
             QPieSlice * slice=series->slices().at(0);
             slice->setExploded();
             slice->setLabelVisible();
             slice->setPen(QPen(Qt::darkGreen,2));
             slice->setBrush(Qt::green);

             QChart *chart= new QChart();
             chart->addSeries(series);
             chart->setTitle("Words Frequency Pie Chart");
             chart->legend()->hide();

             QChartView *chartView = new QChartView(chart);//charts must be place in the chartview
             chartView->setRenderHint(QPainter::Antialiasing);

            setCentralWidget(chartView);
            setWindowTitle("PieChart of Word Frequency");
            resize(500,500);

}//end of function

void chart::lineChart()
{
    QLineSeries * series = new QLineSeries();
    QCategoryAxis *axisX= new QCategoryAxis();

    int i=0;
    int pos=1;
    int size=wordlist.count()-1;
    while(i < amountToShow && size>=0)
    {

            series->append(pos,wordlist[size].getFrequency());
            axisX->append( wordlist[size].getText(),pos);
            --size;
            ++pos;
        i++;
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle(QString::number(amountToShow)+" word frequency");
    QPen pen(QRgb(0x000000));
    pen.setWidth(5);
    series->setPen(pen);

    chart->setAxisX(axisX,series);
    QChartView * chartView= new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(chartView);
    setWindowTitle("LineChart of Words Frequency");
    resize(400,400);

}//end of function lineChart()

void chart::barChart()
{

QList<QBarSet*> barlist;

int size=wordlist.count()-1;
QBarSet *set;
int i=0;
    while(i< amountToShow && size>=0)//adds the correcy number of data in the list
    {
        set= new QBarSet(wordlist[size].getText());
        set->append(wordlist[size].getFrequency());
        barlist<<set;
        --size;
        i++;
    }//end of while

    i=0;
    QBarSeries * barSeries = new QBarSeries();
    while(i < barlist.count()){
        barSeries->append( barlist[i]);
        i++;
    }

    QChart *chart = new QChart();
    chart->addSeries(barSeries);
    chart->setTitle("Barchart of "+QString::number(amountToShow)+" words Frequency");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);//charts must be place in the chartview
    chartView->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(chartView);
    setWindowTitle("Barchart of words frequency");
    resize(500,500);

}//end of function barChart()

void chart::chartinformation(int show,QList<Word> list, int type)
{
    wordlist=list;//initialize the Qlist

    if(show==0)//set the right number of words to display in chart
        amountToShow=10;
    else if(show==1)
        amountToShow=20;
    else
        amountToShow=list.count();

    switch (type) //determine which chart to call
    {
    case 0:
            lineChart();
        break;
    case 1:
        barChart();
        break;
    case 2:
        piechart();
        break;
    }//end of switch

}//end of function chartinformation
