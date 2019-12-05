#ifndef FINDMDNFCLASS_H
#define FINDMDNFCLASS_H
#include <QObject>
#include <ui_mainwindow.h>
#include <QMainWindow>

class findMDNFClass : public QObject
{
    Q_OBJECT

public:
    findMDNFClass();

public slots:
    void certainMDNF();

};

#endif // FINDMDNFCLASS_H
