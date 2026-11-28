#pragma once

class Sandbox;

class ConsoleUI {
public:
    explicit ConsoleUI(Sandbox& sandbox);
    void run();

private:
    Sandbox& sandbox_;
};
