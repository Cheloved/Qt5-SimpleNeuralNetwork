#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::RefreshWidgetList(QObject* parent)
{
    this->widgetList.insert( std::make_pair( parent->objectName(), parent ) );
    QList<QObject*> list = parent->findChildren<QObject*>();
    if ( list.size() > 0 )
        foreach ( QObject* item, list )
            this->RefreshWidgetList(item);
}

void MainWindow::changeWidthSlider()
{
    area->setPenWidth(this->widthSlider->value());
    static_cast<QLabel*>( widgetList["widthLabel"] )->setText( QString::fromStdString( std::to_string(widthSlider->value()) ) );
}

void MainWindow::createButtonClicked()
{
    std::vector<int> layers = { 400, 16, 16, 10 };
    temporary = area->image.scaled(20,20, Qt::KeepAspectRatio);
    net = new NeuralNetwork( layers, &temporary );
    qDebug() << "[OK] Create network";
}

void MainWindow::startButtonClicked()
{
    net->LoadWeights(net->WeightPath);
    qDebug() << "[OK] Load weights";
    net->Learn(answerLine->text().toInt());
    qDebug() << "[OK] Network run";
    qDebug() << "Network result: " << net->GetNetworkResult();

    for ( unsigned long i = 0; i < net->neurons.back().size(); i++ )
    {
        int row = static_cast<int>(i);
        this->table->setItem(row, 1, new QTableWidgetItem( QString::fromStdString( std::to_string(net->neurons.back()[i].value) ) ));
    }
    update();
    qDebug() << "[OK] Result showed";
    //net->ShowStructure();
    //net->ShowWeights();
    delete net;
}

void MainWindow::action_CreateWeights_OnClick()
{
    net->CreateWeights();
    net->SaveWeights(net->WeightPath);
    qDebug() << "[OK] Create and save weights";
}

void MainWindow::createActions()
{
    clearBtn = static_cast<QPushButton*>( widgetList["clearButton"] );
}

void MainWindow::generateTable()
{
    table = static_cast<QTableWidget*>( widgetList["tableWidget"] );
    table->setRowCount(10);
    table->setColumnCount(2);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem( tr("Digit"), 0 ));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Result"), 0 ));
    for ( int i = 0; i < 10; i++ )
    {
        table->setVerticalHeaderItem(i, new QTableWidgetItem());
        table->setItem( i, 0, new QTableWidgetItem( QString::fromStdString(std::to_string(i)), 0 ) );
    }
    table->showGrid();
}

void MainWindow::makeConnections()
{
    connect(clearBtn, &QPushButton::clicked, area, &ScribbleArea::clearArea);
    connect(widthSlider, &QSlider::valueChanged, this, &MainWindow::changeWidthSlider);
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(action_CreateWeights, &QAction::triggered, this, &MainWindow::action_CreateWeights_OnClick);
    connect(createBtn, &QPushButton::clicked, this, &MainWindow::createButtonClicked);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    RefreshWidgetList(this);
    setWindowTitle(tr("Neural Network"));
    area = static_cast<ScribbleArea*>( widgetList["areaWidget"] );
    widthSlider = static_cast<QSlider*>( widgetList["widthSlider"] );
    startBtn = static_cast<QPushButton*>( widgetList["startButton"] );
    action_CreateWeights = static_cast<QAction*>( widgetList["actionCreate_weights"] );
    answerLine = static_cast<QLineEdit*>( widgetList["answerLine"] );
    createBtn = static_cast<QPushButton*>( widgetList["createButton"] );

    createActions();
    generateTable();
    makeConnections();

    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}
