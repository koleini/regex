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
    cIndex = strIndex;
   
    if (*cIndex == '\\' && *(cIndex + 1) == '%' && *(cIndex + 2) == '\\' && *(cIndex + 3) == '{')
    {
      string::iterator idx = cIndex;
      cIndex += 4;
      string tcs_idx = intToken (cIndex, arg.end());
      verify_parse_condition(cIndex == idx, arg, cIndex - arg.begin());
      
      switch (*cIndex)
      {
        case 'S':
        {
          idx = ++cIndex;

          string space_limit = intToken (cIndex, arg.end());
          verify_parse_condition(cIndex == idx || *cIndex != '\\' || *(cIndex + 1) != '}', arg, cIndex - arg.begin());
            
          regex << "(\\w+)( {" << space_limit << "}(\\w+))*";
          strIndex = cIndex + 1;
          break;
        }
        case 'G':
        {
          verify_parse_condition(*(cIndex + 1) != '\\' || *(cIndex + 2) != '}', arg, cIndex - arg.begin());
            
          regex << ".+?";
          strIndex = cIndex + 2;
          break;
        }
        case '\\':
        {
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
      regex << *strIndex;
  }

  return regex.str();
}
