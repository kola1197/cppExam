#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iconDefault = QIcon("rofl1.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::generateField()
{
    for (int i=0;i<16;i++)
    {
        for (int j=0;j<16;j++)
        {
            cages[i][j] = Cage();
        }
    }
    int minesToAdd=40;
    while (minesToAdd>0)
    {
        int x = std::rand() % 16;
        int y = std::rand() % 16;
        if (!cages[x][y].mine)
        {
            cages[x][y].mine=true;
            minesToAdd--;
        }
    }
    for (int i=0;i<16;i++)
    {
        for (int j=0;j<16;j++)
        {
            if (cages[i][j].mine)
            {
                int delta [8][2] = {{1,1},{1,-1},{1,0},{-1,1},{-1,0},{-1,-1},{0,-1},{0,1}};
                for (int k=0;k<8;k++)
                {
                    if (i+delta[k][0]>-1 && i+delta[k][0]<16 && j+delta[k][1]>-1 && j+delta[k][1]<16 )
                    cages[i+delta[k][0]][j+delta[k][1]].countOfMines++;
                }
            }
        }
    }
    for (int i=0;i<16;i++)
    {
        for (int j=0;j<16;j++)
        {
            if (cages[i][j].mine)
            {
                //button[i][j]->setText("#");
                cages[i][j].text="#";
            }
            else
            {
                if (cages[i][j].countOfMines!=0)
                {
                    //button[i][j]->setText(QString::number(cages[i][j].countOfMines));
                    cages[i][j].text = QString::number(cages[i][j].countOfMines).toStdString();
                }
            }
        }
    }
}
void MainWindow::startGame()
{
    this->setWindowTitle("Сапер");
    drawField();
    generateField();
    gameIsActive=true;
}


void MainWindow::onClick()
{
    if (gameIsActive)
    {
        QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
        QString s = buttonSender->toolTip();
        //std::cout<<"-"<<s.toStdString()<<"-"<<std::endl;

        QString x = "";
        QString y = "";
        bool wasDelimiter=false;
        for (int i=0;i<s.length();i++)
        {
            if (s[i]=="_")
            {
                wasDelimiter=true;
            }
            else {
                if (wasDelimiter)
                {
                    y+=s[i];
                }
                else {
                    x+=s[i];
                }
            }
        }
        std::cout<<x.toStdString()<<" -x, and y = "<<y.toStdString()<<std::endl;
        openCage(x.toInt(),y.toInt());
    }
}

void MainWindow::openCage(int x,int y)
{
    if (!cages[x][y].opened)
    {
        cages[x][y].opened=true;
        countOfOpened++;
        if (cages[x][y].text=="0")
        {
            int delta [8][2] = {{1,1},{1,-1},{1,0},{-1,1},{-1,0},{-1,-1},{0,-1},{0,1}};
            for (int k=0;k<8;k++)
            {
                if (x+delta[k][0]>-1 && x+delta[k][0]<16 && y+delta[k][1]>-1 && y+delta[k][1]<16 )
                {
                openCage(x+delta[k][0],y+delta[k][1]);
                }
            }
        }
        if (cages[x][y].mine==true)
        {
            gameIsActive = false;
            this->setWindowTitle("GameOver");
        }
    }
    updateGame();
}

void MainWindow::setMineFlag(int x,int y)
{
    cages[x][y].mineFlag = true;
    updateGame();
}

void MainWindow::updateGame()
{
    if (countOfOpened==16*16-40 && gameIsActive)
    {
        gameIsActive==false;
        this->setWindowTitle("You win!!!");
    }
    redraw();
}

void MainWindow::redraw()
{
    for (int i=0;i<16;i++)
    {
        for (int j=0;j<16;j++)
        {
            if (cages[i][j].opened)
            {
                button[i][j]->setText(cages[i][j].text.c_str());
            }
            if (cages[i][j].mineFlag)
            {
                button[i][j]->setText("?");
            }
        }
    }
}

void MainWindow::drawField()
{
    ui->gridLayout_2->setSpacing(0);
    for (int i=0;i<16;i++)
    {
        for (int j=0;j<16;j++)
        {
            QString name = QString::number(i)+"_"+QString::number(j);
            //button[i][j]->setObjectName( "PPPPPP");
            button[i][j] = new QPushButton("");
            button[i][j]->setFixedSize(40,40);
            button[i][j]->move(40*i,40*j);

            button[i][j]->setToolTip(name);
            button[i][j]->setToolTipDuration(1);

            connect(button[i][j],SIGNAL( released()),this,SLOT (onClick()));
            button[i][j]->show();
            ui->gridLayout_2->addWidget(button[i][j],i,j);
        }
    }

}


