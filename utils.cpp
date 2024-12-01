#include "utils.h"

#include <cmath>
#include <string>
#include <time.h>



/* Timer */

Timer::Timer(double duration)
{
    stopTime = time(NULL) + duration;
}

void Timer::SetTimer(double duration)
{
    stopTime = time(NULL) + duration;
}

int Timer::Remaining() const
{
    return stopTime - time(NULL);
}

int Timer::Minutes() const
{
    return std::floor(Remaining() / 60);
}

int Timer::Seconds() const
{
    return Remaining() % 60;
}

std::string Timer::Display() const
{
    int seconds = Seconds();
    if (seconds < 10)
        return std::to_string(Minutes()) + ":0" + std::to_string(seconds);
    else
        return std::to_string(Minutes()) + ":" + std::to_string(seconds);
}

void Timer::Pause()
{
    pauseTime = time(NULL);
}

void Timer::Play()
{
    stopTime += time(NULL) - pauseTime;
}



/* Vector */

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