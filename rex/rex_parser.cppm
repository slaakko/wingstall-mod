
export module rex.parser;

import rex.context;
import rex.nfa;
import rex.lexer;
import lexer;
import parser;
import std.core;

// this file has been automatically generated from 'D:/work/soulng-project/soulng/rex/RexParser.parser' using soulng parser generator spg version 3.0.0

export struct RexParser
{
    static rex::Nfa Parse(RexLexer& lexer, rex::Context* context);
    static parser::Match RegularExpression(RexLexer& lexer, rex::Context* context);
    static parser::Match Alternative(RexLexer& lexer, rex::Context* context);
    static parser::Match Catenation(RexLexer& lexer, rex::Context* context);
    static parser::Match Repetition(RexLexer& lexer, rex::Context* context);
    static parser::Match Primary(RexLexer& lexer, rex::Context* context);
    static parser::Match Class(RexLexer& lexer, rex::Context* context);
    static parser::Match Range(RexLexer& lexer, rex::Context* context);
    static parser::Match Char(RexLexer& lexer);
};
