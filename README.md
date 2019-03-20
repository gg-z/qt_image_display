### QTImageDisplay
a vs 2015 project using qlable to display image,supporting zoom in,zoom out, and translate image. On QT 5.6.2 

### Author
If the code presented helps you, it literally is delight to me ^.^ .



本文主要介绍，如何使用在VS2015平台上集成QT环境实现一个控件（Widget），该控件可以展示图片，并且该支持滚动鼠标中间放大缩小图片，右键移动鼠标进行图片的拖动。本文将从以下几个部分进行描述：
1. 将QT集成到VS2015平台
2. 自定义控件描述
3. 图片放大、缩小、移动的实现
4. 工程效果
5. 总结
------
#### 一、 将QT集成到VS2015平台
<br>
主要过程：安装QT、配置环境、和安装vs2015插件。在开源社区的一篇博客描述得很清楚，在这里不赘述了。[点此跳转到该博客](https://my.oschina.net/jthmath/blog/640227)

#### 二、 自定以控件描述
<br>
在QT当中没有专门用来展示图片的控件，通常用于展示图片的控件是QLabel，但是该控件不支持图片的缩放移动等功能，所以就自定义一个控件，使其支持上述功能。
先上代码：

**qclicklabel.hpp**：
```c++
#pragma once
#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>

class QClickLabel : public QLabel {
	Q_OBJECT
signals :
	void MouseRelease(QMouseEvent *evt);
	void KeyPressed(QKeyEvent *evt);
	void MouseMoved(QMouseEvent* evt);
	void MousePressed(QMouseEvent* evt);
	void MouseDoubliClick(QMouseEvent* evt);
	void WheelEvent(QWheelEvent* evt);

public:
	QClickLabel(QWidget * parent = Q_NULLPTR): QLabel(parent)
	{}
	~QClickLabel();

protected:
	void mouseReleaseEvent(QMouseEvent*);  // listen to mouse event
	void mousePressEvent(QMouseEvent*);  // listen to mouse event
	void mouseMoveEvent(QMouseEvent*);  // listen to mouse event
	void keyPressEvent(QKeyEvent *);  // listen to key press event
	void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	
};
```
QClickLabel继承于QLabel，该控件能够支持鼠标的点击、移动、释放、双击、和中键滚动事件（代码protected部分定义）；同时该控件能够发射出相应的信号（代码signals处定义）

<br>

**qclicklabel.cpp**：
```c++
#include "qclicklabel.hpp"

// destructor
QClickLabel::~QClickLabel() {
	
}


void QClickLabel::mouseReleaseEvent(QMouseEvent *evt)
{
	qDebug() << "in function ClickedLabel mouse release" << endl;
	emit MouseRelease(evt);
}


void QClickLabel::keyPressEvent(QKeyEvent *evt) {
	// listen to key press event
	qDebug() << "in function ClickedLabel key press" << endl;
	emit KeyPressed(evt);
}


void QClickLabel::mouseMoveEvent(QMouseEvent *event) {
	qDebug() << "in function mouseMoveEvent" << endl;
	emit MouseMoved(event);
}


void QClickLabel::mousePressEvent(QMouseEvent *event) {
	qDebug() << "in function mousePressEvent" << endl;
	emit MousePressed(event);
}


void QClickLabel::mouseDoubleClickEvent(QMouseEvent *event) {
	qDebug() << "in function mouseDoubleClickEvent" << endl;
	emit MouseDoubliClick(event);
}

void QClickLabel::wheelEvent(QWheelEvent *event) {
	qDebug() << "in function wheelEvent" << endl;
	emit WheelEvent(event);
}
```
在**qclicklabel.cpp**实现部分，控件接收到任何鼠标事件，都发送相应的信号出去，以便在主程序当中做出相应的操作。


#### 三、 图片放大、缩小、移动的实现
<br>

在自定以的控件当中，由于控件可以直接获取到鼠标事件，并且根据获取到的鼠标事件发出相应的信号，所以在主程序当中，我们只需要监听监听该控件，如果发出信号A，就做出相应的操作。

**图片放大和缩小：**
在此工程的实现中，加载到内存当中的图片都是以横纵铺满的方式展示在qlabel中，如下图所示
 ![图 1  展示图片](http://upload-images.jianshu.io/upload_images/1769441-41ccea0d7eaae114.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

<br>
此时展示的图片的宽高就和qlabel控件的宽高一致了，所以原图片的宽高 ``o_img_width、o_img_height``和qclicklabel的宽高 ``show_width、show_height``会有一个比值 ``ratio_o_2_show_width 、 ratio_o_2_show_height``。
此时，图片刚刚导入展示时图片的放大的倍数-scale为1（宽高扩大的倍数相同），鼠标的中键滚动，只需要改变这个scale在大于1的一个合理的范围，然后先将图片的宽高通过QImage里面的方法转换到show_width*****scale  , show_height*****scale，然后将图片的``Rect(show_start_x，show_start_y，show_width，show_height）-起点为（show_start
_x，show_start_y），宽高分别为show_width，show_height的长方形，此时的show_start_x，show_start_y都为0``部分展示出来既可以实现图片的放大缩小了。

响应鼠标滚动事件的槽
```c++
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
```

show_image()函数实现
```c++
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
```

<br>
**图片移动：**
在图片的移动过程中，需要使用到三个鼠标事件：鼠标单击事件、鼠标移动事件，鼠标释放事件。
在鼠标单击事件当中，我们需要记录此时点击的是鼠标的左键还是右键（用两个private variable保存该状态，代码如下所示）,并且当右键点击时，需要记录此时鼠标的位置，为了后面移动图片的需求；
```c++
// used to select edge point and translate image
void QTImageDisplay::slot_qclicklabel_mouse_press(QMouseEvent* evt) {

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
```

在鼠标移动事件当中，由于我们设置的是右键单击然后移动鼠标进行图片的移动，所以在此，我们只需要处理右键被点击时的事件，此时记录鼠标移动后的所在的新的位置，然后交由translate_image()去移动图片，并且展示，该函数主要是通过鼠标的原始位置和鼠标最新位置的差值来更新```show_start_x，show_start_y```，并且判断此时的要展示的```Rect(show_start_x，show_start_y，show_width，show_height）```是否超出了边界；
```c++
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
```
在鼠标释放事件当中，需要恢复记录鼠标的点击状态的两个private variable，代码如下所示：
```c++
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
```
至此，图片的移动也就实现了。


#### 四、 工程效果
<br>


![图 2    效果图](http://upload-images.jianshu.io/upload_images/1769441-598a07ac8097bfa0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 五、 总结
  本文只是展示了作者在实现展示图片控件的一个思路，但是还不够完美，比如可以多设置几种展示的方式，比如居中，适应宽度，适应高度，宽高独立放大缩小等等。不过如果你对于本文描述的实现方式能够理解，那么上述的一些功能只需要根据自己工程的需求去私人定制就好了。
