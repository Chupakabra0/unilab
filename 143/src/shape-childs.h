/***************************************************************
 * Name:      shape-childs.h
 * Purpose:   Defines shapes childs
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#ifndef SHAPECHILDS_H
#define SHAPECHILDS_H

#include "shapes.h"

namespace ShapeChilds {

class Triangle : public Shape {
public:
  Triangle() = default;
	static const Pointer<Shape> create();
  const Pointer<Shape> clone() override;
  void drawShape(const Cairo::RefPtr<Cairo::Context>& context) override;
  bool isInShapeVirtual(const Point& p) const override;
};


class Rectangle : public Shape {
public:
  Rectangle() = default;
	static const Pointer<Shape> create();
  const Pointer<Shape> clone() override;
  void drawShape(const Cairo::RefPtr<Cairo::Context>& context) override;
  bool isInShapeVirtual(const Point& p) const override;
};


class Ellipse : public Shape {
public:
  Ellipse() = default;
	static const Pointer<Shape> create();
  const Pointer<Shape> clone() override;
  void drawShape(const Cairo::RefPtr<Cairo::Context>& context) override;
  bool isInShapeVirtual(const Point& p) const override;
};

class Aggregator : public Shape {
public:
  Aggregator() : test(Ellipse::create()), defaultSize_(1, 1) {}
	static const Pointer<Shape> create();
  const Pointer<Shape> clone() override;
  void drawShape(const Cairo::RefPtr<Cairo::Context>& context) override;
  bool isInShapeVirtual(const Point& p) const override;
  const Size& getDefaultSize() const override;

private:
	Pointer<Shape> test;
  Size defaultSize_;
};

}

#endif // SHAPECHILDS_H
