#pragma once

#include <tuple>
#include <map>
#include <iostream>

template<typename T>
class Factory
{
public:
    template<typename ... Args>
    static T*  create(Args ... args)
    {
        auto    key = std::tuple<Args...>(args...);
        auto&   obj = get<decltype(key), T>(key);

        if(!obj)
        {
            obj = std::make_unique<T>(args...);
        }

        return obj.get();
    }

private:
    template<typename Key, typename Val>
    static std::unique_ptr<Val>&    get(Key const key)
    {
        static std::map<Key, std::unique_ptr<Val>>  storage;

        return  storage[key];
    }
};
