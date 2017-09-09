#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define XBEE_DEBUG
//#define USART_DEBUG
#endif

#ifdef XBEE_DEBUG
//#define debug_printf(fmt, ...) dprintf(XBEE_FILENO, "%s(%u):" fmt, __func__, __LINE__, __VA_ARGS__)
#define debug_printf(fmt, ...) xbee_printf("%s(%u): " fmt, __func__, __LINE__, ##__VA_ARGS__)
#elif USART_DEBUG
#define debug_printf(...) printf("%s(%u): " fmr, __func__, __LINE__, __VA_ARGS__)
#else
#define debug_printf(...)
#endif

#endif /* DEBUG_H */
