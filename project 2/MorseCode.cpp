#include <string>
#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <cctype>
#include <regex>

#define STRINGIFY( ... ) #__VA_ARGS__

struct Pair
{
	int val;
	char letter;
	std::string code;

	Pair( int v , char l , std::string c )
	{
		val = v;
		letter = l;
		code = c;
	}

	void print() const
	{
		std::cout << letter << "\n";
	}

	bool operator ==( const Pair &other ) const
	{
		return this->val == other.val;
	}

	bool operator >( const Pair &other ) const
	{
		return this->val > other.val;
	}

	bool operator <( const Pair &other ) const
	{
		return other > *this;
	}
};

// A basic node
struct Node
{
	Pair value;
	Node *left;
	Node *right;

	Node( int v , char l , std::string c ) : value( v , l , c )
	{
		left = nullptr;
		right = nullptr;
	}

	Node( Pair p ) : value( p )
	{
		left = nullptr;
		right = nullptr;
	}
};

struct Tree
{
	Node root;

	// 2000 = Magic Number = Greater than 'e' (1222) but lesser than 't' (3222)
	Tree() : root( 2000 , 0 , "" ) {}

	void add( Pair obj )
	{
		Node *current = &root;
		while( true )
		{
			Node **target = nullptr;

			if( obj > current->value ) // Insert at right.
			{
				target = &current->right;
			}
			else // Insert at left.
			{
				target = &current->left;
			}

			if( !( *target ) ) // The node was empty (available).
			{
				*target = new Node( obj );
				break ;
			}

			current = *target; // Go ahead, again!.
		}
	}

	const Pair* find( Pair obj ) const
	{
		const Node *current = &root;
		while( true )
		{
			if( !current ) // Node is null?
			{
				return nullptr;
			}

			if( obj == current->value ) // Are equals? if so, return current.
			{
				return &current->value;
			}

			if( obj > current->value ) // Search for the right.
			{
				current = current->right;
			}
			else // Search for the left.
			{
				current = current->left;
			}
		}
	}

	void printRoot() const
	{
		printf( "Me is: Root\n" );
		printf( "\tMy left is: %c\n" , root.left->value.letter );
		printf( "\tMy my right is: %c\n" , root.right->value.letter );
		print( root.left );
		print( root.right );
	}

	void print( Node *n ) const
	{
		printf( "Me is: %c\n" , n->value.letter );

		if( n->left )
		{
			printf( "\tMy left is %c\n" , n->left->value.letter );
		}
		if( n->right )
		{
			printf( "\tMy right is %c\n" , n->right->value.letter );
		}

		if( n->left )
		{
			print( n->left );
		}
		if( n->right )
		{
			print( n->right );
		}
	}
};

std::unordered_map< char , std::string > lettersToCode;


/* Convert Morse codes to Numbers 
   This function assumes that:
   1) Max code length is 4.
   2) Min code length is 1.
   3) Dots (.) < Blanks (X) < UnderScores (_) 
    1 < 2 < 3 => where: Dots = 1, Blanks = 2, UnderScores = 3.
   Filling the resting digits with X so:
   e = .   = .XXX = 1222
   a = .-  = .-XX = 1322
   u = ..- = ..-X = 1132
   o = ___ = ___X = 3331
   And so on.
   
   Whit this method we keep the SAME order as using dots and underscores, and gain a LOT of performance
   because comparing numbers is always equal or faster (commonly faster) than comparing strings.
*/
int MorseToValue( const std::string &code )
{
	static const std::string dot( "." );

	{ // Sanity check scope.
		const size_t size = code.size();
		if( size < 1 || size > 4 )
			return 0;
		for( const auto &c : code )
		{
			if( !( c == '.' || c == '_' ) ) // Different from those characters?
			{
				return 0;
			}
		}
	}

	std::string v = "";

	for( int i = 0 ; i < code.size() ; ++i )
	{
		if( code.at( i ) == dot.at( 0 ) ) // Is a dot?
			v += "1";
		else // Is an underscore?
			v += "3";
	}

	const size_t nonDefined = 4 - code.size();

	// Blanks digits (those are only present at last).
	for( int i = 0 ; i < nonDefined ; ++i )
	{
		v += "2";
	}

	return std::stoi( v ); // Convert the string to a number.
}

// Convert Morse code to A-Z letters.
std::string getLettersFromMorse( const Tree &tree , std::string &input )
{
	std::vector< std::string > codes;

	{ // Create a scope.
		input = std::regex_replace( input , std::regex( "  " ) , "+" ); // Replace the double space with a + symbol.
		std::string code = "";
		for( const auto &c : input )
		{
			if( c == ' ' || c == '+' ) // is space?
			{
				codes.push_back( code ); // Save the previous string.
				code = ""; // Reset the string.

				if( c == '+' ) // Interpret the + symbol as a whitespace.
				{
					codes.push_back( " " );
				}
			}
			else
			{
				code += c;
			}
		}
		codes.push_back( code ); // Store the last code as well.
	}

	std::string message = "";

	for( const auto &code : codes )
	{
		if( code == " " ) // Is extra space?
		{ 
		   message += " " ;
		   continue;
		}
		const Pair *p = tree.find( Pair( MorseToValue( code ) , 0 , code ) );
		if( !p )
		{
			return std::string( "Your text have an invalid code" );
		}
		message += p->letter;
	}

	return message;
}

// Convert A-Z letters to Morse code.
std::string getMorseFromLetters( const std::string &input )
{
	std::string message = "";
	for( const auto &character : input )
	{
	    if( character == ' ' ) // Whitespace? Add it as well.
		{
		   message += ' ' ; 
		   continue;
		}
		char c = std::tolower( character );
		if( c < 'a' || c > 'z' ) // Character out of bounds?
		{
			message = "The message have invalid characters!";
			break ;
		}
		message += lettersToCode[ c ] + " ";
	}
	return message;
}

int main()
{
	std::ifstream input( "morse.txt" ); // Open the file with the letters-Morse code.

	std::array< std::vector< Pair > , 4 > arrVecCodes;
	// #0 --> Vector with 1 length codes.
	// #1 --> Vector with 2 length codes. etc etc..
	// This will allow us to build of tree balanced.	

	// Decode the file.
	for( std::string line ; getline( input , line ) ; )
	{
		char letter = line.at( 0 );
		std::string code = line.substr( 1 );

		// Assuming there is a SINGLE <letter,code> per line.
		lettersToCode[ letter ] = code;

		int value = MorseToValue( code );

		arrVecCodes[ code.size() - 1 ].push_back( Pair( value , letter , code ) );
	}

	Tree tree; // Create the Search Binary Tree.

	for( const auto &vec : arrVecCodes )
	{
		for( const auto val : vec )
		{
			tree.add( val ); // Add all the letters into the tree.
		}
	}

	// tree.printRoot(); // For debugging, exactly the same tree as the paper shows.

	std::string instructions = STRINGIFY
	(
		Usage:
		\n1. For Morse code to A-Z letters.
		\n2. A-Z letters to Morse code.
		\n3. Finish the program.
		\nNote: Use spaces as delimiters, hit enter when input done.
		\nYour option is:
	);

	// A pretty lambda function.
	auto Input = []( std::string &input , const std::string &message )
			{
				std::cin.ignore();
				std::cout << "Your input:" << std::endl;
				std::getline( std::cin , input );
				std::cout << message << std::endl;
			};

	while( true )
	{
		int option = 0;
		std::cout << instructions;
		std::cin >> option;

		std::string userInput;

		switch( option )
		{
			case 1 :
				Input( userInput , "A-Z is:" );
				std::cout << getLettersFromMorse( tree , userInput ) << std::endl;
				break ;
			case 2 :
			{
				Input( userInput , "Morse code is:" );
				std::cout << getMorseFromLetters( userInput ) << std::endl;
				break ;
			}
			case 3 : // Finishes the program.
				return 0;
			default : // Wrong option.
				std::cin.clear(); // In order to correctly handle non-numbers input.
				std::cin.ignore( std::numeric_limits< std::streamsize >::max() , '\n' );
				std::cout << "Wrong option, try again!!!" << std::endl;
				break ;
		}
	}

	getchar();
	return 0;
}
