#include <iostream>

// 2D vector for Euclid's geometry
class Vector {
  public:
    float x;
    float y;

    Vector() : x{0}, y{0} {}
    Vector(float x) : x{x}, y{0} {}
    Vector(float x, float y) : x{x}, y{y} {}
    Vector(const Vector &other) {
        if (&other != this) {
            x = other.x;
            y = other.y;
        }
    }
    Vector &operator=(const Vector &other) {
        if (&other != this) {
            x = other.x;
            y = other.y;
        }

        return *this;
    }

    ~Vector() {}

  private:
};

class Shape {
  public:
    Vector position;

    Shape() {}

    void draw() {}

    ~Shape() {}

  private:
};

class Point : Shape {
  public:
  private:
};

class Segment : Shape {
  public:
  private:
};

class Ray : Shape {
  public:
  private:
};

class Line : Shape {
  public:
  private:
};

class Angle : Shape {};

class Triangle : Shape {
  public:
  private:
};

class Renderer {
  public:
    Renderer() {}
    Renderer(const Renderer &other) = delete;
    Renderer &operator=(const Renderer &other) = delete;

    void draw_axis() {}
    void draw_shape() {}
    void draw() {}

    ~Renderer() {}

  private:
};
