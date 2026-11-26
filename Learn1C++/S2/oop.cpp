#include <iostream>

using namespace std;

class Items {
public:
    size_t price = 0;
    string type;
};

class HockeyMan{
    public:
    string type = "HockeyMan";
    int number = 0;         
    string surname = "None";

    void WhoIAm() const{
        cout << "parent " << "I am " << type << " " << surname << " " << number << endl;
    }
    void Equipment(const Items& item) {
        cout << type << ": " << item.type<< " " << item.price << endl;
    }
};

void ClientCode(HockeyMan& hm){
        hm.WhoIAm();
    }

struct Klushka : public Items {
    Klushka() { 
        price = 20000; 
        type = "Klushka"; 
    }
};

struct Konki : public Items {
    Konki() { 
        price = 80000;
        type = "Konki"; 
    }
};

struct Shaiba : public Items {
    Shaiba() { 
        price = 1000;
        type = "Shaiba"; 
    }   
};

class HockeyPlayer : public HockeyMan{
public:
    HockeyPlayer() {
        type = "HockeyPlayer";
    }
    HockeyPlayer(int num, const string& str){
        number = num;
        surname = str;
        type = "HockeyPlayer";
    }
};

class Unior : public HockeyMan{
public:
    Unior() {
        type = "Unior";
    }
};


int main() {
    Klushka k;
    Shaiba l;
    Konki m;
    HockeyPlayer h;
    Unior t;

    h.Equipment(k);
    h.Equipment(l);
    h.Equipment(m);
    t.Equipment(k);
    t.Equipment(l);
    t.Equipment(m);
    HockeyPlayer h1(8, "Ovechkin");
    HockeyPlayer h2(17, "Ivanov");
    h2.Equipment(k);
    h2.WhoIAm();
    ClientCode(h1);
    ClientCode(h2);

    return 0;
}
