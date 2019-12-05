#include "findmdnfclass.h"
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
#include <QMovie>
extern int varCount;
extern int needOnes;
extern QMap <int, QStringList> headMap;
extern QMap <int, QStringList> banned;
extern QMap <int, QMap <QString, QList<int> > > accepted; // col, set, rows
extern QMap <int, int> selectedOnes; //row, bool
extern QMap <int, QMap <QString, bool> > selectedSets;  // col, str, bool
extern int minLen;
extern QString varLbl[6];
extern QMap <QString, int> assoc;
extern QMap <int, QList <QStringList> >answers;
extern QString currentFunction;
extern int varCountp2;
extern int pow(int, int);
extern bool checkDubl(QStringList list);
extern bool isOverlapped (int col, QString set);
extern QString getTerm(QString head, QString val);
extern Ui::MainWindow *extUi;
findMDNFClass::findMDNFClass()
{
    qDebug() << "Class used";
}
















void findMDNF(int curLen, QStringList curDNF, int idx, int ones){
     if(ones == needOnes) {
        if (curLen <= minLen){
            minLen = curLen;
            curDNF.sort();
            if (checkDubl(curDNF)){
            answers[curLen] << curDNF;
            }
        }
        return;
    }
     QCoreApplication::processEvents();
     int tmpIdx = 0;
    for (int i = 1; i < varCountp2; ++i){ //col
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

           // findMDNF(curLen+key.length(), curDNF, idx+1);

           }

        }
    }
}


void findMDNFClass::certainMDNF(){
    QTextBrowser * lbl = extUi->lblAns;
    QLabel * lblGif = extUi->lblGif;
    QMovie * mov = new QMovie(":img/main.gif");
    if(!mov->isValid()){
        qDebug() << "aaaaaa";
    }
    lblGif->setMovie(mov);
    mov->start();
    lbl->setText("Загрузка...");
    QStringList garbageList;
    findMDNF(0, garbageList,0, 0);


    QString s1, s2;
    s1.setNum(minLen);
    s2.setNum(answers[minLen].length());
    if (minLen == 0){
        s2.setNum(0);
    }
    QString strr = "Длина: " + s1 + "    Кол-во: "+s2+"\n";
    for (int i = 0; i < answers[minLen].length(); ++i){
        strr += answers[minLen][i].join(" v ") + "\n";
    }
    lbl->setText(strr);
    lblGif->clear();
}




