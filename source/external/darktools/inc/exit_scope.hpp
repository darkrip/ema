#ifndef __EXIT_SPOCE_HPP__
#define __EXIT_SPOCE_HPP__

namespace
{
template<typename F>
class ExitScope
{
public:
    ExitScope(const F& fn) : m_fn(fn) {;}
    ~ExitScope()
    {
        m_fn();
    }
private:
    F m_fn;
};

template<typename F>
ExitScope<F> CreateScopeExit(F fn)
{
    return ScopeExit<F>(fn);
}

}
#define EXIT_SCOPE_CREATE_UNIQ_NAME2(line) exit_scope_guard_##line
#define EXIT_SCOPE_CREATE_UNIQ_NAME(line) EXIT_SCOPE_CREATE_UNIQ_NAME2(line)
#define EXIT_SCOPE(F) const auto  &EXIT_SCOPE_CREATE_UNIQ_NAME(__LINE__) = CreateScopeExit([]{F;})





#endif//__EXIT_SPOCE_HPP__