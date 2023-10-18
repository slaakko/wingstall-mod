module rex.parser;

import rex.tokens;
import rex.token_value_parsers;
import rex.symbol;
import util;

// this file has been automatically generated from 'D:/work/soulng-project/soulng/rex/RexParser.parser' using soulng parser generator spg version 3.0.0

using namespace RexTokens;

rex::Nfa RexParser::Parse(RexLexer& lexer, rex::Context* context)
{
    std::unique_ptr<parser::Value<rex::Nfa>> value;
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule(soulng::unicode::ToUtf32("parse"));
        lexer.Log()->IncIndent();
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    ++lexer;
    lexer::Span span = lexer.GetSpan();
    parser::Match match = RexParser::RegularExpression(lexer, context);
    value.reset(static_cast<parser::Value<rex::Nfa>*>(match.value));
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
            return value->value;
        }
        else
        {
            lexer.ThrowExpectationFailure(lexer.GetSpan(), util::ToUtf32(lexer::GetEndTokenInfo()));
        }
    }
    else
    {
        lexer.ThrowExpectationFailure(span, U"RegularExpression");
    }
    return value->value;
}

parser::Match RexParser::RegularExpression(RexLexer& lexer, rex::Context* context)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("RegularExpression"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    std::unique_ptr<parser::Value<rex::Nfa>> alternative;
    parser::Match match(false);
    parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        parser::Match match = RexParser::Alternative(lexer, context);
        alternative.reset(static_cast<parser::Value<rex::Nfa>*>(match.value));
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("RegularExpression"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<rex::Nfa>(alternative->value));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("RegularExpression"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("RegularExpression"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match RexParser::Alternative(RexLexer& lexer, rex::Context* context)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Alternative"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    rex::Nfa value = rex::Nfa();
    std::unique_ptr<parser::Value<rex::Nfa>> left;
    std::unique_ptr<parser::Value<rex::Nfa>> right;
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
                    parser::Match match = RexParser::Catenation(lexer, context);
                    left.reset(static_cast<parser::Value<rex::Nfa>*>(match.value));
                    if (match.hit)
                    {
                        value = left->value;
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
                                        if (*lexer == ALT)
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
                                                parser::Match match(true);
                                                parser::Match* parentMatch10 = &match;
                                                {
                                                    lexer::Span span = lexer.GetSpan();
                                                    parser::Match match = RexParser::Catenation(lexer, context);
                                                    right.reset(static_cast<parser::Value<rex::Nfa>*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.ThrowExpectationFailure(span, U"Catenation");
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    value = rex::Alt(*context, value, right->value);
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
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Alternative"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<rex::Nfa>(value));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Alternative"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Alternative"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match RexParser::Catenation(RexLexer& lexer, rex::Context* context)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Catenation"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    rex::Nfa value = rex::Nfa();
    std::unique_ptr<parser::Value<rex::Nfa>> left;
    std::unique_ptr<parser::Value<rex::Nfa>> right;
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
                    parser::Match match = RexParser::Repetition(lexer, context);
                    left.reset(static_cast<parser::Value<rex::Nfa>*>(match.value));
                    if (match.hit)
                    {
                        value = left->value;
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
                                        parser::Match match = RexParser::Repetition(lexer, context);
                                        right.reset(static_cast<parser::Value<rex::Nfa>*>(match.value));
                                        if (match.hit)
                                        {
                                            value = rex::Cat(value, right->value);
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
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Catenation"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<rex::Nfa>(value));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Catenation"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Catenation"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match RexParser::Repetition(RexLexer& lexer, rex::Context* context)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Repetition"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    rex::Nfa value = rex::Nfa();
    std::unique_ptr<parser::Value<rex::Nfa>> left;
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
                    parser::Match match = RexParser::Primary(lexer, context);
                    left.reset(static_cast<parser::Value<rex::Nfa>*>(match.value));
                    if (match.hit)
                    {
                        value = left->value;
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
                    int64_t save = lexer.GetPos();
                    parser::Match* parentMatch5 = &match;
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch6 = &match;
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch7 = &match;
                            {
                                int64_t save = lexer.GetPos();
                                parser::Match match(false);
                                parser::Match* parentMatch8 = &match;
                                {
                                    int64_t save = lexer.GetPos();
                                    parser::Match match(false);
                                    parser::Match* parentMatch9 = &match;
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
                                            value = rex::Kleene(*context, value);
                                        }
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch8 = match;
                                    if (!match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch10 = &match;
                                        lexer.SetPos(save);
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch11 = &match;
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
                                                    value = rex::Pos(*context, value);
                                                }
                                                *parentMatch11 = match;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                }
                                *parentMatch7 = match;
                                if (!match.hit)
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch12 = &match;
                                    lexer.SetPos(save);
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch13 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            parser::Match match(false);
                                            if (*lexer == QUEST)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                value = rex::Opt(*context, value);
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch7 = match;
                                }
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
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Repetition"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<rex::Nfa>(value));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Repetition"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Repetition"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match RexParser::Primary(RexLexer& lexer, rex::Context* context)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Primary"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    rex::Nfa value = rex::Nfa();
    std::unique_ptr<parser::Value<rex::Nfa>> alt;
    std::unique_ptr<rex::Class> cls;
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
                    int64_t save = lexer.GetPos();
                    parser::Match match(false);
                    parser::Match* parentMatch4 = &match;
                    {
                        int64_t save = lexer.GetPos();
                        parser::Match match(false);
                        parser::Match* parentMatch5 = &match;
                        {
                            int64_t save = lexer.GetPos();
                            parser::Match match(false);
                            parser::Match* parentMatch6 = &match;
                            {
                                int64_t save = lexer.GetPos();
                                parser::Match match(false);
                                parser::Match* parentMatch7 = &match;
                                {
                                    int64_t save = lexer.GetPos();
                                    parser::Match match(false);
                                    parser::Match* parentMatch8 = &match;
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch9 = &match;
                                        {
                                            parser::Match match(false);
                                            if (*lexer == LPAREN)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch9 = match;
                                        }
                                        if (match.hit)
                                        {
                                            parser::Match match(false);
                                            parser::Match* parentMatch10 = &match;
                                            {
                                                parser::Match match(false);
                                                parser::Match* parentMatch11 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    parser::Match match(true);
                                                    parser::Match* parentMatch12 = &match;
                                                    {
                                                        lexer::Span span = lexer.GetSpan();
                                                        parser::Match match = RexParser::Alternative(lexer, context);
                                                        alt.reset(static_cast<parser::Value<rex::Nfa>*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            *parentMatch12 = match;
                                                        }
                                                        else
                                                        {
                                                            lexer.ThrowExpectationFailure(span, U"Alternative");
                                                        }
                                                    }
                                                    if (match.hit)
                                                    {
                                                        value = alt->value;
                                                    }
                                                    *parentMatch11 = match;
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    if (match.hit)
                                    {
                                        parser::Match match(false);
                                        parser::Match* parentMatch13 = &match;
                                        {
                                            parser::Match match(true);
                                            parser::Match* parentMatch14 = &match;
                                            {
                                                lexer::Span span = lexer.GetSpan();
                                                parser::Match match(false);
                                                if (*lexer == RPAREN)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch14 = match;
                                                }
                                                else
                                                {
                                                    lexer.ThrowExpectationFailure(span, util::ToUtf32(GetTokenInfo(RPAREN)));
                                                }
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
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
                                                if (*lexer == ESCAPE)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    value = MakeNfa(*context, context->MakeChar(rex::MakeEscapeValue(lexer.FileName(), lexer.GetToken(pos))));
                                                }
                                                *parentMatch16 = match;
                                            }
                                            *parentMatch15 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                }
                                *parentMatch6 = match;
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
                                            if (*lexer == INVERSE)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                value = MakeNfa(*context, context->MakeChar('^'));
                                            }
                                            *parentMatch18 = match;
                                        }
                                        *parentMatch17 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                            }
                            *parentMatch5 = match;
                            if (!match.hit)
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch19 = &match;
                                lexer.SetPos(save);
                                {
                                    parser::Match match(false);
                                    parser::Match* parentMatch20 = &match;
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
                                            value = MakeNfa(*context, context->MakeChar('-'));
                                        }
                                        *parentMatch20 = match;
                                    }
                                    *parentMatch19 = match;
                                }
                                *parentMatch5 = match;
                            }
                        }
                        *parentMatch4 = match;
                        if (!match.hit)
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch21 = &match;
                            lexer.SetPos(save);
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch22 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    lexer::Span span = lexer.GetSpan();
                                    parser::Match match(false);
                                    if (*lexer == CHAR)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        std::u32string match = lexer.GetMatch(span);
                                        value = MakeNfa(*context, context->MakeChar(match.front()));
                                    }
                                    *parentMatch22 = match;
                                }
                                *parentMatch21 = match;
                            }
                            *parentMatch4 = match;
                        }
                    }
                    *parentMatch3 = match;
                    if (!match.hit)
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch23 = &match;
                        lexer.SetPos(save);
                        {
                            parser::Match match(false);
                            parser::Match* parentMatch24 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                parser::Match match(false);
                                if (*lexer == DOT)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    value = MakeNfa(*context, context->MakeAny());
                                }
                                *parentMatch24 = match;
                            }
                            *parentMatch23 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch25 = &match;
                    lexer.SetPos(save);
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch26 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            parser::Match match = RexParser::Class(lexer, context);
                            cls.reset(static_cast<rex::Class*>(match.value));
                            if (match.hit)
                            {
                                value = MakeNfa(*context, cls.release());
                            }
                            *parentMatch26 = match;
                        }
                        *parentMatch25 = match;
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
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Primary"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<rex::Nfa>(value));
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Primary"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Primary"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match RexParser::Class(RexLexer& lexer, rex::Context* context)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Class"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    rex::Class* cls = nullptr;
    std::unique_ptr<rex::Symbol> r;
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
                        int64_t pos = lexer.GetPos();
                        parser::Match match(false);
                        if (*lexer == LBRACKET)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            cls = context->MakeClass();
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    parser::Match match(false);
                    parser::Match* parentMatch5 = &match;
                    {
                        parser::Match match(false);
                        parser::Match* parentMatch6 = &match;
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
                                            parser::Match match(false);
                                            if (*lexer == INVERSE)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                cls->SetInverse();
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
                            if (match.hit)
                            {
                                parser::Match match(false);
                                parser::Match* parentMatch11 = &match;
                                {
                                    parser::Match match(true);
                                    parser::Match* parentMatch12 = &match;
                                    {
                                        while (true)
                                        {
                                            int64_t save = lexer.GetPos();
                                            {
                                                parser::Match match(false);
                                                parser::Match* parentMatch13 = &match;
                                                {
                                                    parser::Match match(false);
                                                    parser::Match* parentMatch14 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        parser::Match match = RexParser::Range(lexer, context);
                                                        r.reset(static_cast<rex::Symbol*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            cls->AddSymbol(r.release());
                                                        }
                                                        *parentMatch14 = match;
                                                    }
                                                    *parentMatch13 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch12 = match;
                                                }
                                                else
                                                {
                                                    lexer.SetPos(save);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    *parentMatch11 = match;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                parser::Match match(false);
                parser::Match* parentMatch15 = &match;
                {
                    parser::Match match(true);
                    parser::Match* parentMatch16 = &match;
                    {
                        lexer::Span span = lexer.GetSpan();
                        parser::Match match(false);
                        if (*lexer == RBRACKET)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            *parentMatch16 = match;
                        }
                        else
                        {
                            lexer.ThrowExpectationFailure(span, util::ToUtf32(GetTokenInfo(RBRACKET)));
                        }
                    }
                    *parentMatch15 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Class"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, cls);
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Class"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Class"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match RexParser::Range(RexLexer& lexer, rex::Context* context)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Range"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    rex::Symbol* symbol = nullptr;
    std::unique_ptr<parser::Value<char32_t>> s;
    std::unique_ptr<parser::Value<char32_t>> e;
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
                parser::Match match = RexParser::Char(lexer);
                s.reset(static_cast<parser::Value<char32_t>*>(match.value));
                *parentMatch2 = match;
            }
            if (match.hit)
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
                            int64_t save = lexer.GetPos();
                            parser::Match match(false);
                            parser::Match* parentMatch6 = &match;
                            {
                                parser::Match match(false);
                                if (*lexer == MINUS)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch6 = match;
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
                                        parser::Match match = RexParser::Char(lexer);
                                        e.reset(static_cast<parser::Value<char32_t>*>(match.value));
                                        if (match.hit)
                                        {
                                            symbol = context->MakeRange(s->value, e->value);
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
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
                                        parser::Match match(true);
                                        if (match.hit)
                                        {
                                            symbol = context->MakeChar(s->value);
                                        }
                                        *parentMatch10 = match;
                                    }
                                    *parentMatch9 = match;
                                }
                                *parentMatch5 = match;
                            }
                        }
                        *parentMatch4 = match;
                    }
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
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Range"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, symbol);
            }
        }
        *parentMatch0 = match;
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Range"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Range"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

parser::Match RexParser::Char(RexLexer& lexer)
{
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    lexer::Span parser_debug_match_span;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_span = lexer.GetSpan();
        lexer::WriteBeginRuleToLog(lexer, soulng::unicode::ToUtf32("Char"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    parser::Match match(false);
    int64_t pos = lexer.GetPos();
    lexer::Span span = lexer.GetSpan();
    switch (*lexer)
    {
    case LPAREN:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>('('));
            }
        }
        break;
    }
    case RPAREN:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>(')'));
            }
        }
        break;
    }
    case LBRACKET:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>('['));
            }
        }
        break;
    }
    case ALT:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>('|'));
            }
        }
        break;
    }
    case STAR:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>('*'));
            }
        }
        break;
    }
    case PLUS:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>('+'));
            }
        }
        break;
    }
    case QUEST:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>('?'));
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
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>('.'));
            }
        }
        break;
    }
    case ESCAPE:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>(rex::MakeEscapeValue(lexer.FileName(), lexer.GetToken(pos))));
            }
        }
        break;
    }
    case INVERSE:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>('^'));
            }
        }
        break;
    }
    case MINUS:
    {
        ++lexer;
        {
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>('-'));
            }
        }
        break;
    }
    case CHAR:
    {
        ++lexer;
        {
            std::u32string match = lexer.GetMatch(span);
            {
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
#endif // SOULNG_PARSER_DEBUG_SUPPORT
                return parser::Match(true, new parser::Value<char32_t>(match.front()));
            }
        }
        break;
    }
    }
#ifdef SOULNG_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) lexer::WriteSuccessToLog(lexer, parser_debug_match_span, soulng::unicode::ToUtf32("Char"));
        else lexer::WriteFailureToLog(lexer, soulng::unicode::ToUtf32("Char"));
    }
#endif // SOULNG_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}
