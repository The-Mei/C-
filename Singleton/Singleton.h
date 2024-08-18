#pragma once

#ifdef __linux__
#include <pthread.h>
#else
#include <mutex>
#endif

#include <stdlib.h>

template<typename T>
class Singleton
{
public:
    static T *getInstance()
    {
#ifdef __linux__
        pthread_once(&mOnceFlag, init);
#else
        std::call_once(mOnceFlag, init);
#endif
        return mSelf;
    }

protected:
    Singleton() = default;

private:
    Singleton(const Singleton &) = delete;

    Singleton &operator=(const Singleton &) = delete;

    static void init()
    {
        mSelf = new T();
        atexit(destroy);
    }

    static void destroy()
    {
        if (mSelf)
        {
            delete mSelf;
            mSelf = nullptr;
        }
    }

private:
#ifdef __linux__
    static pthread_once_t mOnceFlag;
#else
    static std::once_flag mOnceFlag;
#endif
    static T *mSelf;
};

#ifdef __linux__
template<typename T>
pthread_once_t Singleton<T>::mOnceFlag = PTHREAD_ONCE_INIT;
#else
template<typename T>
std::once_flag Singleton<T>::mOnceFlag{};
#endif

template<typename T>
T *Singleton<T>::mSelf = nullptr;