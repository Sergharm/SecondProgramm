#ifndef UI_H
#define UI_H

class UserInterface {
public:
    void run();
private:
    void show_menu();
    void process_text(bool is_encrypt);
    void process_file(bool is_encrypt);
    void generate_key();
};

#endif
