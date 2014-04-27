/*
 * KeyboradLayout.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: kolo
 */

#include "view/keyboard/KeyboardLayout.h"
#include "dictionaries/language/LanguageManager.h"

#define MAP_INSERT(MAP,SIGN,SIGNS) MAP.insert(pair<const string, const string>(SIGN,SIGNS));

namespace keyboard
{

KeyboardLayout KeyboardLayout::QWERTYKeyboardLayout()
{
	return KeyboardLayout ( Alphabet::AlphabetQWERTY(),
							map<const string, const string>(),
							KeyboardLayoutRows(), true, false, "-" );
}

KeyboardLayout KeyboardLayout::QWERTZKeyboardLayout()
{
	return KeyboardLayout ( Alphabet::AlphabetQWERTZ(),
							map<const string, const string>(),
							KeyboardLayoutRows(), true, false, "-" );
}

KeyboardLayout KeyboardLayout::AZERTYKeyboardLayout()
{
	return KeyboardLayout ( Alphabet::AlphabetAZERTY(),
							map<const string, const string>(),
							KeyboardLayoutRows(), true, false, "-" );
}

KeyboardLayout KeyboardLayout::PolishKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "e", "ę" )
	MAP_INSERT ( signs, "o", "ó" )
	MAP_INSERT ( signs, "a", "ą" )
	MAP_INSERT ( signs, "s", "ś" )
	MAP_INSERT ( signs, "l", "ł" )
	MAP_INSERT ( signs, "z", "żź" )
	MAP_INSERT ( signs, "c", "ć" )
	MAP_INSERT ( signs, "n", "ń" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "pl" );
}

KeyboardLayout KeyboardLayout::EnglishKeyboardLayout()
{
	return KeyboardLayout ( Alphabet::AlphabetQWERTY(),
							map<const string, const string>(),
							KeyboardLayoutRows(), true, false, "en" );
}

KeyboardLayout KeyboardLayout::RussianKeyboardLayout()
{
	return KeyboardLayout (
			   Alphabet ( "йцукенгшщзхъфывапролджэячсмитьбюё" ),
			   map<const string, const string>(), KeyboardLayoutRows ( 12, 12, 9 ), false,
			   false,
			   "ru" );
}

KeyboardLayout KeyboardLayout::BelorussianKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "е", "ё" )
	return KeyboardLayout ( Alphabet::AlphabetBelorussian(), signs,
							KeyboardLayoutRows ( 11, 11, 9 ),
							false, false, "be" );
}

KeyboardLayout KeyboardLayout::BulgarianKeyboardLayout()
{
	return KeyboardLayout (
			   Alphabet::AlphabetBulgarian(),
			   map<const string, const string>(), KeyboardLayoutRows ( 11, 11, 8 ), false,
			   false,
			   "bg" );
}

KeyboardLayout KeyboardLayout::CroatianKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "s", "š" )
	MAP_INSERT ( signs, "d", "đ" )
	MAP_INSERT ( signs, "c", "čć" )
	return KeyboardLayout ( Alphabet::AlphabetQWERTZ(), signs, KeyboardLayoutRows(),
							true,
							false, "hr" );
}

KeyboardLayout KeyboardLayout::GermanKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "ä" )
	MAP_INSERT ( signs, "o", "ö" )
	MAP_INSERT ( signs, "u", "ü" )
	MAP_INSERT ( signs, "s", "ß" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTZ(), signs, KeyboardLayoutRows(),
							true,
							false, "de" );
}

KeyboardLayout KeyboardLayout::FrenchKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "q", "à" )
	MAP_INSERT ( signs, "e", "éè" )
	MAP_INSERT ( signs, "u", "ùú" )
	MAP_INSERT ( signs, "i", "ìíî" )
	MAP_INSERT ( signs, "o", "òó" )
	return KeyboardLayout ( Alphabet::AlphabetAZERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "fr" );
}

KeyboardLayout KeyboardLayout::SpanishKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "e", "é" )
	MAP_INSERT ( signs, "u", "úü" )
	MAP_INSERT ( signs, "i", "í" )
	MAP_INSERT ( signs, "o", "ó" )
	MAP_INSERT ( signs, "a", "á" )
	MAP_INSERT ( signs, "n", "ñ" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "es" );
}

KeyboardLayout KeyboardLayout::ItalianKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "e", "èé" )
	MAP_INSERT ( signs, "u", "ùú" )
	MAP_INSERT ( signs, "i", "ìíî" )
	MAP_INSERT ( signs, "o", "òó" )
	MAP_INSERT ( signs, "a", "à" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "it" );
}

KeyboardLayout KeyboardLayout::VietnameseKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "e", "ê" )
	MAP_INSERT ( signs, "u", "ư" )
	MAP_INSERT ( signs, "o", "ôơ" )
	MAP_INSERT ( signs, "a", "âă" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "vi" );
}

KeyboardLayout KeyboardLayout::SwedishKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "åä" )
	MAP_INSERT ( signs, "o", "ö" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "sv" );
}

KeyboardLayout KeyboardLayout::CzechKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "e", "ěé" )
	MAP_INSERT ( signs, "r", "ř" )
	MAP_INSERT ( signs, "z", "ž" )
	MAP_INSERT ( signs, "i", "í" )
	MAP_INSERT ( signs, "a", "á" )
	MAP_INSERT ( signs, "s", "š" )
	MAP_INSERT ( signs, "z", "ý" )
	MAP_INSERT ( signs, "c", "č" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTZ(), signs, KeyboardLayoutRows(),
							true,
							false, "cs" );
}

KeyboardLayout KeyboardLayout::GreekKeyboardLayout()
{
	return KeyboardLayout (
			   Alphabet ( ".ςερτυθιοπασδφγηξκλζχψωβνμ" ),
			   map<const string, const string>(), KeyboardLayoutRows(), false, false, "gre" );
}

KeyboardLayout KeyboardLayout::DanishKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "æå" )
	MAP_INSERT ( signs, "o", "ø" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "da" );
}

KeyboardLayout KeyboardLayout::EstonianKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "s", "š" )
	MAP_INSERT ( signs, "z", "ž" )
	MAP_INSERT ( signs, "a", "ä" )
	MAP_INSERT ( signs, "o", "õö" )
	MAP_INSERT ( signs, "u", "ü" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "et" );
}

KeyboardLayout KeyboardLayout::FinnishKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "åä" )
	MAP_INSERT ( signs, "o", "ö" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "fin" );
}

KeyboardLayout KeyboardLayout::HungarianKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "e", "é" )
	MAP_INSERT ( signs, "a", "á" )
	MAP_INSERT ( signs, "o", "óöő" )
	MAP_INSERT ( signs, "u", "úüű" )
	MAP_INSERT ( signs, "i", "í" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTZ(), signs, KeyboardLayoutRows(),
							true,
							false, "hu" );
}

KeyboardLayout KeyboardLayout::IcelanticKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "áæ" )
	MAP_INSERT ( signs, "d", "ð" )
	MAP_INSERT ( signs, "e", "é" )
	MAP_INSERT ( signs, "i", "í" )
	MAP_INSERT ( signs, "o", "óö" )
	MAP_INSERT ( signs, "u", "ú" )
	MAP_INSERT ( signs, "y", "ý" )
	MAP_INSERT ( signs, "b", "þ" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "is" );
}

KeyboardLayout KeyboardLayout::IndonesianKeyboardLayout()
{
	return KeyboardLayout ( Alphabet::AlphabetQWERTY(),
							map<const string, const string>(),
							KeyboardLayoutRows(), true, false, "id" );
}

KeyboardLayout KeyboardLayout::LatvianKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "ā" )
	MAP_INSERT ( signs, "c", "č" )
	MAP_INSERT ( signs, "e", "ē" )
	MAP_INSERT ( signs, "g", "ģ" )
	MAP_INSERT ( signs, "i", "ī" )
	MAP_INSERT ( signs, "s", "š" )
	MAP_INSERT ( signs, "u", "ū" )
	MAP_INSERT ( signs, "z", "ž" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "lv" );
}

KeyboardLayout KeyboardLayout::LithuanianKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "ą" )
	MAP_INSERT ( signs, "c", "č" )
	MAP_INSERT ( signs, "e", "ęė" )
	MAP_INSERT ( signs, "i", "į" )
	MAP_INSERT ( signs, "s", "š" )
	MAP_INSERT ( signs, "u", "ųū" )
	MAP_INSERT ( signs, "z", "ž" )

	return KeyboardLayout ( Alphabet::AlphabetAZERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "lt" );
}

KeyboardLayout KeyboardLayout::MacedonianKeyboardLayout()
{
	return KeyboardLayout (
			   Alphabet ( "љњертѕуиопшѓасдфгхјклчќжзџцвбнм" ),
			   map<const string, const string>(), KeyboardLayoutRows ( 12, 12, 7 ), false,
			   false,
			   "mk" );
}

KeyboardLayout KeyboardLayout::NorwegianKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "æå" )
	MAP_INSERT ( signs, "o", "ø" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "no" );
}

KeyboardLayout KeyboardLayout::PortugueseKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "áâãà" )
	MAP_INSERT ( signs, "c", "ç" )
	MAP_INSERT ( signs, "e", "éê" )
	MAP_INSERT ( signs, "i", "í" )
	MAP_INSERT ( signs, "o", "óôõ" )
	MAP_INSERT ( signs, "u", "ú" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "pt" );
}

KeyboardLayout KeyboardLayout::RomanianKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "ăâ" )
	MAP_INSERT ( signs, "s", "ș" )
	MAP_INSERT ( signs, "t", "ț" )
	MAP_INSERT ( signs, "i", "î" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "ro" );
}

KeyboardLayout KeyboardLayout::SlovakKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "a", "áä" )
	MAP_INSERT ( signs, "e", "é" )
	MAP_INSERT ( signs, "i", "í" )
	MAP_INSERT ( signs, "o", "óô" )
	MAP_INSERT ( signs, "u", "ú" )
	MAP_INSERT ( signs, "y", "ý" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTZ(), signs, KeyboardLayoutRows(),
							true,
							false, "sk" );
}

KeyboardLayout KeyboardLayout::UkrainianKeyboardLayout()
{
	return KeyboardLayout (
			   Alphabet ( "йцукенгшщзхґфівапролджєячсмитьбюї" ),
			   map<const string, const string>(), KeyboardLayoutRows ( 12, 11, 10 ), false,
			   false,
			   "uk" );
}

KeyboardLayout KeyboardLayout::TurkishKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "c", "ç" )
	MAP_INSERT ( signs, "g", "ğ" )
	MAP_INSERT ( signs, "i", "ı" )
	MAP_INSERT ( signs, "o", "ö" )
	MAP_INSERT ( signs, "s", "ş" )
	MAP_INSERT ( signs, "u", "ü" )

	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "tr" );
}

KeyboardLayout KeyboardLayout::DutchKeyboardLayout()
{
	return KeyboardLayout ( Alphabet::AlphabetAZERTY(),
							map<const string, const string>(),
							KeyboardLayoutRows(), true, false, "dum" );
}

KeyboardLayout KeyboardLayout::MalteseKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "c", "ċ" )
	MAP_INSERT ( signs, "g", "ġ" )
	MAP_INSERT ( signs, "h", "ħ" )
	MAP_INSERT ( signs, "z", "ż" )
	return KeyboardLayout ( Alphabet::AlphabetQWERTY(), signs, KeyboardLayoutRows(),
							true,
							false, "mt" );
}

KeyboardLayout KeyboardLayout::SerbianKeyboardLayout()
{
	return KeyboardLayout (
			   Alphabet ( "љњертзуиопшђасдфгхјклчћжџцвбнм" ),
			   map<const string, const string>(), KeyboardLayoutRows ( 12, 12, 7 ), false,
			   false,
			   "sr" );
}

KeyboardLayout KeyboardLayout::SlovenianKeyboardLayout()
{
	map<const string, const string> signs;
	MAP_INSERT ( signs, "c", "č" )
	MAP_INSERT ( signs, "s", "š" )
	MAP_INSERT ( signs, "z", "ž" )
	return KeyboardLayout ( Alphabet::AlphabetQWERTZ(), signs, KeyboardLayoutRows(),
							true,
							false, "sl" );
}

#undef MAP_INSERT

} /* namespace keyboard */
