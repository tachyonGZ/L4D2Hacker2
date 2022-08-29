#pragma once

#define WIN32_LEAN_AND_MEAN

template<typename T>
class CSingleton
{
protected:
    CSingleton() {}
    ~CSingleton() {}

    CSingleton(const CSingleton&) = delete;
    CSingleton& operator=(const CSingleton&) = delete;

    CSingleton(CSingleton&&) = delete;
    CSingleton& operator=(CSingleton&&) = delete;

public:
    static T& GetInstance()
    {
        static T instance{ };
        return instance;
    }
};