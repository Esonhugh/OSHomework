#ifdef DEBUG_MODE
#define DEBUG(a) printf("%s",a)
#define DEBUG_STRING(name, value) printf("[DEBUG]:%s: %s\n", name, value)
#define DEBUG_INT(name, value) printf("[DEBUG]: %s: %d\n", name, value)
#define MAX_LINE_LEN 1024
#else 
#define DEBUG(a)
#define DEBUG_STRING(name, value) 
#define DEBUG_INT(name, value)
#define MAX_LINE_LEN 1024
#endif