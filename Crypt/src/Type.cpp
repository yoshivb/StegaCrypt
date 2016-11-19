#include "Type.h"
#include <algorithm>

int ParseSquareFile( std::string& a_squareFilePath, PolybiusSquare& a_squareOut )
{
	std::ifstream squareFile(a_squareFilePath.c_str());

	uint size = -1;
	Coords coords;
	coords.y = 0;

	std::string line;
	while (getline(squareFile, line))
	{
		std::istringstream iss(line);
		
		if ( size == -1 )
			size = line.size();
		else if ( line.size() != size )
			return -1;

		for ( coords.x = 0; coords.x < line.size(); coords.x++ )
			a_squareOut.insert( PolybiusSquarePair( line[coords.x], coords) );

		coords.y++;
	}

	return ( coords.y == size ) ? size : -1;
}

PolybiusSquare::iterator FindByCoords( PolybiusSquare& a_square, Coords a_coords )
{
	return find_if(begin(a_square), end(a_square), [&](const PolybiusSquarePair &pair)
	{
		return pair.second.x == a_coords.x && pair.second.y == a_coords.y;
	});
}