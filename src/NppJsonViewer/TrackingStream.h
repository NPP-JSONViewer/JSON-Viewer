#pragma once

#include <string>
#include <memory>

#include <rapidjson/stream.h>


class TrackingStream : public std::enable_shared_from_this<TrackingStream>
{
private:
    char                    m_chPrevChar {};    // Store previous character for handling column postion
    size_t                  m_nLine {};
    size_t                  m_nColumn {};
    rapidjson::StringStream m_ss;

public:
    using Ch = char;    // Define Ch to conform to RapidJSON's expectations

    TrackingStream(const std::string& jsonText)
        : m_ss(jsonText.c_str())
        , m_nLine(1)
        , m_nColumn(1)
        , m_chPrevChar('\0')
    {
    }

    std::shared_ptr<TrackingStream> GetShared()
    {
        return shared_from_this();
    }

    inline size_t getLine() const
    {
        return m_nLine;
    }

    inline size_t getColumn() const
    {
        return m_nColumn;
    }

    // Read the next character and update line/column numbers
    Ch Take()
    {
        Ch ch = m_ss.Take();
        if (ch == '\n')
        {
            ++m_nLine;
            m_nColumn = 1;
        }
        else
        {
            ++m_nColumn;
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

    Ch* PutBegin()
    {
        return m_ss.PutBegin();
    }

    size_t PutEnd(Ch* pCh)
    {
        return m_ss.PutEnd(pCh);
    }

    void Put(Ch ch)
    {
        m_ss.Put(ch);
    }
};

using TrackingStreamSharedPtr = std::shared_ptr<TrackingStream>;
