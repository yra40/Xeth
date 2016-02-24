#pragma once 

#include <leveldb/slice.h>


namespace Xeth{


template<class Key>
class KeySerializer
{
    public:
        leveldb::Slice operator()(const Key &) const;


};



}

#include "KeySerializer.ipp"
