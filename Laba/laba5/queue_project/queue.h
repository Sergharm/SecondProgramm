#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <vector>
#include <map>

using namespace std;

struct Ticket {
    string id;
    int duration;
};

struct Window {
    int number;
    int totalTime;
    vector<string> tickets;
    
    Window() : number(0), totalTime(0) {}
};

void initQueue(vector<Window>& windows, int windowCount);
void enqueue(vector<Ticket>& queue, int& ticketCounter, int duration);
void distribute(vector<Ticket>& queue, vector<Window>& windows);
void showWindows(const vector<Window>& windows);

#endif