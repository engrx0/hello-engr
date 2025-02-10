#include <ncurses.h>

void print_border(int start_row, int start_col, int height, int width, int delay);

void print_text(int start_row, int start_col, const char * const msg, const int delay)
{
    move(start_row, start_col);

    const char *m;
    m = msg;

    int i = 0;
    while(*m)
    {
        if (i++ % 2 == 0)
            addch(A_BOLD | A_REVERSE | *m);
        else  
            addch(*m);

        m++;
        refresh();
        napms(delay); // delay 100 ms
    }
}

struct window_t
{
    int xo;
    int yo;
    int width;
    int height;
};

int main()
{
    initscr();
    curs_set(0);
    // cbreak();
    // noecho();

    int main_height = 24;
    int main_width = 80;
    print_border(0, 0, main_height, main_width, 10);

    int message_yo = 2;
    int message_xo = 10;
    print_border(message_yo, message_xo, 4, 60, 10);
    
    int ascii_yo = 8;
    int ascii_xo = 5;
    print_border(ascii_yo, ascii_xo, 15, 70, 10);

    char text_hello[] = "hello engr";
    int hello_delay_ms = 200; 
    print_text(message_yo + 1 , message_xo + 1, text_hello, hello_delay_ms);


    getch();

    endwin(); // close ncurses

    return 0;
}

void print_border(int start_row, int start_col, int height, int width, int delay)
{
    int end_row = start_row + height;
    int end_col = start_col + width;

    // Draw top-left corner
    move(start_row, start_col);
    addch(ACS_ULCORNER);
    napms(delay);
    refresh();

    // Draw top horizontal line
    for (int col = start_col + 1; col < end_col; col++)
    {
        move(start_row, col);
        addch(ACS_HLINE);
        napms(delay);
        refresh();
    }

    // Draw top-right corner
    move(start_row, end_col);
    addch(ACS_URCORNER);
    napms(delay);
    refresh();

    // Draw right vertical line
    for (int row = start_row + 1; row < end_row; row++)
    {
        move(row, end_col);
        addch(ACS_VLINE);
        napms(delay);
        refresh();
    }

    // Draw bottom-right corner
    move(end_row, end_col);
    addch(ACS_LRCORNER);
    napms(delay);
    refresh();

    // Draw bottom horizontal line
    for (int col = end_col - 1; col > start_col; col--)
    {
        move(end_row, col);
        addch(ACS_HLINE);
        napms(delay);
        refresh();
    }

    // Draw bottom-left corner
    move(end_row, start_col);
    addch(ACS_LLCORNER);
    napms(delay);
    refresh();

    // Draw left vertical line
    for (int row = end_row - 1; row > start_row; row--)
    {
        move(row, start_col);
        addch(ACS_VLINE);
        napms(delay);
        refresh();
    }
}
