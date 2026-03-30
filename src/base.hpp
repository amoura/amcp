#ifndef AMCP_BASE_HPP
#define AMCP_BASE_HPP

namespace am {

template <typename T, typename E>
using result = std::variant<T,E>;

template <typename T, typename E>
inline bool
is_ok(const result<T,E>& r)
{
    return std::holds_alternative<T>(r);
}

template <typename T, typename E>
inline bool
is_err(const result<T,E>& r)
{
    return std::holds_alternative<E>(r);
}

template <typename T, typename E>
T&
get_value(result<T,E>& r)
{
    return std::get<T>(r);
}

template <typename T, typename E>
E&
get_err(result<T,E>& r)
{
    return std::get<E>(r);
}

template <typename T, typename E>
T&&
get_value(result<T,E>&& r)
{
    return std::get<T>(std::forward(r));
}

template <typename T, typename E>
E&&
get_err(result<T,E>&& r)
{
    return std::get<E>(std::forward(r));
}

template <typename T, typename E>
const T&
get_value(const result<T,E>& r)
{
    return std::get<T>(r);
}

template <typename T, typename E>
const E&
get_err(const result<T,E>& r)
{
    return std::get<E>(r);
}

template <typename T, typename E>
const T&&
get_value(const result<T,E>&& r)
{
    return std::get<T>(std::forward(r));
}

template <typename T, typename E>
const E&&
get_err(const result<T,E>&& r)
{
    return std::get<E>(std::forward(r));
}

} // namespace am


////////////////////////////////////////////////
// Tests

#ifdef TESTING

void test_result_type()
{
    using result = am::result<int,char*>;
    result ok = 12;
    assert(am::is_ok(ok));
    assert(not am::is_err(ok));
    assert(am::get_value(ok) == 12);

    char msg[] = "wrong!";
    result err = ((char *)msg);
    assert(am::is_err(err));
    assert(not am::is_ok(err));
    assert(std::string_view(am::get_err(err)) ==
        std::string_view("wrong!"));
}

#endif // TESTING

#endif // AMCP_BASE_HPP
