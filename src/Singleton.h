#ifndef SINGLETON_H
#define SINGLETON_H
#include <new>

template<typename T>
class Singleton
{
public:
    static T& Instance()
    {
        if (!m_pInstance)
            m_pInstance = new(std::nothrow) T();
        return *m_pInstance;
    }
    
    static void Release()
    {
        if (m_pInstance)
            delete m_pInstance;
    }

    Singleton(const Singleton&)                  = delete;
    const Singleton& operator=(const Singleton&) = delete;
    
    Singleton(Singleton&&)            = delete;
    Singleton& operator=(Singleton&&) = delete;

protected: 
    Singleton() = default;
    virtual ~Singleton() = default;

    static T * m_pInstance;
};
#endif // SINGLETON_H