//#ifndef IFUNWIDGET_H
//#define IFUNWIDGET_H

//#include <QWidget>
//#include <QVariant>
//#include "Interface/IController.h"
//#include "Interface/IModel.h"

//class IFunWidget : public QWidget
//{
//    Q_OBJECT
//public:

//    explicit IFunWidget(QWidget *parent = nullptr);
//    virtual ~IFunWidget();
//    virtual void initWidget() = 0;
//    virtual bool isEnter();
//    virtual bool isQuit();
//    virtual void quit();
//    virtual void updateWidget();
//    virtual void showedWidgetDo();

//    IController *getCL();
//    void setCL(IController *cl);

//    IModel *getModel();
//    void setModel(IModel *md);

//    bool setName(QString name);
//    QString getName();
//protected:
//    void paintEvent(QPaintEvent* event);
//public slots:

//    virtual void onClDataCome(int cmd, QVariant data = INT_INVALID);
//    virtual void onModelUpdate(int cmd);

//private:

//    IController  *m_pICL;
//    IModel       *m_pIModel;
//private:
//    QString     _name;
//};

//#endif // IFUNWIDGET_H

#ifndef IFUNWIDGET_H
#define IFUNWIDGET_H

#include <QWidget>
#include <QVariant>
#include "Interface/IController.h"
#include "Interface/IModel.h"
//#include "MainFile/widgetsize.h"

class IFunWidget : public QWidget
{
    Q_OBJECT
public:

    explicit IFunWidget(QWidget *parent = nullptr);
    virtual ~IFunWidget();
    virtual void initWidget();
    virtual bool isEnter();
    virtual bool isQuit();
    virtual void quit();
    virtual void updateWidget();
    virtual void showedWidgetDo();

    bool setName(QString name);
    QString getName();

    IController *getCL();
    void setCL(IController *cl);

    IModel *getModel();
    void setModel(IModel *md);
protected:
    void paintEvent(QPaintEvent* event);

public slots:

    virtual void onClDataCome(int cmd, QVariant data = INT_INVALID);
    virtual void onModelUpdate(int cmd);

protected:
    IController  *m_pICL=nullptr;
    IModel     *m_pIModel=nullptr;

private:
    QString     name;

};

#endif // IFUNWIDGET_H
