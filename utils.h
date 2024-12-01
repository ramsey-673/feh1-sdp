#pragma once

#include <string>



/**
 * Game timer.
 * 
 * @author Andrew Loznianu
 */
class Timer
{
private:
    double stopTime;
    double pauseTime;

public:
    Timer(double duration);

    /*
     * Set the remaining time.
     */
    void SetTimer(double duration);

    /*
     * Number of seconds remaining.
     */
    int Remaining() const;

    /*
     * Number of minutes and seconds remaining on the timer
     * (for display purposes)
     */
    int Minutes() const;
    int Seconds() const;
    std::string Display() const;

    /*
     * Stop/start the timer.
     */
    void Pause();
    void Play();
};



// Simple (x, y) pair with overloaded math operators
class Vector
{
public:
    // Pair of coordinates or measurements
    float x;
    float y;

    /*
     * Operators for mathematically manipulating vectors.
     */
    Vector operator+(const Vector& a);
    Vector operator-(const Vector& a);
    Vector operator*(const int a);
    Vector operator/(const int a);

    /*
     * Assignment operators for vectors.
     */
    void operator+=(const Vector& a);
    void operator-=(const Vector& a);
    void operator*=(const int a);
    void operator/=(const int a);
};