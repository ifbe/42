#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct window
{
        u64 buf;
        u64 fmt;
        u64 w;
        u64 h;
};




void icon_linux(struct window* win, int x, int y)
{
}
void icon_mac(struct window* win, int x, int y)
{
}
void icon_win(struct window* win, int x, int y)
{
}