#ifndef SINGLEON_H
#define SINGLEON_H

#include <iostream>
#include <memory>
#include <mutex>

template <class T>
class Singleon
{
protected:
    Singleon() = default;
    Singleon(const Singleon<T>&) = delete;
    Singleon& operator = (const Singleon<T>&) = delete;

    static std::shared_ptr<T> instance_;
public:

    static std::shared_ptr<T> getIntance()
    {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]()
                       { instance_ = std::shared_ptr<T>(new T()); });
        return instance_;
    }

    ~Singleon()
    {
        std::cout << "this is singno destruct" << std::endl;
    }
};

#endif // SINGLEON_H

template <typename T>
std::shared_ptr<T> Singleon<T>::instance_ = nullptr;