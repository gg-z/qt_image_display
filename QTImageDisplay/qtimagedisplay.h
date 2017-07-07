#ifndef QTIMAGEDISPLAY_H
#define QTIMAGEDISPLAY_H

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QImage>
#include <vector>
#include <QScrollBar>
#include "ui_qtimagedisplay.h"
#include "qclicklabel.hpp"


class QTImageDisplay : public QMainWindow
{
	Q_OBJECT

public:
	QTImageDisplay(QWidget *parent = 0);
	~QTImageDisplay();


	QClickLabel * qclicklabel;
	QAction *action_open_img_file;
	QAction *action_exit;


	// class variable
	int i_show_image_label_width;
	int i_show_image_label_height;

	// image show start x, start y
	int i_show_image_start_x;
	int i_show_image_start_y;

	// image zoom in ratio
	float f_iamge_ratio_x;
	float f_iamge_ratio_y;

	// zoom parameter
	float f_zoom_step;
	float f_zoom_ratio_max;
	float f_zoom_ratio_min;
	float f_zoom_current_scale;


	// QImage front image pointer
	QImage* qimage_front_image;
	bool b_is_front_image_opened;

	// bool variable
	bool is_translate_image;
	int i_mouse_last_position_x;
	int i_mouse_last_position_y;
	int i_mouse_new_position_x;
	int i_mouse_new_position_y;
	bool b_mouse_right_button_clicked;
	bool b_mouse_left_button_clicked;
	
	//**************************************
	//functions
	//**************************************
	void show_image();
	void translate_image();


private:
	Ui::QTImageDisplayClass ui;


private slots:
	// slots
	void slot_open_img_file();
	void slot_qclicklabel_wheel_move(QWheelEvent* evt);
	// used to select edge point and translate image
	void slot_qclicklabel_mouse_press(QMouseEvent* evt);
	void slot_qclicklabel_mouse_move(QMouseEvent* evt);
	void slot_qclicklabel_mouse_release(QMouseEvent* evt);
};

#endif // QTIMAGEDISPLAY_H
