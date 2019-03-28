/***************************************************************
 * Name:      window.cpp
 * Purpose:   Implementation of window widget
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#include "window.h"
#include "shape-childs.h"
#include <chrono>
#include <thread>

Timer::Timer(Glib::Dispatcher& dispatcher) : dispatcher_(dispatcher) {}

void Timer::do_work() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		dispatcher_.emit();
	}
}

template<class T>
void getObject(
	Glib::RefPtr<Gtk::Builder> builder, Glib::RefPtr<T>& pointer, const char* name
) {
	pointer = Glib::RefPtr<T>::cast_dynamic(
		builder->get_object(name)
	);
	if (!pointer) {
		printf("No %s\n", name);
		exit(1);
	}
}

Window::Window(
	BaseObjectType* cobject,
	const Glib::RefPtr<Gtk::Builder>& builder
) : Gtk::ApplicationWindow(cobject), builder_(builder),
shapes_(), dispatcher(), timer(dispatcher) {
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("add_rectangle_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::addRectangle));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("add_triangle_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::addTriangle));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("add_ellipse_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::addEllipse));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("trace_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::toggleTrace));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("reset_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::reset));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("color_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::changeColor));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("visibility_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::toggleVisibility));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("zoom_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::zoomShape));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("clone_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::cloneShape));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("delete_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::deleteShape));
		}
	}

	getObject(builder_, nAdjustment_, "n_adjustment");
	getObject(builder_, drawingArea_, "drawing_area");
	getObject(builder_, statusbar_, "statusbar");


	getObject(builder_, xAdjustment_, "x_adjustment");
	xAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, yAdjustment_, "y_adjustment");
	yAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, widthAdjustment_, "width_adjustment");
	widthAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, heightAdjustment_, "height_adjustment");
	heightAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, minimumZoomAdjustment_, "minimum_zoom_adjustment");
	minimumZoomAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, traceSizeAdjustment_, "trace_size_adjustment");
	traceSizeAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, traceTimeAdjustment_, "trace_time_adjustment");
	traceTimeAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));

	drawingArea_->add_events(
		Gdk::BUTTON_PRESS_MASK |
		Gdk::BUTTON_MOTION_MASK |
		Gdk::BUTTON_RELEASE_MASK
	);
	drawingArea_->signal_draw().connect(sigc::mem_fun(
		*this, &Window::draw
	));
	drawingArea_->signal_button_press_event().connect(sigc::mem_fun(
		*this, &Window::activate
	));
	drawingArea_->signal_motion_notify_event().connect(sigc::mem_fun(
		*this, &Window::moveActive
	));
	drawingArea_->signal_button_release_event().connect(sigc::mem_fun(
		*this, &Window::release
	));

	dispatcher.connect(sigc::mem_fun(*this, &Window::update));
	new std::thread(&Timer::do_work, &timer);
	parametersChanged();
}

Window::~Window() {
}

void Window::quit() {
	exit(0);
}

void Window::update() {
	drawingArea_->queue_draw();
}

bool Window::draw(const Cairo::RefPtr<Cairo::Context>& context) {
	Gtk::Allocation allocation = drawingArea_->get_allocation();
	context->set_antialias(Cairo::ANTIALIAS_NONE);
	context->set_line_width(3);
	context->set_line_cap(Cairo::LINE_CAP_ROUND);
	shapes_.draw(context, allocation);
	return true;
}

void Window::parametersChanged() {
	SHAPE.setX(xAdjustment_->get_value());
	SHAPE.setY(yAdjustment_->get_value());
	SHAPE.setWidth(widthAdjustment_->get_value());
	SHAPE.setHeight(heightAdjustment_->get_value());
	SHAPE.setMinimumZoom(minimumZoomAdjustment_->get_value());
	SHAPE.setTraceSize(traceSizeAdjustment_->get_value());
	SHAPE.setTraceTime(traceTimeAdjustment_->get_value());
	update();
}

void Window::addRectangle() {
	int n = nAdjustment_->get_value();
	for (int i = 0; i < n; i++) {
		shapes_.add(ShapeChilds::Rectangle::create());
	}
	update();
}

void Window::addTriangle() {
	int n = nAdjustment_->get_value();
	for (int i = 0; i < n; i++) {
		shapes_.add(ShapeChilds::Triangle::create());
	}
	update();
}

void Window::addEllipse() {
	int n = nAdjustment_->get_value();
	for (int i = 0; i < n; i++) {
		shapes_.add(ShapeChilds::Ellipse::create());
	}
	update();
}

void Window::toggleTrace() {
	try {
		shapes_.getActive().toggleTrace();
	} catch(const gauraException&) {}
	update();
}

void Window::reset() {
	try {
		shapes_.getActive().reset();
	} catch(const gauraException&) {}
	update();
}

void Window::changeColor() {
	try {
		shapes_.getActive().changeColor();
	} catch(const gauraException&) {}
	update();
}

void Window::toggleVisibility() {
	try {
		shapes_.getActive().toggleVisibility();
	} catch(const gauraException&) {}
	update();
}

void Window::zoomShape() {
	try {
		shapes_.getActive().toggleZoom();
	} catch(const gauraException&) {}
	update();
}

void Window::cloneShape() {
	try {
		int n = nAdjustment_->get_value();
		for (int i = 0; i < n; i++) {
			shapes_.add(shapes_.getActive().clone());
		}
	} catch(const gauraException&) {}
	update();
}

void Window::deleteShape() {
	shapes_.erase(shapes_.getActiveId());
	update();
}

bool Window::activate(GdkEventButton* event) {
	if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1)) {
		shapes_.activate(Point(event->x, event->y));
		update();
	}
	return true;
}

bool Window::moveActive(GdkEventMotion* event) {
	shapes_.moveActive(Point(event->x, event->y));
	update();
	return true;
}

bool Window::release(GdkEventButton* event) {
	shapes_.release();
	update();
	return true;
}
