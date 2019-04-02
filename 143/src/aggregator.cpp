/***************************************************************
 * Name:      aggregator.cpp
 * Purpose:   Definition of Aggregator
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#include "aggregator.h"

/*************
* Aggregator *
*************/
Aggregator::Aggregator(std::vector< Glib::RefPtr<Shape> > array)
  : array_(std::move(array)) {
  Point mp1(0, 0), mp2(0, 0);
  bool first = true;
  for (auto& i : array_) {
    if (i) {
      Point p1 = i->getPosition() - i->getSize() / 2,
            p2 = i->getPosition() + i->getSize() / 2;
      if (first) {
        mp1 = p1; mp2 = p2;
        first = false;
      }
      if (p1.getX() < mp1.getX()) {
        mp1.setX(p1.getX());
      }
      if (p1.getY() < mp1.getY()) {
        mp1.setY(p1.getY());
      }
      if (p2.getX() > mp2.getX()) {
        mp2.setX(p2.getX());
      }
      if (p2.getY() > mp2.getY()) {
        mp2.setY(p2.getY());
      }
    }
  }
  Point position = (mp1 + mp2) / 2,
            size =  mp2 - mp1;

  setSize(size);
  setPosition(floor(position));
  for (auto& i : array_) {
    if (i) {
      i->setSizeForce(i->getSize() * 2.0f / size);
      i->setPosition((i->getPosition() - position) * 2.0f / size);
      if (i->isSelected())
        i->toggleSelection();
    }
  }
}

Aggregator::Aggregator(const Aggregator& object) : Shape(object) {
  for(auto& i : object.array_) {
    if (i) {
      array_.emplace_back(i->clone());
    }
  }
}

const Glib::RefPtr<Shape> Aggregator::create(
  const std::vector< Glib::RefPtr<Shape> >& array
) {
  return Glib::RefPtr<Shape>(new Aggregator(array));
}

const Glib::RefPtr<Shape> Aggregator::clone() {
  return Glib::RefPtr<Shape>(new Aggregator(*this));
}

void Aggregator::drawShape(
    const Cairo::RefPtr<Cairo::Context>& context,
    float alpha
) {
  for (auto& i : array_) {
    if (i) {
      i->draw(context, alpha);
    }
  }
}

bool Aggregator::isInShapeVirtual(const Point& point) const {
  for (auto& i : array_) {
    if (i && i->isInShape(point))
      return true;
  }
  return false;
}

void Aggregator::toggleSelectionVirtual() {
  for (auto& i : array_) {
    if (i) {
      i->toggleSelection();
    }
  }
}

const std::vector< Glib::RefPtr<Shape> > Aggregator::deaggregate() {
  std::vector< Glib::RefPtr<Shape> > arr;
  arr.reserve(array_.size());
  for (auto& i : array_) {
    if (i) {
      auto s = i->clone();
      if (s) {
        s->setSizeForce(i->getSize() * getSize() / 2.0f);
        s->setPosition(i->getPosition() * getSize() / 2.0f + getPosition());
        if (s->isSelected())
          s->toggleSelection();
        arr.emplace_back(s);
      }
    }
  }
  return arr;
}