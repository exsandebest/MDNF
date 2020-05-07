#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QMap>
#include <QMessageBox>
#include <QMovie>
#include <qmath.h>

int varCount = 3;
int needOnes = 0;
int minLen = 1000000000;
int varCountP2 = pow(2, varCount);
QString currentFunction = "00000000";
QString varLbl[6] = {"a","b","c","d","e","f"};

QMap <int, QStringList> headMap;
QMap <int, QStringList> banned;
QMap <int, QMap <QString, QList<int> > > accepted; // col, set, rows
QMap <int, int> selectedOnes; //row, bool
QMap <int, QMap <QString, bool> > selectedSets;  // col, str, bool
QMap <QString, int> assoc;
QMap <int, QList <QStringList> >answers;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    tmpList << "a" << "b" << "c" << "d" << "ab" << "ac" << "ad" <<"bc" <<"bd" << "cd" << "abc" << "abd" << "acd" << "bcd" << "abcd";
    headMap[4] = tmpList;
    tmpList.clear();
    tmpList << "a" << "b" << "c" << "d" << "e" << "ab" << "ac" << "ad" << "ae" << "bc" << "bd" << "be" << "cd" << "ce" << "de" << "abc" << "abd" << "abe" << "acd" << "ace" << "ade" <<"bcd" << "bce" <<"bde" << "cde" << "abcd" <<"abce" << "abde" << "acde" << "bcde" << "abcde";
    headMap[5] = tmpList;
    tmpList.clear();
    tmpList << "a" << "b" << "c" << "d" << "e" << "f" << "ab" << "ac" << "ad" <<"ae" << "af" << "bc" <<"bd" << "be" << "bf" << "cd" << "ce" << "cf" << "de" << "df" << "ef" << "abc" << "abd" << "abe" << "abf" << "acd" << "ace" << "acf" << "ade" << "adf" << "aef" << "bcd" << "bce" << "bcf" << "bde" << "bdf" << "bef" << "cde" << "cdf" << "cef" << "def" << "abcd" << "abce" << "abcf" <<"abde" << "abdf" << "abef" << "acde" <<"acdf" << "acef" << "adef" << "bcde" << "bcdf" << "bcef" << "bdef" << "cdef" << "abcde" << "abcdf" << "abcef" << "abdef" << "acdef" << "bcdef" << "abcdef";
    headMap[6] = tmpList;
    tmpList.clear();
    this->showMaximized();
    ui->r1->setChecked(true);
    updateVarCount(varCount);
    ui->spinBoxVarCount->setValue(varCount);
    QObject :: connect(ui->mainTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(changeCellValue(int, int)));
    MDNF();
}


MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::isBanned(int x, QString s){
    if (banned[x].indexOf(s) != -1) {
        return true;
    }
    return  false;
}

bool MainWindow::isOverlapped (int col, QString set){
    for (int i = 0; i < accepted[col][set].length(); ++i){
        if (!selectedOnes[accepted[col][set][i]]){
            return false;
        }
    }
    return true;
}

void MainWindow::setOriginalState(){
    minLen = 1000000000;
    answers.clear();
    needOnes = 0;
    selectedOnes.clear();
    selectedSets.clear();
    accepted.clear();
    banned.clear();
}

void MainWindow::MDNF(){
    buildTable();
    certainMDNF();
}

QString MainWindow::reverse(QString s){
    QString ans = "";
    for (int i = s.length()-1; i>=0; --i){
        ans+=s[i];
    }
    return ans;
}

bool MainWindow::checkDuplicates(QStringList list){
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

QString MainWindow::getTerm(QString head, QString val){
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


void MainWindow::updateVarCount(int x) {
    varCount = x;
    varCountP2 = pow(2,x);
    for(int i = 0; i < x; ++i){
        assoc[varLbl[i]] = x-i-1;
    }
}


QStringList MainWindow::getSimpleColumn(QString s){
    int x = assoc[s];
    QStringList ans;
    while(ans.length() < varCountP2){
        for (int i = 0; i < pow(2,x); ++i){
            ans<<"0";
        }
        for(int i = 0; i < pow(2,x); ++i){
            ans<<"1";
        }
    }
    return ans;
}

QStringList MainWindow::getColumn(QString s){
    QStringList ans;
    for (int i = 0; i < varCountP2; ++i){
        ans << "";
    }
    for(int i = 0; i < s.length(); ++i){
        QStringList tmpColumn = getSimpleColumn(QString(s[i]));
        for (int j = 0; j < varCountP2; ++j){
            ans[j]+=tmpColumn[j];
        }
    }
    return ans;
}


QString MainWindow::get2from10(QString s){
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

QString MainWindow::get10from2 (QString s){
    long long ansNum = 0;
    for (int i = s.length()-1; i>=0; --i){
        ansNum+=(QString(s[i]).toInt())*pow(2,i);
    }
    QString ansStr;
    ansStr.setNum(ansNum);
    return ansStr;
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
    for(int i = 0; i < tmpColumnCount; ++i){
        table->removeColumn(0);
        table->removeRow(0);
    }
    for (int i = 0; i < varCountP2; ++i){
        table->insertRow(i);
        table->insertColumn(i);
    }
    QTableWidgetItem * item = new QTableWidgetItem();
    item->setText("f(x)");
    table->setHorizontalHeaderItem(0,item);
    for(int i = 0; i < varCountP2; ++i){
        QTableWidgetItem * itm = new QTableWidgetItem();
        itm->setText(QString(currentFunction[i]));
        itm->setFlags(Qt::ItemIsEnabled);
        itm->setTextAlignment(Qt::AlignCenter);
        table->setItem(i,0,itm);
    }
    for (int i = 1; i < varCountP2; ++i){
        QTableWidgetItem * itm = new QTableWidgetItem();
        itm->setText(headMap[varCount][i-1]);
        table->setHorizontalHeaderItem(i,itm);
    }
    for (int j = 1; j < varCountP2; ++j){
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
    for (int i =0 ; i <varCountP2; ++i){
        QTableWidgetItem * itm = table->item(i,0);
        if (itm->text() == "0"){
            for (int j = 1; j < varCountP2; ++j){
                QCoreApplication::processEvents();
                QTableWidgetItem * tmpItm = table->item(i,j);
                tmpItm->setBackground(QColor(255, 0, 0, 140));
                if (!isBanned(j, tmpItm->text())){
                    banned[j] << tmpItm->text();
                }
            }
        }
    }
    for (int i =0 ; i <varCountP2; ++i){
        QTableWidgetItem * itm = table->item(i,0);
        if (itm->text() == "1"){
            ++needOnes;
            selectedOnes[i+1] = false;
            for (int j = 1; j < varCountP2; ++j){
                QCoreApplication::processEvents();
                QTableWidgetItem * tmpItm = table->item(i,j);
                if(isBanned(j,tmpItm->text())){
                    tmpItm->setBackground(QColor(0,0,225,127));
                } else {
                    tmpItm->setBackground(QColor(0,225,0,127));
                    accepted[j][tmpItm->text()] << i+1;
                }
            }
        }
    }
}


void MainWindow::findMDNF(int curLen, QStringList curDNF, int idx, int ones){
    if(ones == needOnes) {
        if (curLen <= minLen){
            minLen = curLen;
            curDNF.sort();
            if (checkDuplicates(curDNF)){
                answers[curLen] << curDNF;
            }
        }
        return;
    }
    QCoreApplication::processEvents();
    int tmpIdx = 0;
    for (int i = 1; i < varCountP2; ++i){ //col
        foreach (QString key, accepted[i].keys()){
            if(tmpIdx++ < idx) continue;
            if(isOverlapped(i,key)) continue;
            if(curLen+key.length() > minLen){
                return;
            }
            bool * ssik = &selectedSets[i][key];
            QList<int> * aik = &accepted[i][key];
            if (!*ssik){
                *ssik = true;
                for (int j = 0; j <(*aik).length(); ++j){
                    if (selectedOnes[(*aik)[j]]==0) ++ones;
                    selectedOnes[(*aik)[j]] +=1;
                }
                QString hed = headMap[varCount][i-1];
                curDNF << getTerm(hed,key);
                findMDNF(curLen+key.length(), curDNF, idx+1, ones);
                curDNF.removeLast();
                *ssik = false;
                for (int j = 0; j < (*aik).length(); ++j){
                    selectedOnes[(*aik)[j]] -=1;
                    if (selectedOnes[(*aik)[j]] == 0){
                        --ones;
                    }
                }
            }
        }
    }
}


void MainWindow::certainMDNF(){
    QTextBrowser * lbl = ui->lblAns;
    QLabel * lblGif = ui->lblGif;
    QMovie * mov = new QMovie(":img/loading.gif");
    lblGif->setMovie(mov);
    mov->start();
    lbl->setText("Loading...");
    QStringList garbageList;
    findMDNF(0, garbageList,0, 0);
    QString s1, s2;
    s1.setNum(minLen);
    s2.setNum(answers[minLen].length());
    if (minLen == 0){
        s2.setNum(0);
    }
    QString strr = "Length: " + s1 + "    Count: "+s2+"\n";
    for (int i = 0; i < answers[minLen].length(); ++i){
        strr += answers[minLen][i].join(" v ") + "\n";
    }
    lbl->setText(strr);
    lblGif->clear();
}


void MainWindow::on_btnApply_clicked()
{
    int tmpVarCount = ui->spinBoxVarCount->value();
    QString stringInput;
    if(ui->r1->isChecked()){
        stringInput = ui->funcNumberInput1->text();
        if (!QRegExp("[10]*").exactMatch(stringInput)){
            QMessageBox::about(this, "Warning!", "Incorrect value");
            return;
        }
        for (int i = 0; pow(2,tmpVarCount)-stringInput.length()>0; ++i){
            stringInput = "0"+stringInput;
        }
        ui->funcNumberInput2->setText("");
    } else if (ui->r2->isChecked()){
        stringInput = get2from10(ui->funcNumberInput2->text());
        if (!QRegExp("[0-9]*").exactMatch(stringInput)){
            QMessageBox::about(this, "Warning!", "Incorrect value");
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
        QMessageBox::about(this, "Warning!", "Incorrect value");
        return;
    }
    updateVarCount(tmpVarCount);
    setOriginalState();
    currentFunction = stringInput;
    MDNF();
}

void MainWindow::on_btnApply2_clicked()
{
    QString str = "";
    for (int i = 0; i < varCountP2; ++i){
        QString s = ui->mainTable->item(i,0)->text();
        str+=s;
    }
    if (str == currentFunction){
        return;
    }
    setOriginalState();
    currentFunction = str;
    MDNF();
}



