#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
    JSTR_OBJECT = 0x01,
    JSTR_ARRAY  = 0x02,
    JSTR_STRING = 0x04,
    JSTR_NUMBER = 0x08,
    JSTR_TRUE   = 0x10,
    JSTR_FALSE  = 0x20,
    JSTR_NULL   = 0x40
} jstr_type_t;

#ifndef JSTR_TOKEN_COMPRESSED
#if __x86_64__
#define JSTR_TOKEN_COMPRESSED 1
#endif
#endif

typedef struct {
#if JSTR_TOKEN_COMPRESSED
    uintptr_t opaque;
#else
    jstr_type_t type;
    union {
        const char *value;
        size_t offset;
    };
#endif
} jstr_token_t;

static inline jstr_type_t jstr_type(const jstr_token_t *token) {
#if JSTR_TOKEN_COMPRESSED
    return 0xff & token->opaque;
#else
    return token->type;
#endif
}

static inline const char *jstr_value(const jstr_token_t *token) {
#if JSTR_TOKEN_COMPRESSED
    return (const char *)(token->opaque >> 8);
#else
    return token->value;
#endif
}

static inline size_t jstr__offset(const jstr_token_t *token) {
#if JSTR_TOKEN_COMPRESSED
    return token->opaque >> 8;
#else
    return token->offset;
#endif
}

static inline const jstr_token_t *jstr_next(const jstr_token_t *token) {
    return token + ((jstr_type(token)&(JSTR_OBJECT|JSTR_ARRAY)) ?
        jstr__offset(token) : 1);
}

typedef struct {
    size_t parse_pos;
    size_t cur_offset;
    size_t parent_offset;
} jstr_parser_t;

static inline void jstr_init(jstr_parser_t *parser) {
    parser->parse_pos = 0;
    parser->cur_offset = 0;
    parser->parent_offset = 0;
}

enum {
    JSTR_INVAL = -1, // parse error
    JSTR_NOMEM = -2, // token array too small
    JSTR_2BIG  = -3  // not enough bits in token to store pointer/offset
};

ssize_t jstr_parse(
    jstr_parser_t *parser, char *str,
    jstr_token_t *token, size_t token_count
);
