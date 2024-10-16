#pragma once

#include <string>
#include <memory>

#include "rapidjson/stream.h"


class TrackingStream
    :  public std::enable_shared_from_this<TrackingStream>
{
private:
    char m_chPrevChar {};    // Store previous character for handling column postion
    int  m_nLine {};
    int  m_nCol {};
    rapidjson::StringStream m_ss;

public:
    using Ch = char;    // Define Ch to conform to RapidJSON's expectations

    TrackingStream(const std::string &jsonText)
        : m_ss(jsonText.c_str())
        , m_nLine(1)
        , m_nCol(1)
        , m_chPrevChar('\0')
    {
    }

    std::shared_ptr<TrackingStream> GetShared()
    {
        return shared_from_this();
    }

    inline int getLine() const
    {
        return m_nLine;
    }

    inline int getCol() const
    {
        return m_nCol;
    }

    // Read the next character and update line/column numbers
    Ch Take()
    {
        Ch ch = m_ss.Take();
        if (ch == '\n')
        {
            ++m_nLine;
            m_nCol = 1;
        }
        else
        {
            ++m_nCol;
        }
        m_chPrevChar = ch;
        return ch;
    }

    Ch Peek() const
    {
        return m_ss.Peek();
    }

    size_t Tell() const
    {
        return m_ss.Tell();
    }

    Ch *PutBegin()
    {
        return m_ss.PutBegin();
    }

    size_t PutEnd(Ch *pCh)
    {
        return m_ss.PutEnd(pCh);
    }

    void Put(Ch ch)
    {
        m_ss.Put(ch);
    }

};

using SharedTrackingStream = std::shared_ptr<TrackingStream>;
