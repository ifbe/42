struct player
{
        u64 type;
        u64 name;
        u8 temp[0x30];

        u64 create;
        u64 delete;
        u64 start;
        u64 stop;
        u64 list;
        u64 choose;
        u64 read;
        u64 write;
};
struct window
{
        u64 buf;
        u64 fmt;
        u64 w;
        u64 h;
};
struct event
{
        u64 why;
        u64 what;
        u64 where;
        u64 when;
};

