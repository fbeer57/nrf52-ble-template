#ifndef STREAM_H
#define STREAM_H 1

#include <stdint.h>
#include <stdbool.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_ringbuf.h"

typedef struct stream_s
{
    const void *p_context;
} stream_t;

typedef ret_code_t (*stream_processor_t)(uint8_t *data, size_t *length);

#define STREAM_DEF(_name,_size)                 \
    NRF_RINGBUF_DEF(CONCAT_2(_name,_rb),_size); \
    stream_t _name = {.p_context = &CONCAT_2(_name,_rb)}

extern void stream_init(stream_t *p_strm);

extern ret_code_t stream_consume(stream_t *p_strm, stream_processor_t consumer, uint16_t max_len);
extern ret_code_t stream_produce(stream_t *p_strm, stream_processor_t producer, uint16_t max_len);

extern ret_code_t stream_put(stream_t* p_strm, const uint8_t *data, uint16_t *length);
extern ret_code_t stream_get(stream_t* p_strm, uint8_t *data, uint16_t *length);

extern uint16_t stream_capacity(const stream_t* p_strm);
extern uint16_t stream_available(const stream_t* p_strm);

extern bool stream_full(const stream_t *p_strm);
extern bool stream_empty(const stream_t *p_strm);

extern ret_code_t stream_putc(stream_t *p_strm, char c);
extern ret_code_t stream_puts(stream_t *p_strm, const char *const p_text);

extern ret_code_t stream_getc(stream_t *p_strm, char *p_c);
extern ret_code_t stream_gets(stream_t *p_strm, char *p_text, uint16_t maxlen);

#endif
/*
** EOF
*/