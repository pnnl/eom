#include <stdexcept>
#include <set>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdio>
#include "csv.hpp"

using std::string;
using std::vector;
using std::map;
using std::auto_ptr;
using std::istream;
using std::logic_error;
using std::runtime_error;
using std::cout;
using std::cerr;
using std::endl;
using std::set;
using std::for_each;
using std::transform;

using namespace gov::pnnl::rim::csv;

enum State {
  ROW_START,
  VALUE_START,
  ESCAPED_VALUE,
  ESCAPED_VALUE_END,
  VALUE,
  CR,
  LF,
  END
};

const string state_names[] = {
  "ROW_START",
  "VALUE_START",
  "ESCAPED_VALUE",
  "ESCAPED_VALUE_END",
  "VALUE",
  "CR",
  "LF",
  "END"
};

Reader::Reader(auto_ptr<istream> in):
  in(in), rowsRead(0)
{
};

vector<vector<string> > Reader::readRows(void) {
  vector<vector<string> > rows;
  in->peek();
  while (in->good() && !in->eof()) {
    vector<string> row = readRow();
    rows.push_back(row);
    in->peek();
  }
  return rows;
}

class csv_read_error : public runtime_error {
	public:
		csv_read_error(string message, long rowsRead) :
			runtime_error(message + " at row #" + boost::lexical_cast<string>(rowsRead+1l))
		{
		}
};

vector<string> Reader::readRow(void) {
  vector<char> buffer;
  vector<string> values;
  char c;
  State state = ROW_START;
  if (in->eof())
    throw logic_error("check for end before reading row");
  if (in->bad()) {
	  perror("bad stream at start of row read");
		throw csv_read_error("bad stream at start of row read", rowsRead);
  }
  if (in->fail()) {
	  perror("input operation failed at start of row read");
	  throw csv_read_error("input operation failed at start of row read", rowsRead);
  }
  while (in->good() && END != state) {
    in->get(c);
    bool eof = in->eof();
    bool good = in->good();
    //cerr << state_names[state] << " " << repr(c) << " " << (good?"":"NOT") << "GOOD " << (eof?"EOF":"") << endl;
    if (ROW_START == state) {
      if (!eof && !good) {
        throw csv_read_error("bad read at row start", rowsRead);
      }
      if ('\n' == c) {
        state = END;
      } else if ('\r' == c) {
        state = CR;
      } else if ('"' == c) {
        state = ESCAPED_VALUE;
      } else {
        in->unget();
        state = VALUE;
      }
    } else if (VALUE_START == state) {
      if (!eof && !good) {
        throw csv_read_error("bad read at value start", rowsRead);
      }
      if ('"' == c) {
        state = ESCAPED_VALUE;
      } else {
        in->unget();
        in->clear();
        state = VALUE;
      }
    } else if (ESCAPED_VALUE == state) {
      if (eof)
        throw csv_read_error("eof inside string", rowsRead);
      if ('"' == c)
        state = ESCAPED_VALUE_END;
      else
        buffer.push_back(c);
    } else if (ESCAPED_VALUE_END == state) {
      if (eof) {
        values.push_back(string(buffer.begin(), buffer.end()));
        buffer.clear();
        state = END;
      } else if ('"' == c) {
        buffer.push_back(c);
        state = ESCAPED_VALUE;
      } else {
        in->unget();
        state = VALUE;
      }
    } else if (VALUE == state) {
      if (eof) {
        values.push_back(string(buffer.begin(), buffer.end()));
        buffer.clear();
        state = END;
      } else if (',' == c) {
        values.push_back(string(buffer.begin(), buffer.end()));
        buffer.clear();
        state = VALUE_START;
      } else if ('\n' == c) {
        values.push_back(string(buffer.begin(), buffer.end()));
        buffer.clear();
        state = END;
      } else if ('\r' == c) {
        values.push_back(string(buffer.begin(), buffer.end()));
        buffer.clear();
        state = CR;
      } else {
        buffer.push_back(c);
      }
    } else if (CR == state) {
      if ('\n' == c)
        state = LF;
      else {
        in->unget();
        state = END;
      }
    } else if (LF == state) {
      in->unget();
      state = END;
    } else {
      throw logic_error("unhandled state reading row " + boost::lexical_cast<string>(rowsRead+1l));
    }
  }
	rowsRead++;
  return values;
}

bool Reader::end(void) const {
	in->peek();
  return in->eof();
}

Dictionary::Dictionary(void): entries() {
}
string Dictionary::operator[](string name) const {
	string lowercaseName = boost::algorithm::to_lower_copy(name);
	map<string,string>::const_iterator iterator = entries.find(lowercaseName);
	if (entries.end() == iterator) {
		throw runtime_error("attempted to access csv value in non-existent column: " + name);
	}
	return iterator->second;
}
void Dictionary::add(string name, string value) {
	string lowercaseName = boost::algorithm::to_lower_copy(name);
	string trimmedValue = boost::algorithm::trim_copy(value);
	std::pair<string, string> entry = std::make_pair(lowercaseName, trimmedValue);
	entries.insert(entry);
}
bool Dictionary::contains(string name) {
	return entries.find(boost::algorithm::to_lower_copy(name)) != entries.end();
}

DictionaryReader::DictionaryReader(istream * in) : reader(auto_ptr<istream>(in)), columnNames(reader.readRow()) {
	initialize();
}
DictionaryReader::DictionaryReader(auto_ptr<istream> in) : reader(in), columnNames(reader.readRow()) {
	initialize();
}
void DictionaryReader::initialize() {
	// trim white space from headers
	for_each(columnNames.begin(), columnNames.end(), boost::bind(&boost::algorithm::trim<string>, _1, std::locale()));

	// validate that headers are unique
	set<string> headerSet;
	headerSet.insert(columnNames.begin(), columnNames.end());
	if (headerSet.size() != columnNames.size())
		throw runtime_error("duplicate headers!");
}

vector<string> DictionaryReader::columns(void) {
	return columnNames;
}

const Dictionary DictionaryReader::next(void) {
	if (end())
		throw std::out_of_range("No next row at end of input.");
	Dictionary row;
	vector<string> columnValues = reader.readRow();
	size_t index;
	for (index = 0; index < columnNames.size() && index < columnValues.size(); index++)
		row.add(columnNames.at(index), columnValues.at(index));
	for (; index < columnNames.size(); index++)
		row.add(columnNames.at(index),"");
	return row;
}

bool DictionaryReader::end(void) {
	return reader.end();
}


