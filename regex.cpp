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

#include "tcs.h"

using namespace std;

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
   
    TokenCaptureSeq tcs;
    
    // create equivalent regular expression
    string regex = tcs.tcs_regex_replace(arg);
    
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
