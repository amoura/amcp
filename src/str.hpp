#ifndef AMCP_STR_HPP
#define AMCP_STR_HPP

class str {
public:
    explicit str(std::string_view view);
    explicit str(const char *str);
    template <typename It>
    str(It b, It e);

    std::string_view get_view() const;
    bool operator==(const str& other) const;
    bool operator!=(const str& other) const;
    bool content_eq(const str& other) const;
    bool content_ne(const str& other) const;

private:
    std::string_view view;
};

class str_store {
public:
    str_store(size_t string_buffer_size);

    str intern(std::string_view view);
    str intern(char *cstr);
    template <typename It>
    str intern(It b, It e);

    std::optional<str> find(std::string_view view) const;
    std::optional<str> find(char *cstr) const;
    template <typename It>
    std::optional<str> find(It b, It e) const;
    size_t count() const;

private:
    std::vector<char> string_buffer;
    std::vector<char>::iterator tail;
    std::unordered_set<std::string_view> views;
};

#endif
