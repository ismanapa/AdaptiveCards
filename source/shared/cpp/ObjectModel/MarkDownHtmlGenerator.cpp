#include "MarkDownHtmlGenerator.h"

using namespace AdaptiveCards;

std::string MarkDownStringHtmlGenerator::GenerateHtmlString()
{
    if (m_isHead) 
    {              
        m_token = "<p>" + m_token;
    }

    if (m_isTail)
    {
        return m_token + "</p>";
    }

    return m_token;
}

//     left and right emphasis tokens are match if
//     1. they are same types
//     2. neither of the emphasis tokens are both left and right emphasis tokens, and 
//        if either or both of them are, then their sum is not multiple of 3 
bool MarkDownEmphasisHtmlGenerator::IsMatch(std::shared_ptr<MarkDownEmphasisHtmlGenerator> &emphasisToken)
{
    if (this->type == emphasisToken->type)
    {
        // rule #9 & #10, sum of delimiter count can't be multiple of 3 
        return !((this->IsLeftAndRightEmphasis() || emphasisToken->IsLeftAndRightEmphasis()) &&
            (((this->m_numberOfUnusedDelimiters+ emphasisToken->m_numberOfUnusedDelimiters) % 3) == 0));
    }
    return false;
}

bool MarkDownEmphasisHtmlGenerator::IsSameType(std::shared_ptr<MarkDownEmphasisHtmlGenerator> &token)
{
    return this->type == token->type;
}

// adjust number of emphasis counts after maching is done
int MarkDownEmphasisHtmlGenerator::AdjustEmphasisCounts(int leftOver, std::shared_ptr<MarkDownEmphasisHtmlGenerator> rightToken)
{
    int delimiterCount = 0;
    if (leftOver >= 0)
    {
        delimiterCount = this->m_numberOfUnusedDelimiters - leftOver;
        this->m_numberOfUnusedDelimiters = leftOver;
        rightToken->m_numberOfUnusedDelimiters = 0;
    }
    else
    {
        delimiterCount = this->m_numberOfUnusedDelimiters;
        rightToken->m_numberOfUnusedDelimiters = leftOver * (-1);
        this->m_numberOfUnusedDelimiters = 0;
    }
    return delimiterCount;
}

// generate bold and emphasis html tags
void MarkDownEmphasisHtmlGenerator::GenerateTags(std::shared_ptr<MarkDownEmphasisHtmlGenerator> &token)
{
    int delimiterCount = 0, leftOver = 0;
    leftOver = this->m_numberOfUnusedDelimiters - token->m_numberOfUnusedDelimiters;
    delimiterCount = this->AdjustEmphasisCounts(leftOver, token);

    // emphasis found
    if (delimiterCount % 2)
    {
        this->PushItalicTag();
        token->PushItalicTag();
    }

    // strong emphasis found
    for (int i = 0; i < delimiterCount / 2; i++)
    {
        this->PushBoldTag();
        token->PushBoldTag();
    }
}

void MarkDownEmphasisHtmlGenerator::PushItalicTag() 
{
    m_tags.push_back("<em>");
}

void MarkDownEmphasisHtmlGenerator::PushBoldTag() 
{
    m_tags.push_back("<strong>");
}

std::string MarkDownLeftEmphasisHtmlGenerator::GenerateHtmlString() 
{
    if (m_numberOfUnusedDelimiters)
    {
        int startIdx = m_token.size() - m_numberOfUnusedDelimiters;
        html << m_token.substr(startIdx, std::string::npos);
    }

    // append tags; since left delims, append it in the reverse order
    for (auto itr = m_tags.rbegin(); itr != m_tags.rend(); itr++)
    { 
        html << *itr;
    }

    if (m_isHead) 
    {   
        return "<p>" + html.str();
    }

    if (m_isTail)
    {
        return html.str() + "</p>";
    }

    return html.str();
}

void MarkDownRightEmphasisHtmlGenerator::PushItalicTag() 
{
    m_tags.push_back("</em>");
}

void MarkDownRightEmphasisHtmlGenerator::PushBoldTag() 
{
    m_tags.push_back("</strong>");
}

std::string MarkDownRightEmphasisHtmlGenerator::GenerateHtmlString() 
{
    // append tags; 
    for (auto itr = m_tags.begin(); itr != m_tags.end(); itr++)
    { 
        html << *itr;
    }

    // if there are unused emphasis, append them 
    if (m_numberOfUnusedDelimiters)
    {
        int startIdx = m_token.size() - m_numberOfUnusedDelimiters;
        html << m_token.substr(startIdx, std::string::npos);
    }

    if (m_isHead) 
    {
        return "<p>" + html.str();
    }

    if (m_isTail)
    {
        return html.str() + "</p>";
    }

    return html.str();
}

void MarkDownLeftAndRightEmphasisHtmlGenerator::PushItalicTag() 
{
    if (m_directionType == Left)
    {
        m_tags.push_back("<em>");
    }
    else
    {
        m_tags.push_back("</em>");
    }
}

void MarkDownLeftAndRightEmphasisHtmlGenerator::PushBoldTag() 
{
    if (m_directionType == Left)
    {
        m_tags.push_back("<strong>");
    }
    else
    {
        m_tags.push_back("</strong>");
    }
}

std::string MarkDownListHtmlGenerator::GenerateHtmlString()
{
    if (m_isHead) 
    {
        m_token = "<ul>" + m_token;
    }

    if (m_isTail)
    {
        return m_token + "</ul>";
    }

    return m_token;
}