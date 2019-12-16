#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "ui_mainwindow.h"

#define DEBUG_MESSAGE "%s: %d\n", __FILE__, __LINE__
#define PRINT_DEBUG_MESSAGE() fprintf(stderr, DEBUG_MESSAGE);fflush(stderr);

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget *parent=nullptr);
		virtual ~MainWindow();
};

#endif //MAINWINDOW_H
