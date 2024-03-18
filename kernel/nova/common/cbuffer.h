////////////////////////////////////////////////////////////
//
// Nova OS
// Copyright (C) 2024 Simon Mårtensson
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#ifndef NOVA_CIRCULAR_BUFFER_H
#define NOVA_CIRCULAR_BUFFER_H

#include <nova/common/types.h>
#include <nova/common/macros.h>
#include <libc/string.h>


template<int N>
class CircularBuffer
{
public:
    CircularBuffer() :
        head_(0),
        full_(false)
    {
        clear();
    }


    void push(const char* data, u32 size)
    {
        for (u32 i = 0; i < size; i++)
        {
            buffer_[head_] = data[i];
            head_ = (head_ + 1) % N;
        }

        if (head_ + size >= N)
        {
            full_ = true;
        }
    }

    void read(char* data)
    {
        if (full_)
        {
            memcpy(data, buffer_ + head_, N - head_);
            memcpy(data + N - head_, buffer_, head_);
        }
        else
        {
            memcpy(data, buffer_, head_);
        }
    }

    void clear()
    {
        for (int i = 0; i < N; i++)
        {
            buffer_[i] = 0;
        }

        head_ = 0;
    }

    bool FORCE_INLINE isFull() const
    {
        return full_;
    }

    u32 size() const
    {
        if (full_)
        {
            return N;
        }
        else
        {
            return head_;
        }
    }

    constexpr u32 FORCE_INLINE capacity() const
    {
        return N;
    }

private:
    u8 buffer_[N];

    u32 head_;
    bool full_;
};


#endif // NOVA_CIRCULAR_BUFFER_H