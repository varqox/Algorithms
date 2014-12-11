// Only for GCC
inline uint ceil2(uint x) { return 1u << (sizeof(uint)*8 - __builtin_clz(x-1)); }

// Only for GCC
inline uint floor2(uint x) { return 1u << (sizeof(uint)*8 - __builtin_clz(x) - 1); }
