# jstr
Minimalistic fully-validating Unicode-aware JSON parser

Pronounced *Jester*, a pun at [JSMN/Jasmin](https://github.com/zserge/jsmn),
the parser generates a read-only JSON DOM in a caller-provided buffer.

Parsing is destructive, i.e. the parser alters JSON string as it goes. This is
primarily used to terminate tokens with `\0` characters. DOM node stores the value
(a C-string) as a pointer into the JSON string. String escapes, ex: `\n`, `\uXXXX`,
are decoded in-place.

Incremental parsing is NOT supported. This decision allows to simplify the library's code
significantly. Who needs incremental parsing, seriously?

Unlike [JSMN/Jasmin](https://github.com/zserge/jsmn), the world's sloppiest JSON parser
(which touts itself as the world's fastest), Jester works according to the JSON spec.
Object keys are checked to be strings, every key must have a corresponding value,
random unquoted literals are not allowed and even inputs that aren't valid UTF-8 are rejected.

(How sloppy JSMN really is? Take a look at commented-out lines in their testsuite
labeled [FIXME](https://github.com/zserge/jsmn/blob/master/test/tests.c#L58).)

## Usage

```c
void jstr_init(jstr_parser_t *parser);
```

```c
ssize_t jstr_parse(
  jstr_parser_t *parser, char *json, jstr_token_t *token, size_t token_count
);
```

```c
typedef enum {
  JSTR_STRING = 0x01,
  JSTR_NUMBER = 0x02,
  JSTR_OBJECT = 0x04,
  JSTR_ARRAY  = 0x08,
  JSTR_TRUE   = 0x10,
  JSTR_FALSE  = 0x20,
  JSTR_NULL   = 0x40
} jstr_type_t;
```

```c
jstr_type_t jstr_type(const jstr_token_t *token);
```

```c
const char *jstr_value(const jstr_token_t *token);
```

```c
const jstr_token_t *jstr_next(const jstr_token_t *token);
```
