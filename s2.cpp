#include <iostream>
using namespace std;

class Shape
{
public:
    virtual float area() = 0; // Pure virtual function
};

class Circle : public Shape
{
private:
    float radius;
public:
    Circle(float r)
    {
        radius = r;
    }
    float area()
    {
        return 3.14 * radius * radius;
    }
};

class Rectangle : public Shape
{
private:
    float length, breadth;
public:
    Rectangle(float l, float b)
    {
        length = l;
        breadth = b;
    }
    float area()
    {
        return length * breadth;
    }
};

class Triangle : public Shape
{
private:
    float base, height;
public:
    Triangle(float b, float h)
    {
        base = b;
        height = h;
    }
    float area()
    {
        return 0.5 * base * height;
    }
};

int main()
{
    float radius, length, breadth, base, height;

    // Input for Circle
    cout << "Enter radius of Circle: ";
    cin >> radius;

    // Input for Rectangle
    cout << "Enter length of Rectangle: ";
    cin >> length;
    cout << "Enter breadth of Rectangle: ";
    cin >> breadth;

    // Input for Triangle
    cout << "Enter base of Triangle: ";
    cin >> base;
    cout << "Enter height of Triangle: ";
    cin >> height;

    Circle c(radius);
    Rectangle r(length, breadth);
    Triangle t(base, height);

    Shape *ptr;

    cout << endl;

    ptr = &c;
    cout << "Area of Circle: " << ptr->area() << endl;

    ptr = &r;
    cout << "Area of Rectangle: " << ptr->area() << endl;

    ptr = &t;
    cout << "Area of Triangle: " << ptr->area() << endl;

    return 0;
}