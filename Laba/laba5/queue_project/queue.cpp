#include "queue.h"
#include <iostream>
#include <iomanip>

using namespace std;

void initQueue(vector<Window>& windows, int windowCount) {
    for (int i = 1; i <= windowCount; ++i) {
        Window window;
        window.number = i;
        window.totalTime = 0;
        windows.push_back(window);
    }
}

void enqueue(vector<Ticket>& queue, int& ticketCounter, int duration) {
    Ticket ticket;
    ticketCounter++;
    
    // Формируем номер талона с ведущими нулями
    ticket.id = "T";
    if (ticketCounter < 10) {
        ticket.id += "00" + to_string(ticketCounter);
    } else if (ticketCounter < 100) {
        ticket.id += "0" + to_string(ticketCounter);
    } else {
        ticket.id += to_string(ticketCounter);
    }
    
    ticket.duration = duration;
    queue.push_back(ticket);
    
    cout << ticket.id << endl;
}

void distribute(vector<Ticket>& queue, vector<Window>& windows) {
    // Распределяем каждый талон по окнам
    for (const Ticket& ticket : queue) {
        // Находим окно с минимальным временем
        int minWindowIndex = 0;
        int minTime = windows[0].totalTime;
        
        for (size_t i = 1; i < windows.size(); ++i) {
            if (windows[i].totalTime < minTime) {
                minTime = windows[i].totalTime;
                minWindowIndex = i;
            }
        }
        
        // Добавляем талон в выбранное окно
        windows[minWindowIndex].tickets.push_back(ticket.id);
        windows[minWindowIndex].totalTime += ticket.duration;
    }
    
    // Очищаем очередь
    queue.clear();
}

void showWindows(const vector<Window>& windows) {
    for (const Window& window : windows) {
        cout << "Окно " << window.number << " (" << window.totalTime << " минут): ";
        
        bool first = true;
        for (const string& ticket : window.tickets) {
            if (!first) {
                cout << ", ";
            }
            cout << ticket;
            first = false;
        }
        
        if (window.tickets.empty()) {
            cout << "-";
        }
        
        cout << endl;
    }
}