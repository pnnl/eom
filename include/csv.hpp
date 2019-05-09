#ifndef GOV_PNNL_RIM_CSV_HPP
#define GOV_PNNL_RIM_CSV_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <memory>
namespace gov {
namespace pnnl {
namespace rim {
namespace csv {

class Reader {
  public:
    Reader(std::auto_ptr<std::istream> in);
    std::vector<std::vector<std::string> > readRows(void);
    std::vector<std::string> readRow(void);
    bool end(void) const;
  private:
    const std::auto_ptr<std::istream> in; // const, so ownership of istream never leaves the reader
    long rowsRead;
};

class Dictionary {
	public:
		Dictionary(void);
		std::string operator[](std::string) const;
		void add(std::string name, std::string value);
		bool contains(std::string name);
	private:
		std::map<std::string, std::string> entries;
};

class DictionaryReader {
  public:
    DictionaryReader(std::auto_ptr<std::istream> in);
    DictionaryReader(std::istream * in);
    std::vector<std::string> columns(void);
		const Dictionary next(void);
		bool end(void);
	private:
		Reader reader;
		std::vector<std::string> columnNames;
		void initialize();
		DictionaryReader(const DictionaryReader &original); // hide as default copy may not work as expected with auto_ptr
};


} // end namespace csv
} // end namespace rim
} // end namespace pnnl
} // end namespace gov

#endif
