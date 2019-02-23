#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QDebug>
#include <QSlider>
#include <QObject>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <map>
#include <scribblearea.h>
#include <neuralnetwork.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void RefreshWidgetList(QObject* parent);

private:
    void createActions();
    void generateTable();
    void makeConnections();

    Ui::MainWindow* ui;
    NeuralNetwork* net;

    std::map<QString, QObject*> widgetList;

    ScribbleArea* area;
    QImage temporary;
    QSlider* widthSlider;

    QLineEdit* answerLine;

    QPushButton* clearBtn;
    QPushButton* startBtn;
    QPushButton* createBtn;

    QAction* action_SaveImage;
    QAction* action_CreateWeights;

    QTableWidget* table;

private slots:
    void action_CreateWeights_OnClick();
    void changeWidthSlider();
    void startButtonClicked();
    void createButtonClicked();
};

#endif // MAINWINDOW_H
