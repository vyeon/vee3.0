#ifndef __VEE_COMM_H__
#define __VEE_COMM_H__

#include <vee/exception.h>

namespace vee {

class protocol_mismatch_exception: public vee::exception
{
public:
    using base_t = vee::exception;
    protocol_mismatch_exception():
        base_t{ "protocol mismatch exception" }
    {
    }
    explicit protocol_mismatch_exception(char const* const);
    virtual ~protocol_mismatch_exception() = default;
    virtual char const* to_string() const noexcept override;
};

class connection_failed_exception: public vee::exception
{
public:
    using base_t = vee::exception;
    connection_failed_exception():
        base_t{ "connection failed exception" }
    {
    }
    explicit connection_failed_exception(char const* const);
    virtual ~connection_failed_exception() = default;
    virtual char const* to_string() const noexcept override;
};

class connection_already_disconnected: public vee::exception
{
public:
    using base_t = vee::exception;
    connection_already_disconnected():
        base_t{ "connection already disconnected exception" }
    {
    }
    virtual ~connection_already_disconnected() = default;
    virtual char const* to_string() const noexcept override;
};

class accept_failed_exception: public vee::exception
{
public:
    using base_t = vee::exception;
    accept_failed_exception():
        base_t{ "accept failed exception" }
    {
    }
    virtual ~accept_failed_exception() = default;
    virtual char const* to_string() const noexcept override;
};

} // !namespace vee

#endif // !_VEE_COMM_H_