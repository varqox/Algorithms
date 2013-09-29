// Only for GCC
inline unsigned long ceil_to_power2(unsigned long x)
{return (x&x-1 ? 1ul<<(sizeof(unsigned long)*8-__builtin_clzl(x)):x);}

// Only for GCC
inline unsigned long floor_to_power2(unsigned long x)
{return (x&x-1 ? 1ul<<(sizeof(unsigned long)*8-1-__builtin_clzl(x)):x);}