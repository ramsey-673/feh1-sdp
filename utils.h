
#pragma once
/**
 * Game timer.
 * 
 * @author Andrew Loznianu
 */
class Timer
{
private:
    /**
     * The relative stop time of the timer.
     */
    double stopTime;
public:
    Timer(double duration);
    /**
     * Check if the timer is ongoing.
     * 
     * @returns true if the timer is ongoing
     * and false if it expired.
     */
    bool isTimeUp() const;
    /**
     * Calculates the time in seconds until the timer expires.
     * 
     * @returns time in seconds until timer expires
     */
    double getTimeLeft() const; // Get remaining time
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