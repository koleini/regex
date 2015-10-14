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
#include <sstream>
#include <stdexcept>
#include <iomanip>

#include "tcs.h"

using namespace std;

string TokenCaptureSeq::intToken (string::iterator& cIndex, string::iterator end)
{
  stringstream token;
  
  while (cIndex != end && isdigit(*cIndex))
    token << *(cIndex++);
  
  return token.str();
}

void TokenCaptureSeq::verify_parse_condition(bool condition, string str, int loc)
{
  if (condition)
  {
    stringstream msg;
      msg << "Parse error at input expression column " << (loc + 1) << ":" << endl
          << "'" << str << "'" << endl
          << std::setw(loc + 2) << "^";
   
    throw runtime_error(msg.str());
  }
}

string TokenCaptureSeq::tcs_regex_replace(string arg)
{
  stringstream regex, err;
  string::iterator cIndex;

  for(string::iterator strIndex=arg.begin(); strIndex != arg.end(); strIndex++)
  {
    // we are assuming that %{ in the original input (appears in arg as \%\{) is a reserved
    // character sequence for "token capture sequence"s. Therefore when parser reaches %{,
    // it always looks for a tcs pattern, or throws a parse-error exception.
    if (*strIndex == '\\' && *(strIndex + 1) == '%' && *(strIndex + 2) == '\\' && *(strIndex + 3) == '{')
    { // common for all token capture sequences
      cIndex = strIndex + 4;
      // read tcs index
      string tcs_idx = intToken (cIndex, arg.end());
      // check if tcs index is non-empty
      verify_parse_condition(!tcs_idx.length(), arg, cIndex - arg.begin());

      switch (*cIndex)
      {
        case 'S':
        { // matches token capture sequence - space limitation
          ++cIndex;
          // read space limit
          string space_limit = intToken (cIndex, arg.end());
          // check if space limit is non-empty
          verify_parse_condition(!space_limit.length() || *cIndex != '\\' || *(cIndex + 1) != '}', arg, cIndex - arg.begin());
            
          regex << "(\\w+)( {" << space_limit << "}(\\w+))*";
          strIndex = cIndex + 1;
          break;
        }
        case 'G':
        {
          // matches token capture sequence - greedy
          // NOTE: we pass non-greedy modifier to regex engine.
          // therefore, .*? represents a greedy match.
          verify_parse_condition(*(cIndex + 1) != '\\' || *(cIndex + 2) != '}', arg, cIndex - arg.begin());
            
          regex << ".+?";
          strIndex = cIndex + 2;
          break;
        }
        case '\\':
        {
          // matches token capture sequence with no modifier - non-greedy
          verify_parse_condition(*(cIndex + 1) != '}', arg, cIndex - arg.begin());
          regex << ".+";
          strIndex = cIndex + 1;
          break;
        }
        default:
          verify_parse_condition(true, arg, cIndex - arg.begin());
      }
    }
    else
      // macthes any character except '\' when follows by "%\{"
      regex << *strIndex;
  }

  return regex.str();
}
