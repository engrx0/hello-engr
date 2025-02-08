#include <ncurses.h>

void print_border(int height, int width, int delay)
{
    addch(ACS_ULCORNER);
    napms(delay);
    refresh();

    // draw top horizontal 
    for (int i = 0; i < width - 1; i++)
    {
        addch(ACS_HLINE);
        napms(delay);
        refresh();
    }

    addch(ACS_URCORNER);
    refresh();
    napms(delay);

    // draw right vertical 
    for (int i = 1; i < height; i++)
    {
        move(i, width);
        addch(ACS_VLINE);
        refresh();
        napms(delay);
    }
    
    move(height, width);
    addch(ACS_LRCORNER);
    refresh();
    napms(delay);

    // draw bottom horizontal 
    for (int i = width - 1; i > 0; i--)
    {
        move(height, i);
        addch(ACS_HLINE);
        refresh();
        napms(delay);
    }

    move(height, 0);
    addch(ACS_LLCORNER);
    refresh();
    napms(delay);

    // draw left vertical 
    for (int i = height - 1; i > 0; i--)
    {
        move(i, 0);
        addch(ACS_VLINE);
        refresh();
        napms(delay);
    }

}

int main()
{
    initscr();
    curs_set(0);
    // cbreak();
    // noecho();

    int main_height = 24;
    int main_width = 80;
    print_border(main_height, main_width, 10);

    getch();

    endwin(); // close ncurses

    return 0;
}