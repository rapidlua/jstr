#pragma once
#include <stddef.h>
#include <stdlib.h>

typedef enum {
    JSTR_STRING = 0x01,
    JSTR_NUMBER = 0x02,
    JSTR_OBJECT = 0x04,
    JSTR_ARRAY  = 0x08,
    JSTR_TRUE   = 0x10,
    JSTR_FALSE  = 0x20,
    JSTR_NULL   = 0x40
} jstr_type_t;

typedef struct {
    jstr_type_t type;
    union {
        const char *value;
        size_t offset;
    };
} jstr_token_t;

static inline jstr_type_t jstr_type(const jstr_token_t *token) {
    return token->type;
}

static inline const char *jstr_value(const jstr_token_t *token) {
    return token->value;
}

static inline size_t jstr__offset(const jstr_token_t *token) {
    return token->offset;
}

static inline const jstr_token_t *jstr_next(const jstr_token_t *token) {
    return token + ((jstr_type(token)&(JSTR_OBJECT|JSTR_ARRAY)) ?
        jstr__offset(token) : 1);
}

typedef struct {
    size_t parse_pos;
    size_t token_count;
    size_t parent_offset;
} jstr_parser_t;

static inline void jstr_init(jstr_parser_t *parser) {
    parser->parse_pos = 0;
    parser->token_count = 0;
    parser->parent_offset = -sizeof(jstr_token_t);
}

enum {
    JSTR_INVAL = -1,
    JSTR_NOMEM = -2
};

ssize_t jstr_parse(
    jstr_parser_t *parser, char *str,
    jstr_token_t *token, size_t token_count
);
