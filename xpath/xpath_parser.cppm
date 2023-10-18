export module xpath.xpath_parser;

import xpath.xpath_expr;
import xpath.xpath_lexer;
import lexer;
import parser;
import std.core;

// this file has been automatically generated from 'D:/work/soulng-project/sngxml/xpath/XPathParser.parser' using soulng parser generator spg version 3.0.0

export struct XPathParser
{
    static std::unique_ptr<xpath::XPathExpr> Parse(XPathLexer& lexer);
    static parser::Match Expr(XPathLexer& lexer);
    static parser::Match OrExpr(XPathLexer& lexer);
    static parser::Match AndExpr(XPathLexer& lexer);
    static parser::Match EqualityExpr(XPathLexer& lexer);
    static parser::Match RelationalExpr(XPathLexer& lexer);
    static parser::Match AdditiveExpr(XPathLexer& lexer);
    static parser::Match MultiplicativeExpr(XPathLexer& lexer);
    static parser::Match UnaryExpr(XPathLexer& lexer);
    static parser::Match UnionExpr(XPathLexer& lexer);
    static parser::Match PathExpr(XPathLexer& lexer);
    static parser::Match FilterExpr(XPathLexer& lexer);
    static parser::Match LocationPath(XPathLexer& lexer);
    static parser::Match AbsoluteLocationPath(XPathLexer& lexer);
    static parser::Match AbbreviatedAbsoluteLocationPath(XPathLexer& lexer);
    static parser::Match RelativeLocationPath(XPathLexer& lexer);
    static parser::Match Step(XPathLexer& lexer);
    static parser::Match AxisSpecifier(XPathLexer& lexer);
    static parser::Match AxisName(XPathLexer& lexer);
    static parser::Match AbbreviatedAxisSpecifier(XPathLexer& lexer);
    static parser::Match NodeTest(XPathLexer& lexer);
    static parser::Match NodeType(XPathLexer& lexer);
    static parser::Match NameTest(XPathLexer& lexer);
    static parser::Match AbbreviatedStep(XPathLexer& lexer);
    static parser::Match Literal(XPathLexer& lexer);
    static parser::Match Number(XPathLexer& lexer);
    static parser::Match Predicate(XPathLexer& lexer);
    static parser::Match PredicateExpr(XPathLexer& lexer);
    static parser::Match PrimaryExpr(XPathLexer& lexer);
    static parser::Match VariableReference(XPathLexer& lexer);
    static parser::Match FunctionCall(XPathLexer& lexer);
    static parser::Match FunctionName(XPathLexer& lexer);
    static parser::Match Argument(XPathLexer& lexer);
    static parser::Match QName(XPathLexer& lexer);
    static parser::Match PrefixedName(XPathLexer& lexer);
    static parser::Match Prefix(XPathLexer& lexer);
    static parser::Match UnprefixedName(XPathLexer& lexer);
    static parser::Match LocalPart(XPathLexer& lexer);
    static parser::Match NCName(XPathLexer& lexer);
};
