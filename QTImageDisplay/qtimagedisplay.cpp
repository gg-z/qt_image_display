#include "qtimagedisplay.h"

QTImageDisplay::QTImageDisplay(QWidget *parent)
	: QMainWindow(parent)
{
	// 1. set
	ui.setupUi(this);
	//set fix size
	setFixedSize(600, 850);


	// 2. get the widget pointer
	qclicklabel = ui.label;
	action_open_img_file = ui.actionOpen_iamge_file;
	action_exit = ui.actionExit;

	// 3. initiate the widget
	qclicklabel->setAutoFillBackground(true); // enable us can change the background
	QPalette label_palette = qclicklabel->palette();
	label_palette.setColor(QPalette::Window, Qt::lightGray);   //change the background 
	qclicklabel->setPalette(label_palette);
	qclicklabel->setEnabled(true);

	// get the width and height
	i_show_image_label_width = qclicklabel->width();
	i_show_image_label_height = qclicklabel->height();

	// start x, start y
	i_show_image_start_x = 0;
	i_show_image_start_y = 0;

	// image scale ratio
	f_iamge_ratio_x = 1.0f;
	f_iamge_ratio_y = 1.0f;

	// zoom parameter
	f_zoom_step = 0.5;
	f_zoom_ratio_max = 4;
	f_zoom_ratio_min = 1;
	f_zoom_current_scale = 1.0;

	// image translate
	b_mouse_left_button_clicked = false;
	b_mouse_right_button_clicked = false;

	// 4. connect signals and slots 
	connect(action_open_img_file, SIGNAL(triggered()), this, SLOT(slot_open_img_file()));
	connect(action_exit, SIGNAL(triggered()), this, SLOT(close()));
	connect(qclicklabel, SIGNAL(MouseMoved(QMouseEvent *)), this, SLOT(slot_qclicklabel_mouse_move(QMouseEvent *)));
	connect(qclicklabel, SIGNAL(MousePressed(QMouseEvent *)), this, SLOT(slot_qclicklabel_mouse_press(QMouseEvent *)));
	connect(qclicklabel, SIGNAL(MouseRelease(QMouseEvent *)), this, SLOT(slot_qclicklabel_mouse_release(QMouseEvent *)));
	connect(qclicklabel, SIGNAL(WheelEvent(QWheelEvent *)), this, SLOT(slot_qclicklabel_wheel_move(QWheelEvent*)));



}

// wheel to zoom in and zoom out
void QTImageDisplay::slot_qclicklabel_wheel_move(QWheelEvent* evt) {

	// get the delta
	float f_degree_delta = evt->delta()*1.0 / 640;

	// case1 : front image opened
	if (b_is_front_image_opened) {

		// zoom in and zoom out
		// update scale ratio
		if (f_zoom_current_scale + f_degree_delta > f_zoom_ratio_max || f_zoom_current_scale + f_degree_delta < f_zoom_ratio_min) {
			return;
		}
		f_zoom_current_scale += f_degree_delta;
		f_iamge_ratio_x = qimage_front_image->width()*1.0 / (f_zoom_current_scale*qclicklabel->width());
		f_iamge_ratio_y = qimage_front_image->height()*1.0 / (f_zoom_current_scale*qclicklabel->height());

		// show image
		show_image();
	}
}


// show front image
void QTImageDisplay::show_image() {

	// test if qimage_front_image is not opened
	if (qimage_front_image == NULL) {
		return;
	}

	// show image 
	QImage qimage_temp = qimage_front_image->copy();

	// judge if the start_x or start_y is legal
	if (i_show_image_start_x + qclicklabel->width() > qclicklabel->width()*f_zoom_current_scale) {
		i_show_image_start_x = qclicklabel->width()*f_zoom_current_scale - qclicklabel->width() - 1;
	}
	if (i_show_image_start_y + qclicklabel->height() > qclicklabel->height()*f_zoom_current_scale) {
		i_show_image_start_y = qclicklabel->height()*f_zoom_current_scale - qclicklabel->height() - 1;
	}

	QImage * qimage_scaled = new QImage;
	*qimage_scaled = qimage_temp.scaled(QSize(qclicklabel->width()*f_zoom_current_scale, qclicklabel->height()*f_zoom_current_scale), Qt::IgnoreAspectRatio).copy(i_show_image_start_x, i_show_image_start_y, qclicklabel->width(), qclicklabel->height());
	qclicklabel->setPixmap(QPixmap::fromImage(*qimage_scaled));
	delete qimage_scaled;

}


// open front image
void QTImageDisplay::slot_open_img_file() {

	QString str_slected_image = QFileDialog::getOpenFileName(this,
		tr("select image file"),
		".//",
		tr("Image (*.png *.jpg *.xpm)"));

	// front image pointer
	qimage_front_image = new QImage();
	if (!str_slected_image.isEmpty()) {

		// validate the image is correctly opened
		if (!(qimage_front_image->load(str_slected_image))) {

			QMessageBox::information(this,
				tr("info"),
				tr("open image fail!"));
			delete qimage_front_image;
			return;
		}


		// record the image scale ration x,y
		f_iamge_ratio_x = qimage_front_image->width()*1.0 / qclicklabel->width();
		f_iamge_ratio_y = qimage_front_image->height()*1.0 / qclicklabel->height();


		// get a image copy and show the image
		QImage qimage_temp = qimage_front_image->copy();

		QImage * qimage_scaled = new QImage;
		*qimage_scaled = qimage_temp.scaled(QSize(qclicklabel->width(), qclicklabel->height()), Qt::IgnoreAspectRatio);
		qclicklabel->setPixmap(QPixmap::fromImage(*qimage_scaled));
		delete qimage_scaled;

		// change status
		b_is_front_image_opened = true;

		// reset parameters about show image
		f_zoom_current_scale = 1;
		// start x, start y
		i_show_image_start_x = 0;
		i_show_image_start_y = 0;


	}
	else {
		// log out information
		QMessageBox::information(this,
			tr("info"),
			tr("you select no file!"));
	}

}



// used to select edge point and translate image
void QTImageDisplay::slot_qclicklabel_mouse_press(QMouseEvent* evt) {

	// case1 : front image is opened
	if (b_is_front_image_opened) {
		// select edge point
		if (evt->button() == Qt::LeftButton) {
			
			b_mouse_left_button_clicked = true;
		}else if (evt->button() == Qt::RightButton) {
			// translate image
			b_mouse_right_button_clicked = true;
			i_mouse_last_position_x = evt->localPos().x();
			i_mouse_last_position_y = evt->localPos().y();
		}
	}
}

void QTImageDisplay::slot_qclicklabel_mouse_move(QMouseEvent* evt) {

	// case 1: front image is opened;
	if (b_is_front_image_opened) {
		if (b_mouse_right_button_clicked) {
			i_mouse_new_position_x = evt->localPos().x();
			i_mouse_new_position_y = evt->localPos().y();
			translate_image();
		}
	}
}

// translate front image
void QTImageDisplay::translate_image() {
	// 
	int i_distance = (i_mouse_new_position_x - i_mouse_last_position_x)*(i_mouse_new_position_x - i_mouse_last_position_x) + (i_mouse_new_position_y - i_mouse_last_position_y)*(i_mouse_new_position_y - i_mouse_last_position_y);
	if (i_distance < 50) {
		return;
	}

	// compute the delta
	int i_delta_x = -1 * (i_mouse_new_position_x - i_mouse_last_position_x);
	int i_delta_y = -1 * (i_mouse_new_position_y - i_mouse_last_position_y);


	// start x exceeds the left edge
	if (i_show_image_start_x + i_delta_x < 0) {
		i_show_image_start_x = 0;

	}
	else if (i_show_image_start_x + i_delta_x + qclicklabel->width() > int(f_zoom_current_scale*qclicklabel->width() - 1)) {
		//start x exceeds the right edge
		i_show_image_start_x = int(f_zoom_current_scale*qclicklabel->width() - qclicklabel->width());
	}
	else {
		i_show_image_start_x += i_delta_x;
	}

	// start y exceeds the top edge
	if (i_show_image_start_y + i_delta_y < 0) {
		i_show_image_start_y = 0;

	}
	else if (i_show_image_start_y + i_delta_y + qclicklabel->height() > int(f_zoom_current_scale*qclicklabel->height() - 1)) {
		//start y exceeds the bottom edge
		i_show_image_start_y = int(f_zoom_current_scale*qclicklabel->height() - qclicklabel->height());
	}
	else {
		i_show_image_start_y += i_delta_y;
	}

	// update the mouse last position 
	i_mouse_last_position_x = i_mouse_new_position_x;
	i_mouse_last_position_y = i_mouse_new_position_y;

	// show image 
	show_image();
}


//mouse wheel move event
void QTImageDisplay::slot_qclicklabel_mouse_release(QMouseEvent* evt) {

	//change the flag
	if (evt->button() == Qt::LeftButton) {
		b_mouse_left_button_clicked = false;
	}
	else if (evt->button() == Qt::RightButton) {
		b_mouse_right_button_clicked = false;
	}
}



QTImageDisplay::~QTImageDisplay()
{



}
