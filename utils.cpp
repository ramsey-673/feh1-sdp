#include "utils.h"
#include "FEHUtility.h"

Timer::Timer(double duration)
{
    this->stopTime = TimeNow() + duration;
}

bool Timer::isTimeUp() const
{
    return TimeNow() > this->stopTime;
}

double Timer::getTimeLeft() const
{
    return this->stopTime - TimeNow();
}

Vector Vector::operator+(const Vector& a)
{
    return { x + a.x, y + a.y };
}

Vector Vector::operator-(const Vector& a)
{
    return { x - a.x, y - a.y };
}

Vector Vector::operator*(const int a)
{
    return { x*a, y*a };
}

Vector Vector::operator/(const int a)
{
    return { x/a, y/a };
}

void Vector::operator+=(const Vector& a)
{
    x += a.x;
    y += a.y;
}

void Vector::operator-=(const Vector& a)
{
    x -= a.x;
    y -= a.y;
}

void Vector::operator*=(const int a)
{
    x *= a;
    y *= a;
}

void Vector::operator/=(const int a)
{
    x /= a;
    y /= a;
}