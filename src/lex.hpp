#ifndef AMCP_LEX_HPP
#define AMCP_LEX_HPP

enum class token_kind {
    OpenParenthesis,
    CloseParenthesis,
    OpenBrace,
    CloseBrace,
    Semicolon,

    Int,
    Void,
    Return,

    Identifier,
    IntegerLiteral,
};

struct token {
    token(token_kind kind);
    token(token_kind kind, i64 val);
    token(token_kind kind, str s);

    token_kind kind;
    std::variant<
        i64,
        str
    > value;
};

enum class lexer_error_kind {
    IllegalCharacter,
    UnknownToken,
    IntegerLiteralOverflow,
    IllFormedIntegerLiteral,
};

struct text_pos {
    text_pos(size_t line, size_t col);

    u32 line;
    u32 col;
};

class lexer;
struct lexer_error {
    lexer_error(lexer_error_kind kind, text_pos pos);
    std::string message(const lexer& lex) const;

    lexer_error_kind kind;
    text_pos pos;
};

using lexer_result = am::result<std::optional<token>, lexer_error>;

class lexer {
public:
    using iterator = std::string_view::const_iterator;

    lexer(
        std::string_view text,
        std::string_view src,
        str_store *store
    );

    std::string_view get_src() const;
    bool at_eof() const;
    text_pos get_curr_text_pos() const;
    lexer_result next_token();

private:
    bool in_bounds() const;
    bool in_strict_bounds() const;
    char curr_char() const;
    void adv();
    void skip_whitespace();

    std::string_view text;
    std::string_view src;
    std::unordered_map<str,token_kind> keywords;
    str_store *store;
    iterator pos;
    iterator beg;
    iterator end;
};



#endif // AMCP_LEX_HPP
