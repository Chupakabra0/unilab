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
Aggregator::Aggregator(std::vector< std::shared_ptr<Shape> > array)
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

const std::string Aggregator::getClassName() {
  return "Aggregator";
}

const std::string Aggregator::getClassNameVirtual() const {
  return getClassName();
}

const std::shared_ptr<Shape>
Aggregator::create(const std::vector< std::shared_ptr<Shape> >& array) {
  return std::make_shared<Aggregator>(array);
}

const std::shared_ptr<Shape> Aggregator::create() {
  auto arr = std::vector< std::shared_ptr<Shape> >();
  return std::make_shared<Aggregator>(arr);
}

const std::shared_ptr<Shape> Aggregator::cloneVirtual() {
  return std::make_shared<Aggregator>(*this);
}

void Aggregator::drawShape(const Cairo::RefPtr<Cairo::Context>& context,
                           bool selected, float alpha) {
  for (auto& i : array_) {
    if (i) {
      i->draw(context, selected, alpha);
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

std::ostream& Aggregator::outputVirtual(std::ostream& out) const {
  out << array_.size() << '\n';
  for (auto& i : array_) {
    out << *i;
  }
  return out;
}

std::istream& Aggregator::inputVirtual(std::istream& in) {
  array_.clear();
  size_t size;
  in >> size;
  array_.reserve(size);
  ShapesMap map = SHAPES_REGISTRY.getShapesMap();
  for (size_t i = 0; i < size; i++) {
    std::string str;
    in >> str;
    auto shape = map[str]();
    in >> (*shape);
    array_.emplace_back(shape);
  }
  return in;
}

const std::vector< std::shared_ptr<Shape> > Aggregator::deaggregate() {
  std::vector< std::shared_ptr<Shape> > arr;
  arr.reserve(array_.size());
  for (auto& i : array_) {
    if (i) {
      auto s = i->clone();
      if (s) {
        s->setSizeForce(i->getSize() * getSize() / 2.0f);
        auto p = i->getPosition() * getSize() / 2.0f + getPosition();
        s->setPosition(p);
        arr.emplace_back(s);
      }
    }
  }
  return arr;
}
