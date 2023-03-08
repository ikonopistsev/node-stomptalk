#pragma once

#include "stomptalk/stackbuf.hpp"
#include "stomptalk/hashval.hpp"
#include <iterator>

namespace stomptalk {

static inline bool ch_isupper(char ch) noexcept
{
    return ('A' <= ch) && (ch <= 'Z');
}

using pointer = const char*;
using hashval_t = hashval<char>;
using stackbuf_t = stackbuf<char, STOMPTALK_PARSER_STACK_SIZE>;

template<class Hook>
struct context
{
    Hook& hook_;
    hashval_t& hval_;
    stackbuf_t& sbuf_;
    pointer curr_;
};

template<class Hook>
struct basic_state
{
    Hook& hook_;
    hashval_t& hval_;
    stackbuf_t& sbuf_;
    pointer curr_;

    virtual std::size_t operator()(pointer curr, pointer end) noexcept = 0;
};


template<class Hook>
struct hdrline_done final
    : public basic_state<Hook>
{
    std::size_t operator()(pointer curr, pointer end) noexcept override
    {

    }
}


template<class Hook>
struct method_state final
    : public basic_state<Hook>
{
    std::size_t operator()(parser::pointer curr, parser::pointer end) noexcept override
    {
        do {
            auto ch = *curr++;

            if (ch_isupper(ch))
            {
                if (!sbuf_.push(ch))
                {
                    hook.set(stomptalk_error_too_big);
                    return curr;
                }
                
                hval_.push(ch);
            }
            else
            {
                if (ch == '\n')
                {
                    // определяем метод
                    // вызываем каллбек
                    hook.on_method(hval_.pop(), sbuf_.pop());

                    // переходим к поиску конца метода
                    state_ = &parser::hdrline_done;

                    return (curr < end) ?
                        hdrline_done(hook, curr, end) : curr;
                }
                else if (ch == '\r')
                {
                    // определяем метод
                    // вызываем каллбек
                    hook.on_method(hval_.pop(), sbuf_.pop());

                    // переходим к поиску конца метода
                    state_ = &parser::hdrline_almost_done;

                    return (curr < end) ?
                        hdrline_almost_done(hook, curr, end) : curr;
                }
                else
                {
                    hook.set(stomptalk_error_inval_method);
                    return curr;
                }
            }

        } while (curr < end);

        return curr;
    }
};

template<class Hook>
auto start_state_lmd(context<Hook>& ctx);

template<class Hook>
auto start_state_lmd(context<Hook>& ctx)
{
    return [&ctx](pointer curr, pointer end) -> void noexcept {
        do {
            auto ch = *curr++;
            // пропускаем символы до первого значимого
            if ((ch == '\r') || (ch == '\n') || (ch == '\0'))
                continue;

            if (!ch_isupper(ch))
            {
                hook.set(stomptalk_error_inval_reqline);
                ctx.curr = curr;
                return curr;
            }

            sbuf_.reset();
            hval_.reset();
            hook.reset();
            // передаем позицию в буфере
            hook.on_frame(curr - 1);

            // сохраняем стек
            sbuf_.push(ch);
            // инкрементируем хэш
            hval_.push(ch);

            // переходим к разбору метода
            state_ = &parser::method_state;

            if (curr < end)
            {
                method_state<Hook> m{hook_, hval_, sbuf_};
                return m(curr, end);
            }

            return curr;

        } while (curr < end);

        return start_state(curr, end);
    }
}

template<class Hook>
struct start_state final
    : public basic_state<Hook>
{
    std::size_t operator()(parser::pointer curr, parser::pointer end) noexcept override
    {
        do {
            auto ch = *curr++;
            // пропускаем символы до первого значимого
            if ((ch == '\r') || (ch == '\n') || (ch == '\0'))
                continue;

            if (!ch_isupper(ch))
            {
                hook.set(stomptalk_error_inval_reqline);
                return curr;
            }

            sbuf_.reset();
            hval_.reset();
            hook.reset();
            // передаем позицию в буфере
            hook.on_frame(curr - 1);

            // сохраняем стек
            sbuf_.push(ch);
            // инкрементируем хэш
            hval_.push(ch);

            // переходим к разбору метода
            state_ = &parser::method_state;

            if (curr < end)
            {
                method_state<Hook> m{hook_, hval_, sbuf_};
                return m(curr, end);
            }

            return curr;

        } while (curr < end);

        return start_state(curr, end);
    }
};

template<class Hook>
class p2
{
    Hook& hook_;

public:
    p2(Hook& hook) noexcept
        : hook_{hook}
    {   }

    std::size_t run(const char *ptr, std::size_t len) noexcept
    {
        hook.set(stomptalk_error_none);

        const char* curr = begin;
        const char* end = begin + len;

        while ((curr < end) && hook.ok())
        {
            [state_, curr] = state_(curr, end)
        }

        //while ((curr < end) && hook.ok())
        //    curr = (this->*state_)(hook, curr, end);

        return static_cast<std::size_t>(std::distance(begin, curr));

        return 0;
    }
};
    
} // namespace stomptalk