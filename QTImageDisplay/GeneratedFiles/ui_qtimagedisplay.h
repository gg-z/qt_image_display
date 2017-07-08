/********************************************************************************
** Form generated from reading UI file 'qtimagedisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTIMAGEDISPLAY_H
#define UI_QTIMAGEDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qclicklabel.hpp"
QT_BEGIN_NAMESPACE

class Ui_QTImageDisplayClass
{
public:
    QAction *actionOpen_iamge_file;
    QAction *actionExit;
    QWidget *centralWidget;
    QClickLabel *label;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QTImageDisplayClass)
    {
        if (QTImageDisplayClass->objectName().isEmpty())
            QTImageDisplayClass->setObjectName(QStringLiteral("QTImageDisplayClass"));
        QTImageDisplayClass->resize(600, 850);
        actionOpen_iamge_file = new QAction(QTImageDisplayClass);
        actionOpen_iamge_file->setObjectName(QStringLiteral("actionOpen_iamge_file"));
        actionExit = new QAction(QTImageDisplayClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(QTImageDisplayClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QClickLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 601, 800));
        QTImageDisplayClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QTImageDisplayClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 17));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        QTImageDisplayClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QTImageDisplayClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QTImageDisplayClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QTImageDisplayClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QTImageDisplayClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_iamge_file);
        menuFile->addAction(actionExit);

        retranslateUi(QTImageDisplayClass);

        QMetaObject::connectSlotsByName(QTImageDisplayClass);
    } // setupUi

    void retranslateUi(QMainWindow *QTImageDisplayClass)
    {
        QTImageDisplayClass->setWindowTitle(QApplication::translate("QTImageDisplayClass", "QTImageDisplay", 0));
        actionOpen_iamge_file->setText(QApplication::translate("QTImageDisplayClass", "open image file", 0));
        actionExit->setText(QApplication::translate("QTImageDisplayClass", "exit", 0));
        label->setText(QString());
        menuFile->setTitle(QApplication::translate("QTImageDisplayClass", "file", 0));
    } // retranslateUi

};

namespace Ui {
    class QTImageDisplayClass: public Ui_QTImageDisplayClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTIMAGEDISPLAY_H
