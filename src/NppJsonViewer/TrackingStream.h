#pragma once

#include "StringStream.h"

/// <summary>
/// Stream wrapper for tracking position during JSON parsing
/// Provides stream interface while maintaining position tracking
/// </summary>
template <typename Encoding = rapidjson::UTF8<>>
class TrackingStream
{
    using Ch = typename Encoding::Ch;
    
    StringStream m_ss;
    size_t       m_position = 0;

public:
    /// <summary>
    /// Initialize tracking stream with JSON string
    /// </summary>
    /// <param name="jsonString">JSON content to wrap</param>
    explicit TrackingStream(const std::string& jsonString) : m_ss(jsonString.c_str()) {}

    /// <summary>
    /// Get next character and advance position
    /// </summary>
    /// <returns>Next character in stream</returns>
    Ch Peek() const
    {
        return m_ss.Peek();
    }

    /// <summary>
    /// Take character at current position and advance
    /// </summary>
    /// <returns>Current character</returns>
    Ch Take()
    {
        m_position++;
        return m_ss.Take();
    }

    /// <summary>
    /// Get current position in stream
    /// </summary>
    /// <returns>Current byte offset</returns>
    size_t Tell() const
    {
        return m_position;
    }

    /// <summary>
    /// Put begin marker (for output stream)
    /// </summary>
    /// <returns>Pointer to put buffer</returns>
    Ch* PutBegin()
    {
        return m_ss.PutBegin();
    }

    /// <summary>
    /// Mark put end position (for output stream)
    /// </summary>
    /// <param name="pCh">Pointer to put buffer end</param>
    /// <returns>Number of characters put</returns>
    size_t PutEnd(Ch* pCh)
    {
        return m_ss.PutEnd(pCh);
    }

    /// <summary>
    /// Put single character (for output stream)
    /// </summary>
    /// <param name="ch">Character to put</param>
    void Put(Ch ch)
    {
        m_ss.Put(ch);
    }
};
