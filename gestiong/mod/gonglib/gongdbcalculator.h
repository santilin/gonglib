#ifndef _GONG_DBCALCULATOR_H
#define _GONG_DBCALCULATOR_H

/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

/** @file gongdbcalculator.h Simple expressions calculator
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>

#include <gongdbrecord.h>


namespace gong {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;
namespace ascii = boost::spirit::ascii;

///////////////////////////////////////////////////////////////////////////////
//  Our calculator grammar
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct calculator : qi::grammar<Iterator, double(), ascii::space_type>
{
    calculator(dbRecord *record) : calculator::base_type(expression), pRecord( record )
{
    using namespace qi::labels;
    using qi::uint_;
    using qi::double_;
    using qi::alpha;
    using qi::alnum;
    using qi::on_error;
    using qi::fail;
    using phoenix::at_c;

    using phoenix::construct;
    using phoenix::val;

    expression =
        term                            [_val = _1]
        >> *(   ('+' > term             [_val += _1])
                |   ('-' > term             [_val -= _1])
            )
        ;

    term =
        factor                          [_val = _1]
        >> *(   ('*' > factor           [_val *= _1])
                |   ('/' > factor           [_val /= _1])
            )
        ;

    factor =
        double_							[_val = _1]
        |   '(' > expression            [_val = _1] > ')'
        |   ('-' > factor               [_val = -_1])
        |   ('+' > factor               [_val = _1])
        | fieldname
        ;

    fieldname = alpha >> *(alnum | '_')
                - ( '.' >> alpha >> *(alnum | '_') ) [ _1 ];

    expression.name("expression");
    term.name("term");
    factor.name("factor");

    on_error<fail>
    (
        expression
        , std::cout
        << val("Error! Expecting ")
        << _4                               // what failed?
        << val(" here: \"")
        << construct<std::string>(_3, _2)   // iterators to error-pos, end
        << val("\"")
        << std::endl
    );
}

double getValueOfField(std::string &fldname) {
    std::cout << fldname << std::endl;
    return pRecord->getValue( fldname ).toDouble();
}
qi::rule<Iterator, double(), ascii::space_type> expression, term, factor;
qi::rule<Iterator, std::string(), ascii::space_type> fieldname;
dbRecord *pRecord;
};

} // namespace gong


#endif // _GONG_DBCALCULATOR_H
