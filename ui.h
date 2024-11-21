#pragma once



// Menu functions
int menu();
int stats();
int tutorial();
int credits();



class UIManager {
private:
    static void renderTimer(int timeLeft);
    static void renderScore(int score);
public:
    static void renderUI();
};

// Timer object
class Timer {
private:
    int startTime;
public:
    Timer(int duration);
    void update();  // Decrement the timer
    bool IsTimeUp() const; // Returns true if the time is up
    int getTimeLeft() const; // Get remaining time
};

// Handes user input
class InputHandler {
public:
    static void processInput();
};