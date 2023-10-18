module xpath.xpath_parser;

import util;
import xpath.xpath_tokens;
import xpath.xpath_token_value_parsers;

// this file has been automatically generated from 'D:/work/soulng-project/sngxml/xpath/XPathParser.parser' using soulng parser generator spg version 3.0.0

using namespace XPathTokens;

std::unique_ptr<xpath::XPathExpr> XPathParser::Parse(XPathLexer& lexer)
{
    std::unique_ptr<xpath::XPathExpr> value;
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule(soulng::unicode::ToUtf32("parse"));
        lexer.Log()->IncIndent();
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    ++lexer;
    lexer::Span span = lexer.GetSpan();
    parser::Match match = XPathParser::Expr(lexer);
    value.reset(static_cast<xpath::XPathExpr*>(match.value));
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->DecIndent();
        lexer.Log()->WriteEndRule(soulng::unicode::ToUtf32("parse"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (match.hit)
    {
        if (*lexer == lexer::END_TOKEN)
        {
            return value;
        }
        else
        {
            lexer.ThrowExpectationFailure(lexer.GetSpan(), util::ToUtf32(lexer::GetEndTokenInfo()));
        }
    }
    else
    {
        lexer.ThrowExpectationFailure(span, U"Expr");
    }
    return value;
}

parser::Match XPathParser::Expr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Expr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> orExpr;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match = XPathParser::OrExpr(lexer);
        orExpr.reset(static_cast<xpath::XPathExpr*>(match.value));
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Expr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, orExpr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Expr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Expr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::OrExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("OrExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    std::unique_ptr<xpath::XPathExpr> left;
    std::unique_ptr<xpath::XPathExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::AndExpr(lexer);
                    left.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(left.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch4 = &match;
                {
                    parser::Match match(true);
                    parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch6 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch7 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            parser::Match match(false);
                                            if (*lexer == NAME)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                lexer::Token token = lexer.GetToken(pos);
                                                pass = lexer.GetKeywordToken(token.match) == OR;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = parser::Match(false);
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch9 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                parser::Match match = XPathParser::AndExpr(lexer);
                                                right.reset(static_cast<xpath::XPathExpr*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new xpath::XPathOrExpr(expr.release(), right.release()));
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("OrExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("OrExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("OrExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::AndExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("AndExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    std::unique_ptr<xpath::XPathExpr> left;
    std::unique_ptr<xpath::XPathExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::EqualityExpr(lexer);
                    left.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(left.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch4 = &match;
                {
                    parser::Match match(true);
                    parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch6 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch7 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            parser::Match match(false);
                                            if (*lexer == NAME)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                lexer::Token token = lexer.GetToken(pos);
                                                pass = lexer.GetKeywordToken(token.match) == AND;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = parser::Match(false);
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch9 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                parser::Match match = XPathParser::EqualityExpr(lexer);
                                                right.reset(static_cast<xpath::XPathExpr*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new xpath::XPathAndExpr(expr.release(), right.release()));
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AndExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AndExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("AndExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::EqualityExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("EqualityExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    xpath::Operator op = xpath::Operator();
    std::unique_ptr<xpath::XPathExpr> left;
    std::unique_ptr<xpath::XPathExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::RelationalExpr(lexer);
                    left.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(left.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch4 = &match;
                {
                    parser::Match match(true);
                    parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch6 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch7 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch8 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t save = lexer.GetPos();
                                                parser::Match match(false);
                                                parser::Match* parentMatch10 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    parser::Match match(false);
                                                    if (*lexer == EQ)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        op = xpath::Operator::equal;
                                                    }
                                                    *parentMatch10 = match;
                                                }
                                                *parentMatch9 = match;
                                                if (!match.hit)
                                                {
                                                    parser::Match match(false);
                                                    parser::Match* parentMatch11 = &match;
                                                    lexer.SetPos(save);
                                                    {
                                                        parser::Match match(false);
                                                        parser::Match* parentMatch12 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            parser::Match match(false);
                                                            if (*lexer == NEQ)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                op = xpath::Operator::notEqual;
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    *parentMatch9 = match;
                                                }
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch13 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch14 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                parser::Match match(true);
                                                parser::Match* parentMatch15 = &match;
                                                {
                                                    lexer::Span span = lexer.GetSpan();
                                                    parser::Match match = XPathParser::RelationalExpr(lexer);
                                                    right.reset(static_cast<xpath::XPathExpr*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch15 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.ThrowExpectationFailure(span, U"RelationalExpr");
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    switch (op)
                                                    {
                                                    case xpath::Operator::equal: expr.reset(new xpath::XPathEqualExpr(expr.release(), right.release()));
                                                        break;
                                                    case xpath::Operator::notEqual: expr.reset(new xpath::XPathNotEqualExpr(expr.release(), right.release()));
                                                        break;
                                                    }
                                                }
                                                *parentMatch14 = match;
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("EqualityExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("EqualityExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("EqualityExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::RelationalExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("RelationalExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    xpath::Operator op = xpath::Operator();
    std::unique_ptr<xpath::XPathExpr> left;
    std::unique_ptr<xpath::XPathExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::AdditiveExpr(lexer);
                    left.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(left.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch4 = &match;
                {
                    parser::Match match(true);
                    parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch6 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch7 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch8 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t save = lexer.GetPos();
                                                parser::Match match(false);
                                                parser::Match* parentMatch10 = &match;
                                                {
                                                    int64_t save = lexer.GetPos();
                                                    parser::Match match(false);
                                                    parser::Match* parentMatch11 = &match;
                                                    {
                                                        int64_t save = lexer.GetPos();
                                                        parser::Match match(false);
                                                        parser::Match* parentMatch12 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            parser::Match match(false);
                                                            if (*lexer == LEQ)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                op = xpath::Operator::lessOrEqual;
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                        if (!match.hit)
                                                        {
                                                            parser::Match match(false);
                                                            parser::Match* parentMatch13 = &match;
                                                            lexer.SetPos(save);
                                                            {
                                                                parser::Match match(false);
                                                                parser::Match* parentMatch14 = &match;
                                                                {
                                                                    int64_t pos = lexer.GetPos();
                                                                    parser::Match match(false);
                                                                    if (*lexer == GEQ)
                                                                    {
                                                                        ++lexer;
                                                                        match.hit = true;
                                                                    }
                                                                    if (match.hit)
                                                                    {
                                                                        op = xpath::Operator::greaterOrEqual;
                                                                    }
                                                                    *parentMatch14 = match;
                                                                }
                                                                *parentMatch13 = match;
                                                            }
                                                            *parentMatch11 = match;
                                                        }
                                                    }
                                                    *parentMatch10 = match;
                                                    if (!match.hit)
                                                    {
                                                        parser::Match match(false);
                                                        parser::Match* parentMatch15 = &match;
                                                        lexer.SetPos(save);
                                                        {
                                                            parser::Match match(false);
                                                            parser::Match* parentMatch16 = &match;
                                                            {
                                                                int64_t pos = lexer.GetPos();
                                                                parser::Match match(false);
                                                                if (*lexer == LESS)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    op = xpath::Operator::less;
                                                                }
                                                                *parentMatch16 = match;
                                                            }
                                                            *parentMatch15 = match;
                                                        }
                                                        *parentMatch10 = match;
                                                    }
                                                }
                                                *parentMatch9 = match;
                                                if (!match.hit)
                                                {
                                                    parser::Match match(false);
                                                    parser::Match* parentMatch17 = &match;
                                                    lexer.SetPos(save);
                                                    {
                                                        parser::Match match(false);
                                                        parser::Match* parentMatch18 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            parser::Match match(false);
                                                            if (*lexer == GREATER)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                op = xpath::Operator::greater;
                                                            }
                                                            *parentMatch18 = match;
                                                        }
                                                        *parentMatch17 = match;
                                                    }
                                                    *parentMatch9 = match;
                                                }
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch19 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch20 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                parser::Match match(true);
                                                parser::Match* parentMatch21 = &match;
                                                {
                                                    lexer::Span span = lexer.GetSpan();
                                                    parser::Match match = XPathParser::AdditiveExpr(lexer);
                                                    right.reset(static_cast<xpath::XPathExpr*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch21 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.ThrowExpectationFailure(span, U"AdditiveExpr");
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    switch (op)
                                                    {
                                                    case xpath::Operator::lessOrEqual: expr.reset(new xpath::XPathLessOrEqualExpr(expr.release(), right.release()));
                                                        break;
                                                    case xpath::Operator::greaterOrEqual: expr.reset(new xpath::XPathGreaterOrEqualExpr(expr.release(), right.release()));
                                                        break;
                                                    case xpath::Operator::less: expr.reset(new xpath::XPathLessExpr(expr.release(), right.release()));
                                                        break;
                                                    case xpath::Operator::greater: expr.reset(new xpath::XPathGreaterExpr(expr.release(), right.release()));
                                                        break;
                                                    }
                                                }
                                                *parentMatch20 = match;
                                            }
                                            *parentMatch19 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("RelationalExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("RelationalExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("RelationalExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::AdditiveExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("AdditiveExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    xpath::Operator op = xpath::Operator();
    std::unique_ptr<xpath::XPathExpr> left;
    std::unique_ptr<xpath::XPathExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::MultiplicativeExpr(lexer);
                    left.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(left.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch4 = &match;
                {
                    parser::Match match(true);
                    parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch6 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch7 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch8 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t save = lexer.GetPos();
                                                parser::Match match(false);
                                                parser::Match* parentMatch10 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    parser::Match match(false);
                                                    if (*lexer == PLUS)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        op = xpath::Operator::plus;
                                                    }
                                                    *parentMatch10 = match;
                                                }
                                                *parentMatch9 = match;
                                                if (!match.hit)
                                                {
                                                    parser::Match match(false);
                                                    parser::Match* parentMatch11 = &match;
                                                    lexer.SetPos(save);
                                                    {
                                                        parser::Match match(false);
                                                        parser::Match* parentMatch12 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            parser::Match match(false);
                                                            if (*lexer == MINUS)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                op = xpath::Operator::minus;
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    *parentMatch9 = match;
                                                }
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch13 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch14 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                parser::Match match(true);
                                                parser::Match* parentMatch15 = &match;
                                                {
                                                    lexer::Span span = lexer.GetSpan();
                                                    parser::Match match = XPathParser::MultiplicativeExpr(lexer);
                                                    right.reset(static_cast<xpath::XPathExpr*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch15 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.ThrowExpectationFailure(span, U"MultiplicativeExpr");
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    switch (op)
                                                    {
                                                    case xpath::Operator::plus: expr.reset(new xpath::XPathAddExpr(expr.release(), right.release()));
                                                        break;
                                                    case xpath::Operator::minus: expr.reset(new xpath::XPathSubExpr(expr.release(), right.release()));
                                                        break;
                                                    }
                                                }
                                                *parentMatch14 = match;
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AdditiveExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AdditiveExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("AdditiveExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::MultiplicativeExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("MultiplicativeExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    xpath::Operator op = xpath::Operator();
    std::unique_ptr<xpath::XPathExpr> left;
    std::unique_ptr<xpath::XPathExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::UnaryExpr(lexer);
                    left.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(left.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch4 = &match;
                {
                    parser::Match match(true);
                    parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch6 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch7 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch8 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t save = lexer.GetPos();
                                                parser::Match match(false);
                                                parser::Match* parentMatch10 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    parser::Match match(false);
                                                    if (*lexer == STAR)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        op = xpath::Operator::mul;
                                                    }
                                                    *parentMatch10 = match;
                                                }
                                                *parentMatch9 = match;
                                                if (!match.hit)
                                                {
                                                    parser::Match match(false);
                                                    parser::Match* parentMatch11 = &match;
                                                    lexer.SetPos(save);
                                                    {
                                                        parser::Match match(false);
                                                        parser::Match* parentMatch12 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            bool pass = true;
                                                            parser::Match match(false);
                                                            if (*lexer == NAME)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                lexer::Token token = lexer.GetToken(pos);
                                                                switch (lexer.GetKeywordToken(token.match))
                                                                {
                                                                case DIV: op = xpath::Operator::div;
                                                                    break;
                                                                case MOD: op = xpath::Operator::mod;
                                                                    break;
                                                                default: pass = false;
                                                                    break;
                                                                }
                                                            }
                                                            if (match.hit && !pass)
                                                            {
                                                                match = parser::Match(false);
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    *parentMatch9 = match;
                                                }
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch13 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch14 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                parser::Match match(true);
                                                parser::Match* parentMatch15 = &match;
                                                {
                                                    lexer::Span span = lexer.GetSpan();
                                                    parser::Match match = XPathParser::UnaryExpr(lexer);
                                                    right.reset(static_cast<xpath::XPathExpr*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch15 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.ThrowExpectationFailure(span, U"UnaryExpr");
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    switch (op)
                                                    {
                                                    case xpath::Operator::mul: expr.reset(new xpath::XPathMulExpr(expr.release(), right.release()));
                                                        break;
                                                    case xpath::Operator::div: expr.reset(new xpath::XPathDivExpr(expr.release(), right.release()));
                                                        break;
                                                    case xpath::Operator::mod: expr.reset(new xpath::XPathModExpr(expr.release(), right.release()));
                                                        break;
                                                    }
                                                }
                                                *parentMatch14 = match;
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("MultiplicativeExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("MultiplicativeExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("MultiplicativeExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::UnaryExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("UnaryExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> subject;
    std::unique_ptr<xpath::XPathExpr> unionExpr;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            if (*lexer == MINUS)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::UnaryExpr(lexer);
                    subject.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("UnaryExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, new xpath::XPathUnaryMinusExpr(subject.release()));
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch4 = &match;
            lexer.SetPos(save);
            {
                parser::Match match(false);
                parser::Match* parentMatch5 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::UnionExpr(lexer);
                    unionExpr.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("UnaryExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, unionExpr.release());
                        }
                    }
                    *parentMatch5 = match;
                }
                *parentMatch4 = match;
            }
            *parentMatch0 = match;
        }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("UnaryExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("UnaryExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::UnionExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("UnionExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    std::unique_ptr<xpath::XPathExpr> left;
    std::unique_ptr<xpath::XPathExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::PathExpr(lexer);
                    left.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(left.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch4 = &match;
                {
                    parser::Match match(true);
                    parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch6 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch7 = &match;
                                    {
                                        parser::Match match(false);
                                        if (*lexer == UNION)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch8 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                parser::Match match = XPathParser::PathExpr(lexer);
                                                right.reset(static_cast<xpath::XPathExpr*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new xpath::XPathUnionExpr(expr.release(), right.release()));
                                                }
                                                *parentMatch9 = match;
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("UnionExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("UnionExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("UnionExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::PathExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("PathExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    xpath::Operator op = xpath::Operator();
    std::unique_ptr<xpath::XPathExpr> locationPath;
    std::unique_ptr<xpath::XPathExpr> functionCall;
    std::unique_ptr<xpath::XPathExpr> filterExpr;
    std::unique_ptr<xpath::XPathExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                int64_t save = lexer.GetPos();
                {
                    parser::Match match = XPathParser::LocationPath(lexer);
                    locationPath.reset(static_cast<xpath::XPathExpr*>(match.value));
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch4 = &match;
                    {
                        int64_t tmp = lexer.GetPos();
                        lexer.SetPos(save);
                        save = tmp;
                        parser::Match match = XPathParser::FunctionCall(lexer);
                        functionCall.reset(static_cast<xpath::XPathExpr*>(match.value));
                        *parentMatch4 = match;
                    }
                    if (!match.hit)
                    {
                        lexer.SetPos(save);
                    }
                    *parentMatch3 = parser::Match(!match.hit, match.value);
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PathExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, locationPath.release());
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch5 = &match;
            lexer.SetPos(save);
            {
                parser::Match match(false);
                parser::Match* parentMatch6 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match(false);
                    parser::Match* parentMatch7 = &match;
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch8 = &match;
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch9 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                parser::Match match = XPathParser::FilterExpr(lexer);
                                filterExpr.reset(static_cast<xpath::XPathExpr*>(match.value));
                                if (match.hit)
                                {
                                    expr.reset(filterExpr.release());
                                }
                                *parentMatch9 = match;
                            }
                            *parentMatch8 = match;
                        }
                        if (match.hit)
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch10 = &match;
                            {
                                parser::Match match(true);
                                int64_t save = lexer.GetPos();
                                parser::Match* parentMatch11 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch12 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch13 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch14 = &match;
                                            {
                                                parser::Match match(false);
                                                parser::Match* parentMatch15 = &match;
                                                {
                                                    int64_t save = lexer.GetPos();
                                                    parser::Match match(false);
                                                    parser::Match* parentMatch16 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        parser::Match match(false);
                                                        if (*lexer == SLASHSLASH)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            op = xpath::Operator::slashSlash;
                                                        }
                                                        *parentMatch16 = match;
                                                    }
                                                    *parentMatch15 = match;
                                                    if (!match.hit)
                                                    {
                                                        parser::Match match(false);
                                                        parser::Match* parentMatch17 = &match;
                                                        lexer.SetPos(save);
                                                        {
                                                            parser::Match match(false);
                                                            parser::Match* parentMatch18 = &match;
                                                            {
                                                                int64_t pos = lexer.GetPos();
                                                                parser::Match match(false);
                                                                if (*lexer == SLASH)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    op = xpath::Operator::slash;
                                                                }
                                                                *parentMatch18 = match;
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        *parentMatch15 = match;
                                                    }
                                                }
                                                *parentMatch14 = match;
                                            }
                                            *parentMatch13 = match;
                                        }
                                        if (match.hit)
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch19 = &match;
                                            {
                                                parser::Match match(false);
                                                parser::Match* parentMatch20 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    parser::Match match = XPathParser::RelativeLocationPath(lexer);
                                                    right.reset(static_cast<xpath::XPathExpr*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        switch (op)
                                                        {
                                                        case xpath::Operator::slashSlash: {
                                                            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PathExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                                                                return parser::Match(true, new xpath::XPathCombineStepExpr(expr.release(), new xpath::XPathCombineStepExpr(new xpath::XPathLocationStepExpr(xml::Axis::descendantOrSelf, new xpath::XPathAnyNodeTest), right.release())));
                                                            }
                                                        }
                                                        case xpath::Operator::slash: {
                                                            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PathExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                                                                return parser::Match(true, new xpath::XPathCombineStepExpr(expr.release(), right.release()));
                                                            }
                                                        }
                                                        }
                                                    }
                                                    *parentMatch20 = match;
                                                }
                                                *parentMatch19 = match;
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch12 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch11 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                    }
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PathExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, expr.release());
                        }
                    }
                    *parentMatch6 = match;
                }
                *parentMatch5 = match;
            }
            *parentMatch0 = match;
        }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PathExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("PathExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::FilterExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("FilterExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    std::unique_ptr<xpath::XPathExpr> primaryExpr;
    std::unique_ptr<xpath::XPathExpr> predicate;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::PrimaryExpr(lexer);
                    primaryExpr.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(primaryExpr.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch4 = &match;
                {
                    parser::Match match(true);
                    parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch6 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch7 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        parser::Match match = XPathParser::Predicate(lexer);
                                        predicate.reset(static_cast<xpath::XPathExpr*>(match.value));
                                        if (match.hit)
                                        {
                                            expr.reset(new xpath::XPathFilterExpr(expr.release(), predicate.release()));
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("FilterExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("FilterExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("FilterExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::LocationPath(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("LocationPath"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> absoluteLocationPath;
    std::unique_ptr<xpath::XPathExpr> relativeLocationPath;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            parser::Match match = XPathParser::AbsoluteLocationPath(lexer);
            absoluteLocationPath.reset(static_cast<xpath::XPathExpr*>(match.value));
            if (match.hit)
            {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("LocationPath"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, absoluteLocationPath.release());
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            lexer.SetPos(save);
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::RelativeLocationPath(lexer);
                    relativeLocationPath.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("LocationPath"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, relativeLocationPath.release());
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch0 = match;
        }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("LocationPath"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("LocationPath"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::AbsoluteLocationPath(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("AbsoluteLocationPath"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    std::unique_ptr<xpath::XPathExpr> abbreviatedAbsoluteLocationPath;
    std::unique_ptr<xpath::XPathExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                int64_t save = lexer.GetPos();
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::AbbreviatedAbsoluteLocationPath(lexer);
                    abbreviatedAbsoluteLocationPath.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(abbreviatedAbsoluteLocationPath.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch4 = &match;
                    lexer.SetPos(save);
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch5 = &match;
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch6 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                parser::Match match(false);
                                if (*lexer == SLASH)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    expr.reset(new xpath::XPathRootNodeExpr);
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch7 = &match;
                            {
                                parser::Match match(true);
                                int64_t save = lexer.GetPos();
                                parser::Match* parentMatch8 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch9 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch10 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            parser::Match match = XPathParser::RelativeLocationPath(lexer);
                                            right.reset(static_cast<xpath::XPathExpr*>(match.value));
                                            if (match.hit)
                                            {
                                                expr.reset(new xpath::XPathCombineStepExpr(expr.release(), right.release()));
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch8 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                    }
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AbsoluteLocationPath"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AbsoluteLocationPath"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("AbsoluteLocationPath"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::AbbreviatedAbsoluteLocationPath(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("AbbreviatedAbsoluteLocationPath"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        parser::Match match(false);
        if (*lexer == SLASHSLASH)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                parser::Match match = XPathParser::RelativeLocationPath(lexer);
                right.reset(static_cast<xpath::XPathExpr*>(match.value));
                if (match.hit)
                {
                    {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AbbreviatedAbsoluteLocationPath"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                        return parser::Match(true, new xpath::XPathCombineStepExpr(new xpath::XPathRootNodeExpr, new xpath::XPathCombineStepExpr(new xpath::XPathLocationStepExpr(xml::Axis::descendantOrSelf, new xpath::XPathAnyNodeTest), right.release())));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AbbreviatedAbsoluteLocationPath"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("AbbreviatedAbsoluteLocationPath"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::RelativeLocationPath(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("RelativeLocationPath"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr = std::unique_ptr<xpath::XPathExpr>();
    xpath::Operator op = xpath::Operator();
    std::unique_ptr<xpath::XPathLocationStepExpr> left;
    std::unique_ptr<xpath::XPathLocationStepExpr> right;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::Step(lexer);
                    left.reset(static_cast<xpath::XPathLocationStepExpr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(left.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch4 = &match;
                {
                    parser::Match match(true);
                    parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch6 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch7 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch8 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t save = lexer.GetPos();
                                                parser::Match match(false);
                                                parser::Match* parentMatch10 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    parser::Match match(false);
                                                    if (*lexer == SLASHSLASH)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        op = xpath::Operator::slashSlash;
                                                    }
                                                    *parentMatch10 = match;
                                                }
                                                *parentMatch9 = match;
                                                if (!match.hit)
                                                {
                                                    parser::Match match(false);
                                                    parser::Match* parentMatch11 = &match;
                                                    lexer.SetPos(save);
                                                    {
                                                        parser::Match match(false);
                                                        parser::Match* parentMatch12 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            parser::Match match(false);
                                                            if (*lexer == SLASH)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                op = xpath::Operator::slash;
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    *parentMatch9 = match;
                                                }
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch13 = &match;
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch14 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                parser::Match match = XPathParser::Step(lexer);
                                                right.reset(static_cast<xpath::XPathLocationStepExpr*>(match.value));
                                                if (match.hit)
                                                {
                                                    switch (op)
                                                    {
                                                    case xpath::Operator::slashSlash: {
                                                        expr.reset(new xpath::XPathCombineStepExpr(expr.release(), new xpath::XPathCombineStepExpr(new xpath::XPathLocationStepExpr(xml::Axis::descendantOrSelf, new xpath::XPathAnyNodeTest), right.release())));
                                                        break;
                                                    }
                                                    case xpath::Operator::slash: {
                                                        expr.reset(new xpath::XPathCombineStepExpr(expr.release(), right.release()));
                                                        break;
                                                    }
                                                    }
                                                }
                                                *parentMatch14 = match;
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("RelativeLocationPath"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("RelativeLocationPath"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("RelativeLocationPath"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::Step(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Step"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathLocationStepExpr> expr = std::unique_ptr<xpath::XPathLocationStepExpr>();
    std::unique_ptr<parser::Value<xml::Axis>> axis;
    std::unique_ptr<xpath::XPathNodeTestExpr> nodeTest;
    std::unique_ptr<xpath::XPathExpr> predicate;
    std::unique_ptr<xpath::XPathLocationStepExpr> abbreviatedStep;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                int64_t save = lexer.GetPos();
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    parser::Match match(false);
                    parser::Match* parentMatch4 = &match;
                    {
                        parser::Match match = XPathParser::AxisSpecifier(lexer);
                        axis.reset(static_cast<parser::Value<xml::Axis>*>(match.value));
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch5 = &match;
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch6 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                parser::Match match = XPathParser::NodeTest(lexer);
                                nodeTest.reset(static_cast<xpath::XPathNodeTestExpr*>(match.value));
                                if (match.hit)
                                {
                                    expr.reset(new xpath::XPathLocationStepExpr(axis->value, nodeTest.release()));
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch7 = &match;
                    {
                        parser::Match match(true);
                        parser::Match* parentMatch8 = &match;
                        {
                            while (true)
                            {
                                int64_t save = lexer.GetPos();
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch9 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch10 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            parser::Match match = XPathParser::Predicate(lexer);
                                            predicate.reset(static_cast<xpath::XPathExpr*>(match.value));
                                            if (match.hit)
                                            {
                                                expr->AddPredicate(predicate.release());
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch8 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                        break;
                                    }
                                }
                            }
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch11 = &match;
                    lexer.SetPos(save);
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch12 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            parser::Match match = XPathParser::AbbreviatedStep(lexer);
                            abbreviatedStep.reset(static_cast<xpath::XPathLocationStepExpr*>(match.value));
                            if (match.hit)
                            {
                                expr.reset(abbreviatedStep.release());
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch11 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Step"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Step"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Step"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::AxisSpecifier(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("AxisSpecifier"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<parser::Value<xml::Axis>> axis;
    std::unique_ptr<parser::Value<xml::Axis>> abbreviatedAxisSpecifier;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match = XPathParser::AxisName(lexer);
            axis.reset(static_cast<parser::Value<xml::Axis>*>(match.value));
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match(false);
                    if (*lexer == COLONCOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisSpecifier"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, new parser::Value<xml::Axis>(axis->value));
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch4 = &match;
            lexer.SetPos(save);
            {
                parser::Match match(false);
                parser::Match* parentMatch5 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::AbbreviatedAxisSpecifier(lexer);
                    abbreviatedAxisSpecifier.reset(static_cast<parser::Value<xml::Axis>*>(match.value));
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisSpecifier"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, new parser::Value<xml::Axis>(abbreviatedAxisSpecifier->value));
                        }
                    }
                    *parentMatch5 = match;
                }
                *parentMatch4 = match;
            }
            *parentMatch0 = match;
        }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisSpecifier"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("AxisSpecifier"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::AxisName(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("AxisName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        bool pass = true;
        parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            lexer::Token token = lexer.GetToken(pos);
            switch (lexer.GetKeywordToken(token.match))
            {
            case ANCESTOR: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::ancestor));
                }
            }
            case ANCESTOR_OR_SELF: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::ancestorOrSelf));
                }
            }
            case ATTRIBUTE: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::attribute));
                }
            }
            case CHILD: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::child));
                }
            }
            case DESCENDANT: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::descendant));
                }
            }
            case DESCENDANT_OR_SELF: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::descendantOrSelf));
                }
            }
            case FOLLOWING: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::following));
                }
            }
            case FOLLOWING_SIBLING: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::followingSibling));
                }
            }
            case NAMESPACE: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::ns));
                }
            }
            case PARENT: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::parent));
                }
            }
            case PRECEDING: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::preceding));
                }
            }
            case PRECEDING_SIBLING: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::precedingSibling));
                }
            }
            case SELF: {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::self));
                }
            }
            default: {
                pass = false;
                break;
            }
            }
        }
        if (match.hit && !pass)
        {
            match = parser::Match(false);
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AxisName"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("AxisName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::AbbreviatedAxisSpecifier(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("AbbreviatedAxisSpecifier"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            parser::Match match(false);
            if (*lexer == AT)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AbbreviatedAxisSpecifier"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::attribute));
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            lexer.SetPos(save);
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match(true);
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AbbreviatedAxisSpecifier"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, new parser::Value<xml::Axis>(xml::Axis::child));
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch0 = match;
        }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AbbreviatedAxisSpecifier"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("AbbreviatedAxisSpecifier"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::NodeTest(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("NodeTest"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> pi;
    std::unique_ptr<xpath::XPathNodeTestExpr> nodeType;
    std::unique_ptr<xpath::XPathNodeTestExpr> nameTest;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    parser::Match match(false);
                    parser::Match* parentMatch4 = &match;
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            bool pass = true;
                            parser::Match match(false);
                            if (*lexer == NAME)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                lexer::Token token = lexer.GetToken(pos);
                                pass = lexer.GetKeywordToken(token.match) == PROCESSING_INSTRUCTION;
                            }
                            if (match.hit && !pass)
                            {
                                match = parser::Match(false);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch6 = &match;
                        {
                            parser::Match match(false);
                            if (*lexer == LPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch7 = &match;
                    {
                        parser::Match match = XPathParser::Literal(lexer);
                        pi.reset(static_cast<xpath::XPathExpr*>(match.value));
                        *parentMatch7 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch8 = &match;
                {
                    parser::Match match(false);
                    parser::Match* parentMatch9 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        parser::Match match(false);
                        if (*lexer == RPAREN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NodeTest"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                                return parser::Match(true, new xpath::XPathPILiteralTest(pi.release()));
                            }
                        }
                        *parentMatch9 = match;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch10 = &match;
                lexer.SetPos(save);
                {
                    parser::Match match(false);
                    parser::Match* parentMatch11 = &match;
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch12 = &match;
                        {
                            parser::Match match = XPathParser::NodeType(lexer);
                            nodeType.reset(static_cast<xpath::XPathNodeTestExpr*>(match.value));
                            *parentMatch12 = match;
                        }
                        if (match.hit)
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch13 = &match;
                            {
                                parser::Match match(false);
                                if (*lexer == LPAREN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch13 = match;
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch11 = match;
                    }
                    if (match.hit)
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch14 = &match;
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch15 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                parser::Match match(false);
                                if (*lexer == RPAREN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NodeTest"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                                        return parser::Match(true, nodeType.release());
                                    }
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch14 = match;
                        }
                        *parentMatch11 = match;
                    }
                    *parentMatch10 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch16 = &match;
            lexer.SetPos(save);
            {
                parser::Match match(false);
                parser::Match* parentMatch17 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::NameTest(lexer);
                    nameTest.reset(static_cast<xpath::XPathNodeTestExpr*>(match.value));
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NodeTest"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, nameTest.release());
                        }
                    }
                    *parentMatch17 = match;
                }
                *parentMatch16 = match;
            }
            *parentMatch0 = match;
        }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NodeTest"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("NodeTest"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::NodeType(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("NodeType"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        bool pass = true;
        parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            lexer::Token token = lexer.GetToken(pos);
            switch (lexer.GetKeywordToken(token.match))
            {
            case COMMENT: {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NodeType"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new xpath::XPathCommentNodeTest);
            }
            case TEXT: {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NodeType"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new xpath::XPathTextNodeTest);
            }
            case PROCESSING_INSTRUCTION: {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NodeType"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new xpath::XPathPINodeTest);
            }
            case NODE: {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NodeType"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new xpath::XPathAnyNodeTest);
            }
            default: {
                pass = false;
                break;
            }
            }
        }
        if (match.hit && !pass)
        {
            match = parser::Match(false);
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NodeType"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("NodeType"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::NameTest(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("NameTest"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<parser::Value<std::u32string>> ncname;
    std::unique_ptr<parser::Value<std::u32string>> qname;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                parser::Match match(false);
                if (*lexer == STAR)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NameTest"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                        return parser::Match(true, new xpath::XPathPrincipalNodeTest);
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                lexer.SetPos(save);
                {
                    parser::Match match(false);
                    parser::Match* parentMatch4 = &match;
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch5 = &match;
                        {
                            parser::Match match = XPathParser::NCName(lexer);
                            ncname.reset(static_cast<parser::Value<std::u32string>*>(match.value));
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch6 = &match;
                            {
                                parser::Match match(false);
                                if (*lexer == COLON)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch7 = &match;
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch8 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                parser::Match match(false);
                                if (*lexer == STAR)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NameTest"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                                        return parser::Match(true, new xpath::XPathPrefixTest(util::ToUtf8(ncname->value)));
                                    }
                                }
                                *parentMatch8 = match;
                            }
                            *parentMatch7 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch9 = &match;
            lexer.SetPos(save);
            {
                parser::Match match(false);
                parser::Match* parentMatch10 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::QName(lexer);
                    qname.reset(static_cast<parser::Value<std::u32string>*>(match.value));
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NameTest"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, new xpath::XPathNameTest(util::ToUtf8(qname->value)));
                        }
                    }
                    *parentMatch10 = match;
                }
                *parentMatch9 = match;
            }
            *parentMatch0 = match;
        }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NameTest"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("NameTest"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::AbbreviatedStep(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("AbbreviatedStep"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    parser::Match match(false);
    int64_t pos = lexer.GetPos();
    lexer::Span span = lexer.GetSpan();
    switch (*lexer)
    {
    case DOTDOT:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AbbreviatedStep"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new xpath::XPathLocationStepExpr(xml::Axis::parent, new xpath::XPathAnyNodeTest));
            }
        }
        break;
    }
    case DOT:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AbbreviatedStep"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new xpath::XPathLocationStepExpr(xml::Axis::self, new xpath::XPathAnyNodeTest));
            }
        }
        break;
    }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("AbbreviatedStep"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("AbbreviatedStep"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::Literal(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Literal"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    parser::Match match(false);
    int64_t pos = lexer.GetPos();
    lexer::Span span = lexer.GetSpan();
    switch (*lexer)
    {
    case DQSTRING:
    {
        ++lexer;
        {
            lexer::Token token = lexer.GetToken(pos);
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Literal"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new xpath::XPathLiteral(xpath::ParseDQString(lexer.FileName(), token)));
            }
        }
        break;
    }
    case SQSTRING:
    {
        ++lexer;
        {
            lexer::Token token = lexer.GetToken(pos);
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Literal"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new xpath::XPathLiteral(xpath::ParseSQString(lexer.FileName(), token)));
            }
        }
        break;
    }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Literal"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Literal"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::Number(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Number"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        if (*lexer == NUMBER)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            lexer::Token token = lexer.GetToken(pos);
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Number"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new xpath::XPathNumberExpr(util::ToUtf8(token.match.ToString())));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Number"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Number"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::Predicate(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Predicate"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    parser::Match match(false);
                    if (*lexer == LBRACKET)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch4 = &match;
                    {
                        parser::Match match = XPathParser::PredicateExpr(lexer);
                        expr.reset(static_cast<xpath::XPathExpr*>(match.value));
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch5 = &match;
                {
                    parser::Match match(false);
                    if (*lexer == RBRACKET)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch5 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Predicate"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Predicate"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Predicate"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::PredicateExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("PredicateExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match = XPathParser::Expr(lexer);
        expr.reset(static_cast<xpath::XPathExpr*>(match.value));
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PredicateExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PredicateExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("PredicateExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::PrimaryExpr(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("PrimaryExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> functionCall;
    std::unique_ptr<xpath::XPathExpr> variableReference;
    std::unique_ptr<xpath::XPathExpr> expr;
    std::unique_ptr<xpath::XPathExpr> literal;
    std::unique_ptr<xpath::XPathExpr> number;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                int64_t save = lexer.GetPos();
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t save = lexer.GetPos();
                    parser::Match match(false);
                    parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        parser::Match match = XPathParser::FunctionCall(lexer);
                        functionCall.reset(static_cast<xpath::XPathExpr*>(match.value));
                        if (match.hit)
                        {
                            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PrimaryExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                                return parser::Match(true, functionCall.release());
                            }
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                    if (!match.hit)
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch5 = &match;
                        lexer.SetPos(save);
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch6 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                parser::Match match = XPathParser::VariableReference(lexer);
                                variableReference.reset(static_cast<xpath::XPathExpr*>(match.value));
                                if (match.hit)
                                {
                                    {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PrimaryExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                                        return parser::Match(true, variableReference.release());
                                    }
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch7 = &match;
                    lexer.SetPos(save);
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch8 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            parser::Match match(false);
                            parser::Match* parentMatch9 = &match;
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch10 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch11 = &match;
                                    {
                                        parser::Match match(false);
                                        if (*lexer == LPAREN)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch11 = match;
                                    }
                                    if (match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch12 = &match;
                                        {
                                            parser::Match match = XPathParser::Expr(lexer);
                                            expr.reset(static_cast<xpath::XPathExpr*>(match.value));
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                if (match.hit)
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch13 = &match;
                                    {
                                        parser::Match match(false);
                                        if (*lexer == RPAREN)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch13 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            if (match.hit)
                            {
                                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PrimaryExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                                    return parser::Match(true, expr.release());
                                }
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch14 = &match;
                lexer.SetPos(save);
                {
                    parser::Match match(false);
                    parser::Match* parentMatch15 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        parser::Match match = XPathParser::Literal(lexer);
                        literal.reset(static_cast<xpath::XPathExpr*>(match.value));
                        if (match.hit)
                        {
                            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PrimaryExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                                return parser::Match(true, literal.release());
                            }
                        }
                        *parentMatch15 = match;
                    }
                    *parentMatch14 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch16 = &match;
            lexer.SetPos(save);
            {
                parser::Match match(false);
                parser::Match* parentMatch17 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::Number(lexer);
                    number.reset(static_cast<xpath::XPathExpr*>(match.value));
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PrimaryExpr"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, number.release());
                        }
                    }
                    *parentMatch17 = match;
                }
                *parentMatch16 = match;
            }
            *parentMatch0 = match;
        }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PrimaryExpr"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("PrimaryExpr"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::VariableReference(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("VariableReference"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<parser::Value<std::u32string>> qname;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                if (*lexer == DOLLAR)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    parser::Match match = XPathParser::QName(lexer);
                    qname.reset(static_cast<parser::Value<std::u32string>*>(match.value));
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("VariableReference"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new xpath::XPathVariableReference(util::ToUtf8(qname->value)));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("VariableReference"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("VariableReference"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::FunctionCall(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("FunctionCall"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathFunctionCall> functionCall = std::unique_ptr<xpath::XPathFunctionCall>();
    std::unique_ptr<parser::Value<std::u32string>> functionName;
    std::unique_ptr<xpath::XPathExpr> arg;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    parser::Match match(false);
                    parser::Match* parentMatch4 = &match;
                    {
                        parser::Match match = XPathParser::FunctionName(lexer);
                        functionName.reset(static_cast<parser::Value<std::u32string>*>(match.value));
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch5 = &match;
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch6 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                parser::Match match(false);
                                if (*lexer == LPAREN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    functionCall.reset(new xpath::XPathFunctionCall(util::ToUtf8(functionName->value)));
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch7 = &match;
                    {
                        parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        parser::Match* parentMatch8 = &match;
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch9 = &match;
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch10 = &match;
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch11 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        parser::Match match = XPathParser::Argument(lexer);
                                        arg.reset(static_cast<xpath::XPathExpr*>(match.value));
                                        if (match.hit)
                                        {
                                            functionCall->AddArgument(arg.release());
                                        }
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                if (match.hit)
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch12 = &match;
                                    {
                                        parser::Match match(true);
                                        parser::Match* parentMatch13 = &match;
                                        {
                                            while (true)
                                            {
                                                int64_t save = lexer.GetPos();
                                                {
                                                    parser::Match match(false);
                                                    parser::Match* parentMatch14 = &match;
                                                    {
                                                        parser::Match match(false);
                                                        if (*lexer == COMMA)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        *parentMatch14 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        parser::Match match(false);
                                                        parser::Match* parentMatch15 = &match;
                                                        {
                                                            parser::Match match(false);
                                                            parser::Match* parentMatch16 = &match;
                                                            {
                                                                int64_t pos = lexer.GetPos();
                                                                parser::Match match = XPathParser::Argument(lexer);
                                                                arg.reset(static_cast<xpath::XPathExpr*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    functionCall->AddArgument(arg.release());
                                                                }
                                                                *parentMatch16 = match;
                                                            }
                                                            *parentMatch15 = match;
                                                        }
                                                        *parentMatch14 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch13 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.SetPos(save);
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch8 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch17 = &match;
                {
                    parser::Match match(false);
                    if (*lexer == RPAREN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch17 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("FunctionCall"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, functionCall.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("FunctionCall"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("FunctionCall"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::FunctionName(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("FunctionName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<parser::Value<std::u32string>> qname;
    std::unique_ptr<xpath::XPathNodeTestExpr> nodeType;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            int64_t save = lexer.GetPos();
            {
                parser::Match match = XPathParser::QName(lexer);
                qname.reset(static_cast<parser::Value<std::u32string>*>(match.value));
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t tmp = lexer.GetPos();
                    lexer.SetPos(save);
                    save = tmp;
                    parser::Match match = XPathParser::NodeType(lexer);
                    nodeType.reset(static_cast<xpath::XPathNodeTestExpr*>(match.value));
                    *parentMatch3 = match;
                }
                if (!match.hit)
                {
                    lexer.SetPos(save);
                }
                *parentMatch2 = parser::Match(!match.hit, match.value);
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("FunctionName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<std::u32string>(qname->value));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("FunctionName"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("FunctionName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::Argument(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Argument"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<xpath::XPathExpr> expr;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match = XPathParser::Expr(lexer);
        expr.reset(static_cast<xpath::XPathExpr*>(match.value));
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Argument"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Argument"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Argument"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::QName(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("QName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<parser::Value<std::u32string>> prefixedName;
    std::unique_ptr<parser::Value<std::u32string>> unprefixedName;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            parser::Match match = XPathParser::PrefixedName(lexer);
            prefixedName.reset(static_cast<parser::Value<std::u32string>*>(match.value));
            if (match.hit)
            {
                {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("QName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                    return parser::Match(true, new parser::Value<std::u32string>(prefixedName->value));
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            lexer.SetPos(save);
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    parser::Match match = XPathParser::UnprefixedName(lexer);
                    unprefixedName.reset(static_cast<parser::Value<std::u32string>*>(match.value));
                    if (match.hit)
                    {
                        {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("QName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                            return parser::Match(true, new parser::Value<std::u32string>(unprefixedName->value));
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch0 = match;
        }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("QName"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("QName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::PrefixedName(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("PrefixedName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<parser::Value<std::u32string>> prefix;
    std::unique_ptr<parser::Value<std::u32string>> localPart;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        parser::Match* parentMatch1 = &match;
        {
            parser::Match match(false);
            parser::Match* parentMatch2 = &match;
            {
                parser::Match match(false);
                parser::Match* parentMatch3 = &match;
                {
                    parser::Match match = XPathParser::Prefix(lexer);
                    prefix.reset(static_cast<parser::Value<std::u32string>*>(match.value));
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch4 = &match;
                    {
                        parser::Match match(false);
                        if (*lexer == COLON)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch5 = &match;
                {
                    parser::Match match = XPathParser::LocalPart(lexer);
                    localPart.reset(static_cast<parser::Value<std::u32string>*>(match.value));
                    *parentMatch5 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PrefixedName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<std::u32string>(prefix->value + U":" + localPart->value));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("PrefixedName"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("PrefixedName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::Prefix(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Prefix"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<parser::Value<std::u32string>> ncname;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match = XPathParser::NCName(lexer);
        ncname.reset(static_cast<parser::Value<std::u32string>*>(match.value));
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Prefix"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<std::u32string>(ncname->value));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Prefix"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Prefix"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::UnprefixedName(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("UnprefixedName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<parser::Value<std::u32string>> localPart;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match = XPathParser::LocalPart(lexer);
        localPart.reset(static_cast<parser::Value<std::u32string>*>(match.value));
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("UnprefixedName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<std::u32string>(localPart->value));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("UnprefixedName"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("UnprefixedName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::LocalPart(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("LocalPart"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<parser::Value<std::u32string>> ncname;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match = XPathParser::NCName(lexer);
        ncname.reset(static_cast<parser::Value<std::u32string>*>(match.value));
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("LocalPart"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<std::u32string>(ncname->value));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("LocalPart"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("LocalPart"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match XPathParser::NCName(XPathLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("NCName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            lexer::Token token = lexer.GetToken(pos);
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NCName"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<std::u32string>(token.match.ToString()));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("NCName"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("NCName"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}
