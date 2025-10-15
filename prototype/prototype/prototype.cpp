#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

class Shape {
public:
  virtual ~Shape() = default;

  virtual std::unique_ptr<Shape> clone() const = 0;

  virtual void draw() const = 0;
  virtual void setColor(const std::string& color) = 0;
  virtual std::string getType() const = 0;
};

class Circle : public Shape {
private:
  std::string color;
  double radius;

public:
  Circle(const std::string& color_, double radius_)
    : color(color_), radius(radius_) {
  }

  std::unique_ptr<Shape> clone() const override {
    return std::make_unique<Circle>(*this);
  }

  void draw() const override {
    std::cout << "Рисую " << color << " круг с радиусом " << radius << std::endl;
  }

  void setColor(const std::string& color_) override {
    color = color_;
  }

  std::string getType() const override {
    return "Circle";
  }

  void setRadius(double radius_) {
    radius = radius_;
  }
};

class Rectangle : public Shape {
private:
  std::string color;
  double width;
  double height;

public:
  Rectangle(const std::string& color_, double width_, double height_)
    : color(color_), width(width_), height(height_) {
  }

  std::unique_ptr<Shape> clone() const override {
    return std::make_unique<Rectangle>(*this);
  }

  void draw() const override {
    std::cout << "Рисую " << color << " прямоугольник "
      << width << "x" << height << std::endl;
  }

  void setColor(const std::string& color_) override {
    color = color_;
  }

  std::string getType() const override {
    return "Rectangle";
  }

  void setSize(double width_, double height_) {
    width = width_;
    height = height_;
  }
};

class ShapeManager {
private:
  std::unordered_map<std::string, std::unique_ptr<Shape>> prototypes;

public:
  ShapeManager() {
    prototypes["red_circle"] = std::make_unique<Circle>("красный", 10.0);
    prototypes["blue_rectangle"] = std::make_unique<Rectangle>("синий", 20.0, 15.0);
  }

  void addPrototype(const std::string& key_, std::unique_ptr<Shape> shape_) {
    prototypes[key_] = std::move(shape_);
  }

  std::unique_ptr<Shape> createShape(const std::string& key_) {
    auto it = prototypes.find(key_);
    if (it != prototypes.end()) {
      return it->second->clone();
    }
    return nullptr;
  }

  void listPrototypes() const {
    std::cout << "Доступные прототипы:" << std::endl;
    for (const auto& pair : prototypes) {
      std::cout << " - " << pair.first << " (" << pair.second->getType() << ")" << std::endl;
    }
  }
};

int main() {
  setlocale(LC_ALL, "Russian");

  std::cout << "=== Демонстрация паттерна Prototype ===" << std::endl << std::endl;

  std::cout << "1. Прямое клонирование объектов:" << std::endl;

  Circle originalCircle("красный", 5.0);
  std::cout << "Оригинал: ";
  originalCircle.draw();

  auto clonedCircle = originalCircle.clone();
  std::cout << "Клон: ";
  clonedCircle->draw();

  clonedCircle->setColor("синий");
  dynamic_cast<Circle*>(clonedCircle.get())->setRadius(8.0);
  std::cout << "Модифицированный клон: ";
  clonedCircle->draw();

  std::cout << "Оригинал (не изменился): ";
  originalCircle.draw();

  std::cout << std::endl;

  std::cout << "2. Использование менеджера прототипов:" << std::endl;

  ShapeManager manager;
  manager.listPrototypes();

  std::cout << std::endl << "Создание фигур из прототипов:" << std::endl;

  auto shape1 = manager.createShape("red_circle");
  auto shape2 = manager.createShape("blue_rectangle");
  auto shape3 = manager.createShape("green_triangle");

  if (shape1) {
    std::cout << "Создана: ";
    shape1->draw();
  }

  if (shape2) {
    std::cout << "Создана: ";
    shape2->draw();
  }

  if (shape3) {
    std::cout << "Создана: ";
    shape3->draw();
  }

  std::cout << std::endl << "Модификация созданных фигур:" << std::endl;
  if (shape1) {
    shape1->setColor("оранжевый");
    std::cout << "После модификации: ";
    shape1->draw();
  }

  std::cout << std::endl << "3. Добавление нового прототипа:" << std::endl;
  manager.addPrototype("big_yellow_circle",
    std::make_unique<Circle>("желтый", 25.0));

  auto bigCircle = manager.createShape("big_yellow_circle");
  if (bigCircle) {
    std::cout << "Создан новый прототип: ";
    bigCircle->draw();
  }

  return 0;
}