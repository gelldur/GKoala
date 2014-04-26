/*
 * KeyboardLayout.h
 *
 *  Created on: Oct 9, 2013
 *      Author: kolo
 */

#ifndef KEYBORADLAYOUT_H_
#define KEYBORADLAYOUT_H_

#include "dictionaries/language/Alphabet.h"
#include <map>
#include <sstream>

using namespace std;

namespace keyboard
{

enum LettersOrder
{
	QWERTY = 0, QWERTZ, AZERTY, NONE
};

enum LayoutSettings
{
	NOT_SET = -1, NATIVE = 0, LEARNING = 1
};

struct KeyboardLayoutRows
{
	unsigned short int firstRow;
	unsigned short int secondRow;
	unsigned short int thirdRow;

	KeyboardLayoutRows ( unsigned short int firstRow = 10,
						 unsigned short int secondRow = 9,
						 unsigned short int thirdRow = 7 ) :
		firstRow ( firstRow ),
		secondRow ( secondRow ),
		thirdRow ( thirdRow )
	{
	}
};

struct KeyboardLayout
{
	KeyboardLayoutRows layoutRows;

	Alphabet alphabet;
	map<const string, const string> specialSigns;

	bool isLatinaLanguage;
	bool isSymbolLayout;

	string iso;

	KeyboardLayout() :
		isLatinaLanguage ( true ),
		isSymbolLayout ( false ),
		iso ( "-" )
	{
	}

	KeyboardLayout ( const Alphabet& alphabet,
					 const map<const string, const string>& specialSigns,
					 const KeyboardLayoutRows& layoutRows, bool isLatinaLanguage,
					 bool isSymbolLayout,
					 const string& iso = "-" ) :
		layoutRows ( layoutRows ),
		alphabet ( alphabet ),
		specialSigns ( specialSigns ),
		isLatinaLanguage ( isLatinaLanguage ),
		isSymbolLayout ( isSymbolLayout ),
		iso ( iso )
	{
	}

	Alphabet getAlphabet() const
	{
		ostringstream oss;

		for ( const pair<const string, const string>& signs : specialSigns )
		{
			oss << signs.second;
		}

		return alphabet + Alphabet::parseAlphabet ( oss.str() );
	}

	void setAlphabet ( const Alphabet& alphabet )
	{
		this->alphabet = alphabet;
	}

	static KeyboardLayout QWERTYKeyboardLayout();
	static KeyboardLayout QWERTZKeyboardLayout();
	static KeyboardLayout AZERTYKeyboardLayout();
	static KeyboardLayout PolishKeyboardLayout();
	static KeyboardLayout EnglishKeyboardLayout();
	static KeyboardLayout RussianKeyboardLayout();
	static KeyboardLayout BelorussianKeyboardLayout();
	static KeyboardLayout BulgarianKeyboardLayout();
	static KeyboardLayout CroatianKeyboardLayout();
	static KeyboardLayout GermanKeyboardLayout();
	static KeyboardLayout FrenchKeyboardLayout();
	static KeyboardLayout SpanishKeyboardLayout();
	static KeyboardLayout ItalianKeyboardLayout();
	static KeyboardLayout VietnameseKeyboardLayout();
	static KeyboardLayout SwedishKeyboardLayout();
	static KeyboardLayout CzechKeyboardLayout();
	static KeyboardLayout GreekKeyboardLayout();
	static KeyboardLayout DanishKeyboardLayout();
	static KeyboardLayout EstonianKeyboardLayout();
	static KeyboardLayout FinnishKeyboardLayout();
	static KeyboardLayout HungarianKeyboardLayout();
	static KeyboardLayout IcelanticKeyboardLayout();
	static KeyboardLayout IndonesianKeyboardLayout();
	static KeyboardLayout LatvianKeyboardLayout();
	static KeyboardLayout LithuanianKeyboardLayout();
	static KeyboardLayout MacedonianKeyboardLayout();
	static KeyboardLayout NorwegianKeyboardLayout();
	static KeyboardLayout PortugueseKeyboardLayout();
	static KeyboardLayout RomanianKeyboardLayout();
	static KeyboardLayout SlovakKeyboardLayout();
	static KeyboardLayout UkrainianKeyboardLayout();
	static KeyboardLayout TurkishKeyboardLayout();
	static KeyboardLayout DutchKeyboardLayout();
	static KeyboardLayout MalteseKeyboardLayout();
	static KeyboardLayout SerbianKeyboardLayout();
	static KeyboardLayout SlovenianKeyboardLayout();

};

} /* namespace keyboard */

#endif /* KEYBORADLAYOUT_H_ */
