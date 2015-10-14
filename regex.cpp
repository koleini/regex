/*
 * Copyright (c) 2015 Masoud Koleini <masoud.koleini@nottingham.ac.uk>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <iostream>
#include <string>
#include <pcrecpp.h>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <stdexcept>

/*
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
*/

#include "tcs.h"

using namespace std;
/*
using namespace boost;
using namespace boost::spirit;
*/

/*
 * Notes:
 * 
 * - There is no instuction on handing token capture sequence indexes.
 *   Therefore, it is ignored in the translation/error handling in the
 *   current revision of the program.
 * - It is assumed that syntax enforces token capture sequences not to
 *   contain any spaces.
 */

int main(int argc, char **argv)
{
  try {
    if (argc < 2)
    { // if no input is passed to the program
      stringstream msg;
      msg << "Input expression is not provided." << endl
          << " Usage: " << argv[0] << " [\"exression\"]"<< endl;
      throw runtime_error(msg.str());
    }

    // quoting possible re metacharacters
    string arg = pcrecpp::RE("").QuoteMeta(argv[1]);
   
    /*
    // escape regex special characters
    string re_spec_chars = "\\-[]{}()*+?.,^$|#";
    
    for(string::iterator it=re_spec_chars.begin(); it != re_spec_chars.end(); it++)
    {
       string re = *it == '\\' ? "\\\\" : string(1, *it);
       
       pcrecpp::RE("\\" + string(1, *it)).GlobalReplace("\\\\" + re, &arg);
    }
    */
    
    TokenCaptureSeq tcs;
    
    // create equivalent regular expression
    string regex = tcs.tcs_regex_replace(arg);
    
    /*
    using ascii::char_;
  
    string::iterator frst = arg.begin();
    string::iterator last = arg.end();
  
    bool match = qi::parse(frst, last, 
      lexeme [ // don't skip space characters
      +( // matches token capture sequence - space limitation
        (ascii::string("%\\{") >> qi::int_ >> char_('S') >> qi::int_ >> ascii::string("\\}"))
          [ phoenix::ref(regex) << "(\\w+)( {",
            phoenix::ref(regex) << qi::_4,
            phoenix::ref(regex) << "}(\\w+))*"
          ]
        |
        // matches token capture sequence - greedy
        // NOTE: we pass non-greedy modifier to regex engine.
        //  therefore, .*? represents a greedy match.
        (ascii::string("%\\{") >> qi::int_ >> char_('G') >> ascii::string("\\}"))
          [ phoenix::ref(regex) << ".+?" ]
        |
        // matches token capture sequence with no modifier - non-greedy
        (ascii::string("%\\{") >> qi::int_ >> ascii::string("\\}"))
          [ phoenix::ref(regex) << ".+" ]
        |
        // macthes any character except "%" when follows by "{"
        // NOTE: parser macthes left to right. So this branch is taken
        // when the previous ones don't match. 
        (char_ - (char_('%') >> ascii::string("\\}")))
          [ phoenix::ref(regex) << qi::_1 ]
       )
      ]
      );
    
    if (!match || frst != last)
    { // if parser doesn't match or parse the whole line
      stringstream msg;
      msg << "Parse error at input expression column " << (frst - arg.begin() + 1) << ":" << endl
          << "'" << arg << "'" << endl
          << std::setw(frst - arg.begin() + 2) << "^";
      throw runtime_error(msg.str());
    }
    
    */
    cout << "RegEx: " << regex << endl;

    pcrecpp::RE_Options opt;
    opt.set_ungreedy(true); // sets non-greedy option

    pcrecpp::RE re(regex, opt);

    for (string line; getline(cin, line);)
    // iterates over input line until receives EOF
      if (re.FullMatch(line))
        cout << line << endl;
  }
  catch (const std::exception& e)
  { 
    cerr << "Error: " << e.what() << endl;
    return -1;
  }
  
  return 0;
}
