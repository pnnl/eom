/*
 * ParserInfo.hpp
 *
 *  Created on: Mar 21, 2011
 *      Author: kevin
 */

#ifndef PARSERINFO_HPP_
#define PARSERINFO_HPP_

class ParserInfo
{
public:
	static LOGICAL isDelimiter(char c) {
		return c == ' ' | c == '\t' | c == '\n';
	}
protected:
private:

	ParserInfo(){};
	ParserInfo(ParserInfo & parser){};
	ParserInfo & operator = (ParserInfo & parser) {return *this;};
};

#endif /* PARSERINFO_HPP_ */
