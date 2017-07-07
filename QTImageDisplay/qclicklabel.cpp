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

