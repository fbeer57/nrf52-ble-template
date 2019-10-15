#include "stream.h"

void stream_init(stream_t* p_strm)
{
    nrf_ringbuf_init((nrf_ringbuf_t *)p_strm->p_context);
}

ret_code_t stream_consume(stream_t *p_strm, stream_processor_t consumer, uint16_t max_len)
{
    nrf_ringbuf_t *p_buf = (nrf_ringbuf_t *)p_strm->p_context;

    uint8_t *p_data;
    size_t len = max_len;
    ret_code_t error_code;

    error_code = nrf_ringbuf_get(p_buf, &p_data, &len, true);
    if (error_code == NRF_SUCCESS)
    {
        error_code = consumer(p_data, &len);
        error_code = nrf_ringbuf_free(p_buf, len);
    }
    return error_code;
}

ret_code_t stream_produce(stream_t *p_strm, stream_processor_t producer, uint16_t max_len)
{
    nrf_ringbuf_t *p_buf = (nrf_ringbuf_t *)p_strm->p_context;

    uint8_t *p_data;
    size_t len = max_len;
    ret_code_t error_code;

    error_code = nrf_ringbuf_alloc(p_buf, &p_data, &len, true);
    if (error_code == NRF_SUCCESS)
    {
        error_code = producer(p_data, &len);
        error_code = nrf_ringbuf_put(p_buf, len);
    }
    return error_code;
}

ret_code_t stream_put(stream_t* p_strm, const uint8_t *data, uint16_t *length)
{
    nrf_ringbuf_t *p_buf = (nrf_ringbuf_t *)p_strm->p_context;

    uint8_t *p_data;
    size_t len = *length;
    ret_code_t error_code;

    error_code = nrf_ringbuf_alloc(p_buf, &p_data, &len, true);
    if (error_code == NRF_SUCCESS && len > 0)
    {
        memcpy(p_data, data, len);
        nrf_ringbuf_put(p_buf, len);
        *length = len;
    }
    return error_code;
}

ret_code_t stream_get(stream_t* p_strm, uint8_t *data, uint16_t *length)
{
    nrf_ringbuf_t *p_buf = (nrf_ringbuf_t *)p_strm->p_context;

    uint8_t *p_data;
    size_t len = *length;
    ret_code_t error_code;

    error_code = nrf_ringbuf_get(p_buf, &p_data, &len, true);
    *length = len;

    if (error_code == NRF_SUCCESS && len > 0)
    {
        memcpy(data, p_data, len);
        nrf_ringbuf_free(p_buf, len);
    }
    return error_code;
}

uint16_t stream_capacity(const stream_t* p_strm)
{
    return ((nrf_ringbuf_t *)p_strm->p_context)->bufsize_mask + 1;
}

uint16_t stream_available(const stream_t* p_strm)
{
    nrf_ringbuf_cb_t *p_cb = ((nrf_ringbuf_t *)p_strm->p_context)->p_cb;
    return (p_cb->wr_idx - p_cb->tmp_rd_idx);
}

bool stream_full(const stream_t *p_strm)
{
    nrf_ringbuf_t *p = (nrf_ringbuf_t *)p_strm->p_context;
    return (p->p_cb->tmp_wr_idx - p->p_cb->rd_idx == p->bufsize_mask + 1);
}

bool stream_empty(const stream_t *p_strm)
{
    nrf_ringbuf_cb_t *p_cb = ((nrf_ringbuf_t *)p_strm->p_context)->p_cb;
    return (p_cb->tmp_wr_idx == p_cb->rd_idx);
}

ret_code_t stream_putc(stream_t *p_strm, char c)
{
    uint16_t len = 1;
    ret_code_t error_code = stream_put(p_strm, (uint8_t*)&c, &len);
    return (len != 1 && error_code == NRF_SUCCESS) ? NRF_ERROR_RESOURCES : error_code;
}

ret_code_t stream_getc(stream_t *p_strm, char *p_c)
{
    uint16_t len = 1;
    ret_code_t error_code = stream_get(p_strm, (uint8_t*)p_c, &len);
    return (len != 1 && error_code == NRF_SUCCESS) ? NRF_ERROR_RESOURCES : error_code;
}

ret_code_t stream_puts(stream_t *p_strm, const char *p_text)
{
    uint16_t slen = strlen(p_text);
    uint16_t len = slen;
    ret_code_t error_code = stream_put(p_strm, (uint8_t*)p_text, &len);
    return (len != slen && error_code == NRF_SUCCESS) ? NRF_ERROR_RESOURCES : error_code;
}

ret_code_t stream_gets(stream_t *p_strm, char *p_text, uint16_t maxlen)
{
    uint16_t slen = maxlen - 1;
    uint16_t len = slen;
    ret_code_t error_code = stream_get(p_strm, (uint8_t*)p_text, &len);
    if (error_code == NRF_SUCCESS)
    {
        ((uint8_t*)p_text)[slen] = 0;
    }
    return error_code;
}
