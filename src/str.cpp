///////////////////////////////////////////////
// str implementations

str::str(std::string_view view) :
    view(view)
{
}

str::str(const char *str) :
    view(str)
{
}

template <typename It>
str::str(It b, It e) :
    view(b,e)
{
}

inline std::string_view
str::get_view() const
{
    return view;
}

inline bool
str::operator==(const str& other) const
{
    return view.begin() == other.view.begin();
}

inline bool
str::operator!=(const str& other) const
{
    return view.begin() != other.view.begin();
}

inline bool
str::content_eq(const str& other) const
{
    return view == other.view;
}

inline bool
str::content_ne(const str& other) const
{
    return view != other.view;
}


/////////////////////////////////////////////////
// str_store implementation

str_store::str_store(size_t string_buffer_size) :
    string_buffer(string_buffer_size, 0),
    tail(string_buffer.begin())
{
}

str
str_store::intern(std::string_view view)
{
    auto it = views.find(view);
    if (it != views.end()) {
        // found the string, return the correponding view to our buffer
        return str(*it);
    }
    // the string is new, add it to the end of the buffer and return the view
    auto len = view.length();
    if (tail + len + 1 > string_buffer.end()) {
        std::println("error: str_store::intern: string buffer size exceeded.");
        exit(1);
    }
    std::copy(view.begin(), view.end(), tail);
    auto prev_tail = tail;
    tail += len;
    auto new_entry = std::string_view(prev_tail, tail);
    views.insert(new_entry);
    auto result = str(new_entry);
    assert(*tail == 0);
    tail++;
    assert(tail <= string_buffer.end());
    return result;
}

str
str_store::intern(char *cstr)
{
    return intern(std::string_view(cstr));
}

template <typename It>
str
str_store::intern(It b, It e)
{
    return intern(std::string_view(b, e));
}

std::optional<str>
str_store::find(std::string_view view) const
{
    auto it = views.find(view);
    if (it != views.end()) {
        return str(*it);
    }
    return {};
}

std::optional<str>
str_store::find(char *cstr) const
{
    return find(std::string_view(cstr));
}

template <typename It>
std::optional<str>
str_store::find(It b, It e) const
{
    return find(std::string_view(b, e));
}

inline size_t
str_store::count() const
{
    return views.size();
}


////////////////////////////////////////////////////////
// Tests

#ifdef TESTING

void test_str()
{
    char txt[] = "this is a test. a test.";
    auto s1 = str(std::string_view(txt+10, txt+14));
    auto s2 = str(std::string_view(txt, txt+4));
    auto s1r = str(std::string_view(txt+18, txt+22));
    auto s2r = str("this");

    assert(s1 != s1r);
    assert(s2 != s2r);
    assert(s1.content_eq(s1r));
    assert(s2.content_eq(s2r));
}

void test_str_store()
{
    auto st = str_store(1024*1024);
    char txt[] = "this is a test. a test.";
    auto s1 = st.intern(txt+10, txt+14);
    auto s2 = st.intern(txt, txt+4);
    auto s1r = st.intern(txt+18, txt+22);
    auto s2r = st.intern("this");

    assert(std::string_view("this") == std::string_view(txt, txt+4));

    assert(s1 != s2);
    assert(s1r != s2r);
    assert(s1 == s1r);
    assert(s2 == s2r);
    assert(s1.content_eq(s1r));
    assert(s2.content_eq(s2r));

    assert(!st.find("this ").has_value());
    auto s1rr = st.find("test");
    assert(s1rr.has_value());
    assert(s1rr.value() == s1);
}


#endif

