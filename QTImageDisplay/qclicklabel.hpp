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
