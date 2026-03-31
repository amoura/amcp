#ifndef AMCP_BASE_HPP
#define AMCP_BASE_HPP

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

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
    assert(not is_err(r));
    return std::get<T>(r);
}

template <typename T, typename E>
E&
get_err(result<T,E>& r)
{
    assert(not is_ok(r));
    return std::get<E>(r);
}

template <typename T, typename E>
T&&
get_value(result<T,E>&& r)
{
    assert(not is_err(r));
    return std::get<T>(std::forward(r));
}

template <typename T, typename E>
E&&
get_err(result<T,E>&& r)
{
    assert(not is_ok(r));
    return std::get<E>(std::forward(r));
}

template <typename T, typename E>
const T&
get_value(const result<T,E>& r)
{
    assert(not is_err(r));
    return std::get<T>(r);
}

template <typename T, typename E>
const E&
get_err(const result<T,E>& r)
{
    assert(not is_ok(r));
    return std::get<E>(r);
}

template <typename T, typename E>
const T&&
get_value(const result<T,E>&& r)
{
    assert(not is_err(r));
    return std::get<T>(std::forward(r));
}

template <typename T, typename E>
const E&&
get_err(const result<T,E>&& r)
{
    assert(not is_ok(r));
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

    //auto e = am::get_err(ok);
    //std::println("got val: {}", am::get_value(ok));

    char msg[] = "wrong!";
    result err = ((char *)msg);
    assert(am::is_err(err));
    assert(not am::is_ok(err));
    assert(std::string_view(am::get_err(err)) ==
        std::string_view("wrong!"));

    //auto v = am::get_value(err);
    //std::println("got it: {}", v);
}

#endif // TESTING

#endif // AMCP_BASE_HPP
