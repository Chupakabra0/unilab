#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <string>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include "tclasses.h"

class Point {
private:
	GLfloat cord[3]; // cordinats
public:
	Point();
	Point(GLfloat, GLfloat);
	Point(GLfloat, GLfloat, GLfloat);
	Point(GLfloat cord[3]);
	void set(GLfloat, GLfloat);
	void set(GLfloat, GLfloat, GLfloat);
	void set(GLfloat cord[3]);
	void set_x(GLfloat x);
	void set_y(GLfloat y);
	void set_z(GLfloat z);
	GLfloat get_x();
	GLfloat get_y();
	GLfloat get_z();
	GLfloat * get_cords() const;
	void vertex();
	void put_on_line(const Point& a, const Point& b, GLfloat t);
	void calculate_third(Point, Point, GLfloat, GLfloat, int);
	GLfloat distance(Point& p);
	Point& operator=(const Point&);
	friend bool operator==(const Point& left, const Point& right);
	friend bool operator!=(const Point& left, const Point& right);
	friend Point operator+(const Point& left, const Point& right);
	friend Point& operator+=(Point& left, const Point& right);
	friend const GLfloat operator*(const Point&, const Point&);
};

bool operator==(const Point& left, const Point& right);
bool operator!=(const Point& left, const Point& right);
Point operator+(const Point& left, const Point& right);
Point& operator+=(Point& left, const Point& right);
const GLfloat operator*(const Point&, const Point&);

class Canvas {
private:
	Point CP;
	// current position in the world
	GLfloat CD;
	// current degree in the world
	Rect<GLint> viewport;
	// the current viewport
	Rect<GLfloat> window;
	// the current window
	GLint window_width, window_height;
public:
	Canvas();
	// constructor
	void init(int argc, char * argv[], GLint width, GLint height,
		char * window_title);
	void resize(GLint width, GLint height);
	void set_viewport(GLint l, GLint r, GLint b, GLint  t);
	void set_window(GLfloat l, GLfloat r, GLfloat b, GLfloat t);
	Rect<GLint> get_viewport();
	// divulge the viewport data
	Rect<GLfloat> get_window();
	// divulge the window data
	GLfloat get_width();
	GLfloat get_height();
	GLfloat get_window_aspect_ratio();
	void clear_screen();
	void set_background_color(GLfloat r, GLfloat g, GLfloat b);
	void set_color(GLfloat r, GLfloat g, GLfloat b);
	void line_to(GLfloat x, GLfloat y);
	void line_to(Point p);
	void move_to(GLfloat x, GLfloat y);
	void move_to(Point p);
	void line_rel(GLfloat x, GLfloat y);
	void move_rel(GLfloat x, GLfloat y);
	void turn(GLfloat angl);
	void turn_to(GLfloat angl);
	void forward(GLfloat dist, int is_visible);
};

class PointArray {
protected:
	std::vector<Point> arr;
public:
	PointArray();
	PointArray(size_t rows);
	void clear();
	void push(const Point& p);
	void push(GLfloat cords[3]);
	void set(size_t i, const Point& p);
	std::vector<Point> get_arr() const;
	void move_to(Point p);
	void render();
	friend const PointArray operator*(const PointArray&, const PointArray&);
	PointArray& operator=(const PointArray& right);
};

const PointArray operator*(const PointArray&, const PointArray&);

class Basis : public PointArray {
public:
	Basis();
	void rotate_to(GLfloat angle);
	void rotate_rel(GLfloat angle);
	Basis& operator=(const PointArray& right);
};

class Path : public PointArray {
private:
	Point CP;
	std::string commands;
public:
	Path();
	Path(std::string pth);
	void set(std::string pth);
	void push_command(char com, Point p1);
	void push_command(char com, Point p1, Point p2);
	void push_command(char com, Point p1, Point p2, Point p3);
	void generate(PointArray& dest);
	void q_bezier(PointArray& dest, Point p1, Point p2, Point p3);
};

class Object {
private:
	Path path_orig, path;
	PointArray points_orig, points;
	Basis basis;
	Point pos;
public:
	Object();
	Object(std::string pth);
	Object(Path pth);
	void set(std::string pth);
	void set(Path pth);
	void move_to(Point p);
	void move_rel(Point p);
	void rotate_to(GLfloat angle);
	void rotate_rel(GLfloat angle);
	void render();
};
