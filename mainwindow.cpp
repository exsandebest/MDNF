#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QMap>
#include <QDebug>
#include <QSpacerItem>
#include <QMessageBox>
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <QThreadPool>
#include <findmdnfclass.h>
#include <QMovie>
Ui::MainWindow * extUi;
findMDNFClass * FMDNFC;
int varCount = 3;
int needOnes = 0;
QMap <int, QStringList> headMap;
QMap <int, QStringList> banned;
QMap <int, QMap <QString, QList<int> > > accepted; // col, set, rows
QMap <int, int> selectedOnes; //row, bool
QMap <int, QMap <QString, bool> > selectedSets;  // col, str, bool
int minLen = 1000000000;
QString varLbl[6] = {"a","b","c","d","e","f"};
QMap <QString, int> assoc;
QMap <int, QList <QStringList> >answers;
QString currentFunction = "00000000";


bool isBanned(int x, QString s){
    if (banned[x].indexOf(s) != -1) {
        return true;
    }
    return  false;
}

bool isOverlapped (int col, QString set){
    for (int i = 0; i < accepted[col][set].length(); ++i){
        if (!selectedOnes[accepted[col][set][i]]){
            return false;
        }
    }
    return true;
}


QString reverse(QString s){
    QString ans = "";
    for (int i = s.length()-1; i>=0; --i){
        ans+=s[i];
    }
    return ans;
}

bool checkDubl(QStringList list){
    int flag;
    for (int i = 0; i < answers[minLen].length(); ++i){
        flag = 1;
        for (int j = 0; j < list.length(); ++j){
            if (answers[minLen][i][j] != list[j]){
                flag = 0;
            }
        }
        if (flag) return false;
    }
    return true;
}

int pow(int a, int n){
    if (!n) return 1;
    int ans = a;
    for (int i = 0; i <n-1; ++i){
        ans *= a;
    }
    return ans;
}


int varCountp2 = pow(2,varCount);


QString getTerm(QString head, QString val){
    QString ans;
    for (int i = 0; i < val.length(); ++i){
        if(val[i] == "1"){
            ans+=head[i];
        } else {
            ans+="!"+head[i];
        }
    }
    return ans;
}



void updateVarCount(int x) {
    varCount = x;
    varCountp2 = pow(2,x);
    for(int i = 0; i < x; ++i){
        assoc[varLbl[i]] = x-i-1;
    }
}







QStringList getSimpleColumn(QString s){
    int x = assoc[s];
    QStringList ans;
   while(ans.length() < varCountp2){
       for (int i = 0; i < pow(2,x); ++i){
           ans<<"0";
       }
       for(int i = 0; i < pow(2,x); ++i){
           ans<<"1";
       }
   }
   return ans;
}

QStringList getColumn(QString s){
    QStringList ans;
    for (int i = 0; i < varCountp2; ++i){
        ans << "";
    }
    for(int i = 0; i < s.length(); ++i){
        QStringList tmpColumn = getSimpleColumn(QString(s[i]));
        for (int j = 0; j < varCountp2; ++j){
            ans[j]+=tmpColumn[j];
        }
    }
    return ans;
}


QString get2from10(QString s){
    long long num = s.toLongLong();
    QString ans ="";
    while (num!=0){
        QString s1;
        s1.setNum(num%2);
        ans = s1+ans;
        num/=2;
    }
    return ans;
}

QString get10from2 (QString s){
    long long ansNum = 0;
    for (int i = s.length()-1; i>=0; --i){
        ansNum+=(QString(s[i]).toInt())*pow(2,i);
    }
    QString ansStr;
    ansStr.setNum(ansNum);
    return ansStr;
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    extUi = ui;
    QStringList tmpList;
    tmpList << "a";
    headMap[1] = tmpList;
    tmpList.clear();
    tmpList << "a" << "b" << "ab";
    headMap[2] = tmpList;
    tmpList.clear();
    tmpList << "a" << "b" << "c" << "ab" << "ac" << "bc" << "abc";
    headMap[3] = tmpList;
    tmpList.clear();
    tmpList << "a" << "b" << "c" << "d" << "ab" << "ac" << "ad" <<"bc" <<"bd" << "cd" << "abc"<<"abd"<<"acd"<<"bcd"<<"abcd";
    headMap[4] = tmpList;
    tmpList.clear();
    tmpList << "a" << "b" << "c" << "d" << "e" << "ab" << "ac" << "ad" << "ae" << "bc" << "bd" << "be" << "cd" << "ce" << "de" << "abc" << "abd" << "abe" << "acd" << "ace" << "ade" <<"bcd" << "bce" <<"bde" << "cde" << "abcd" <<"abce" << "abde" << "acde" << "bcde" << "abcde";
    headMap[5] = tmpList;
    tmpList.clear();
    tmpList << "a" << "b" << "c" << "d" << "e" << "f" /**/<< "ab" << "ac" << "ad" <<"ae" << "af" << "bc" <<"bd" << "be" << "bf" << "cd" << "ce" << "cf" << "de" << "df" << "ef"/**/ << "abc" << "abd" << "abe" << "abf" << "acd" << "ace" << "acf" << "ade" << "adf" << "aef" << "bcd" << "bce" << "bcf" << "bde" << "bdf" << "bef" << "cde" << "cdf" << "cef" << "def"/**/ << "abcd" << "abce" << "abcf" <<"abde" << "abdf" << "abef" << "acde" <<"acdf" << "acef" << "adef" << "bcde" << "bcdf" << "bcef" << "bdef" << "cdef" << "abcde" << "abcdf" << "abcef" << "abdef" << "acdef" << "bcdef" << "abcdef";
    headMap[6] = tmpList;
    qDebug() << tmpList.length();
    tmpList.clear();
    this->showMaximized();
    ui->r1->setChecked(true);
    updateVarCount(varCount);
    ui->spinBoxVarCount->setValue(varCount);
    QObject :: connect(ui->mainTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(changeCellValue(int, int)));
    buildTable();
   // thread = new QThread(this);
    FMDNFC = new findMDNFClass();
   /* FMDNFC->moveToThread(thread);
    QObject::connect(thread, &QThread::finished, FMDNFC, &QObject::deleteLater);
    QObject::connect(this, &MainWindow::mySignal , FMDNFC, &findMDNFClass::certainMDNF);
    thread->start();*/

   FMDNFC->certainMDNF();
}


void MainWindow::changeCellValue(int row, int col){
    if (col!=0)return;
    QTableWidgetItem * item = ui->mainTable->item(row,col);
    if (item->text() == "0"){
        item->setText("1");
    } else {
        item->setText("0");
    }
}


void MainWindow::buildTable(){
    QTableWidget * table = ui->mainTable;
    int tmpColumnCount = table->columnCount();

    //ui->funcNumberInput1->setText(currentFunction);
  // ui->funcNumberInput2->setText(get10from2(currentFunction));

    for(int i = 0; i < tmpColumnCount; ++i){
        table->removeColumn(0);
        table->removeRow(0);
    }
    for (int i = 0; i < varCountp2; ++i){
        table->insertRow(i);
        table->insertColumn(i);
    }

    QTableWidgetItem * item = new QTableWidgetItem();
    item->setText("f(x)");
    table->setHorizontalHeaderItem(0,item);



    for(int i = 0; i < varCountp2; ++i){
        QTableWidgetItem * itm = new QTableWidgetItem();
        itm->setText(QString(currentFunction[i]));
        itm->setFlags(Qt::ItemIsEnabled);
        itm->setTextAlignment(Qt::AlignCenter);
        table->setItem(i,0,itm);
    }

    for (int i = 1; i < varCountp2; ++i){
        QTableWidgetItem * itm = new QTableWidgetItem();
        itm->setText(headMap[varCount][i-1]);
        table->setHorizontalHeaderItem(i,itm);
    }

    for (int j = 1; j < varCountp2; ++j){
        QString hed = headMap[varCount][j-1];
        QStringList column = getColumn(hed);
        for (int i = 0; i < column.length(); ++i){
            QCoreApplication::processEvents();
            QTableWidgetItem * itm = new QTableWidgetItem();
            itm->setText(column[i]);
            itm->setFlags(Qt::ItemIsEnabled);
            itm->setTextAlignment(Qt::AlignCenter);
            table->setItem(i,j,itm);
        }
    }


    for (int i =0 ; i <varCountp2; ++i){
        QTableWidgetItem * itm = table->item(i,0);
        if (itm->text() == "0"){
            for (int j = 1; j < varCountp2; ++j){
                QCoreApplication::processEvents();
               QTableWidgetItem * tmpItm = table->item(i,j);
               tmpItm->setBackgroundColor(QColor(255, 0, 0, 140));
               if (!isBanned(j, tmpItm->text())){
                   banned[j] << tmpItm->text();
               }
            }
        }
    }

    for (int i =0 ; i <varCountp2; ++i){
        QTableWidgetItem * itm = table->item(i,0);
        if (itm->text() == "1"){
            ++needOnes;
            selectedOnes[i+1] = false;
            for (int j = 1; j < varCountp2; ++j){
                QCoreApplication::processEvents();
               QTableWidgetItem * tmpItm = table->item(i,j);
               if(isBanned(j,tmpItm->text())){
                   tmpItm->setBackgroundColor(QColor(0,0,225,127));
               } else {
                   tmpItm->setBackgroundColor(QColor(0,225,0,127));
                   accepted[j][tmpItm->text()] << i+1;
               }
            }
        }
    }


}




MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnApply_clicked()
{
    int tmpVarCount = ui->spinBoxVarCount->value();

    QString stringInput;
    if(ui->r1->isChecked()){
       stringInput = ui->funcNumberInput1->text();
       if (!QRegExp("[10]*").exactMatch(stringInput)){
           QMessageBox::about(this, "Warning!", "Некорректное значение");
           return;
       }
       for (int i = 0; pow(2,tmpVarCount)-stringInput.length()>0; ++i){
           stringInput = "0"+stringInput;
       }
       ui->funcNumberInput2->setText("");
     //  stringInput = reverse(stringInput);
    } else if (ui->r2->isChecked()){
       stringInput = get2from10(ui->funcNumberInput2->text());
       if (!QRegExp("[0-9]*").exactMatch(stringInput)){
           QMessageBox::about(this, "Warning!", "Некорректное значение");
           return;
       }
       for (int i = 0; pow(2,tmpVarCount)-stringInput.length()>0; ++i){
           stringInput = "0"+stringInput;
       }
       ui->funcNumberInput1->setText("");
    }
    if(stringInput == currentFunction){
        return;
    }
    if (stringInput.length() > pow(2,tmpVarCount)){
        QMessageBox::about(this, "Warning!", "Некорректное значение");
        return;
    }
    updateVarCount(tmpVarCount);
    qDebug() << stringInput;
    minLen = 1000000000;
    answers.clear();
    needOnes = 0;
    selectedOnes.clear();
    selectedSets.clear();
    accepted.clear();
    banned.clear();
    currentFunction = stringInput;
    buildTable();
     FMDNFC = new findMDNFClass();
     FMDNFC->certainMDNF();
}

void MainWindow::on_btnApply2_clicked()
{
    QString str = "";
    for (int i = 0; i < varCountp2; ++i){
        QString s = ui->mainTable->item(i,0)->text();
        str+=s;
    }
    if (str == currentFunction){
        return;
    }
    minLen = 1000000000;
    answers.clear();
    needOnes = 0;
    selectedOnes.clear();
    selectedSets.clear();
    accepted.clear();
    banned.clear();
    currentFunction = str;
    buildTable();
    // thread = new QThread(this);
     FMDNFC = new findMDNFClass();
    /* FMDNFC->moveToThread(thread);
     QObject::connect(thread, &QThread::finished, FMDNFC, &QObject::deleteLater);
     QObject::connect(this, &MainWindow::mySignal , FMDNFC, &findMDNFClass::certainMDNF);
     thread->start();*/
     FMDNFC->certainMDNF();
}



