bool
token_kind_has_value(token_kind kind)
{
    switch (kind) {
    case token_kind::Identifier:
    case token_kind::IntegerLiteral:
        return true;

    default:
        return false;
    }
}

const char *
error_str(lexer_error_kind kind)
{
    switch (kind) {
    case lexer_error_kind::IllegalCharacter:
        return "invalid character";
    case lexer_error_kind::UnknownToken:
        return "unknown token";
    case lexer_error_kind::IntegerLiteralOverflow:
        return "integer literal overflow";
    case lexer_error_kind::IllFormedIntegerLiteral:
        return "ill-formed integer literal";
    default:
        assert(false);
        return "unknown error";
    }
}

token::token(token_kind kind) :
    kind(kind)
{
    assert(not token_kind_has_value(kind));
}

token::token(token_kind kind, i64 val) :
    kind(kind),
    value(val)
{
    assert(kind == token_kind::IntegerLiteral);
}

token::token(token_kind kind, str s) :
    kind(kind),
    value(s)
{
    assert(kind == token_kind::Identifier);
}


text_pos::text_pos(size_t line, size_t col) :
    line(line),
    col(col)
{
}


lexer_error::lexer_error(lexer_error_kind kind, text_pos pos) :
    kind(kind),
    pos(pos)
{
}

std::string
lexer_error::message(const lexer& lex) const
{
    return std::format("{}:{}:{}: lexing error: {}\n",
        lex.get_src(), pos.line, pos.col, error_str(kind));
}



lexer::lexer(
    std::string_view text,
    std::string_view src,
    str_store *store
) :
    text(text),
    src(src),
    store(store),
    pos(text.begin()),
    beg(text.begin()),
    end(text.end())
{
    keywords[store->intern("int")] = token_kind::Int;
    keywords[store->intern("void")] = token_kind::Void;
    keywords[store->intern("return")] = token_kind::Return;
}

inline std::string_view
lexer::get_src() const
{
    return src;
}

inline bool
lexer::at_eof() const
{
    return pos == end;
}

inline bool
lexer::in_bounds() const
{
    return pos >= beg && pos <= end;
}

inline bool
lexer::in_strict_bounds() const
{
    return pos >= beg && pos < end;
}

inline char
lexer::curr_char() const
{
    assert(in_strict_bounds());
    return *pos;
}

inline void
lexer::adv()
{
    if (!at_eof()) {
        pos++;
    }
}

inline void
lexer::skip_whitespace()
{
    pos = std::find_if_not(pos, end, isspace);
}

text_pos
lexer::get_curr_text_pos() const
{
    u32 line = std::count(beg, pos, '\n') + 1;
    auto rit = std::make_reverse_iterator(pos);
    auto sline_it = std::find(rit, std::make_reverse_iterator(beg), '\n');
    u32 col = sline_it - rit;
    return text_pos(line, col);
}

bool
is_inner_word_char(char c)
{
    switch (c) {
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z': 
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': case '0':
        case '_': 
            return true;
            break;
    }
    return false;
}
 
lexer_result
lexer::next_token()
{
    skip_whitespace();
    if (at_eof()) {
        return {};
    }
    assert(in_strict_bounds());

    auto c = curr_char();
    switch (c) {
    case '(':
        adv();
        return token(token_kind::OpenParenthesis);

    case ')':
        adv();
        return token(token_kind::CloseParenthesis);

    case '{':
        adv();
        return token(token_kind::OpenBrace);

    case '}':
        adv();
        return token(token_kind::CloseBrace);

    case ';':
        adv();
        return token(token_kind::Semicolon);

        
    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': case '0': {
        i64 value = 0;
        while (in_strict_bounds() && isdigit(curr_char())) {
            i64 new_value = value*10 + i64(curr_char() - '0');
            if (new_value < value) {
                return lexer_error(lexer_error_kind::IntegerLiteralOverflow,
                    get_curr_text_pos());
            }
            value = new_value;
            adv();
        }
        if (in_strict_bounds() && is_inner_word_char(curr_char())) {
            return lexer_error(lexer_error_kind::IllFormedIntegerLiteral, 
                               get_curr_text_pos());
        } else {
            return token(token_kind::IntegerLiteral, value);
        }
    } break;

    case 'a': case 'b': case 'c': case 'd': case 'e':
    case 'f': case 'g': case 'h': case 'i': case 'j':
    case 'k': case 'l': case 'm': case 'n': case 'o':
    case 'p': case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x': case 'y':
    case 'z':
    case 'A': case 'B': case 'C': case 'D': case 'E':
    case 'F': case 'G': case 'H': case 'I': case 'J':
    case 'K': case 'L': case 'M': case 'N': case 'O':
    case 'P': case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X': case 'Y':
    case 'Z': 
    case '_': {
        auto idit = std::find_if_not(pos, end, is_inner_word_char);
        assert(idit > pos);
        auto id = store->intern(pos, idit);
        pos = idit;

        // fist check for keywords
        auto kwit = keywords.find(id);
        if (kwit != keywords.end()) {
            return token(kwit->second);
        }
        return token(token_kind::Identifier, id);
    }

    default:
        return lexer_error(lexer_error_kind::UnknownToken,
                           get_curr_text_pos());
    }
}


///////////////////////////////////////////////////////////
// Tests

#ifdef TESTING

#define exp_tok(tkind) \
    res_tok = lex.next_token();\
    assert(am::is_ok(res_tok));\
    maybe_tok = am::get_value(res_tok);\
    assert(maybe_tok.has_value());\
    tok = maybe_tok.value();\
    assert(tok.kind == tkind)

void test_lexer_1_basic()
{
    char text[] = R"(
int main(void) {
  return 42;
}
)";
    char src[] = "test_lexer_1_basic";
    auto st = str_store(1024*1024);
    auto lex = lexer(std::string_view(text),
                     std::string_view(src),
                     &st);

    auto res_tok = lex.next_token();
    assert(am::is_ok(res_tok));
    auto maybe_tok = am::get_value(res_tok);
    assert(maybe_tok.has_value());
    auto tok = maybe_tok.value();
    assert(tok.kind == token_kind::Int);

    res_tok = lex.next_token();
    assert(am::is_ok(res_tok));
    maybe_tok = am::get_value(res_tok);
    assert(maybe_tok.has_value());
    tok = maybe_tok.value();
    assert(tok.kind == token_kind::Identifier);
    assert(std::holds_alternative<str>(tok.value));
    assert(std::get<str>(tok.value) == st.intern("main"));
    auto pos = lex.get_curr_text_pos();
    //std::print("line: {}  col: {}", pos.line, pos.col);

    exp_tok(token_kind::OpenParenthesis);
    exp_tok(token_kind::Void);
    exp_tok(token_kind::CloseParenthesis);
    exp_tok(token_kind::OpenBrace);
    exp_tok(token_kind::Return);
    exp_tok(token_kind::IntegerLiteral);
    assert(std::holds_alternative<i64>(tok.value));
    assert(std::get<i64>(tok.value) == 42);
    exp_tok(token_kind::Semicolon);
    exp_tok(token_kind::CloseBrace);

    res_tok = lex.next_token();
    assert(am::is_ok(res_tok));
    maybe_tok = am::get_value(res_tok);
    assert(not maybe_tok.has_value());
}

#endif
