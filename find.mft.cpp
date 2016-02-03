#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdint.h>

int main( int ac, char * av[] )
{
	if( ac < 2 )
	{
		std::cerr << "usage: " << *av << " file-or-device-path" << std::endl;
		return 1;
	}
	std::ifstream ifs( *(av+1), std::ios::binary);
	if( !ifs )
	{
		std::cerr << "can't open file or device path: \"" << *(av+1) << "\"" << std::endl;
		return 2;
	}
	ifs.exceptions( /* std::ios::failbit | */ // otherwise ifs.read 1k file into 1M buffer throw exception
			std::ios::badbit
		);

	enum { bufsize = 2 * 1024 * 1024 };
	std::string buf;
	buf.resize( bufsize );

	const char mft_signature[] = { 0x24, 0x00, 0x4d, 0x00, 0x46, 0x00, 0x54 };
	enum { sig_align = 16 };
	static_assert( ( bufsize % sig_align ) == 0, "bad signature align or buffer size" );

	uint64_t filepos = 0;
	uint64_t prev_match_sector = -1LL;
	while( ifs )
	{
		ifs.read( &buf[0], buf.size() );
		const size_t readbytes = ifs.gcount();
		if( ( readbytes % sig_align ) != 0 and not ifs.eof() )
		{
			std::cerr << "can't read block of desired size from file" << std::endl;
			return 3;
		}

		for( size_t offset = 0; offset < ( readbytes - sizeof(mft_signature) ); offset += 1 ) //+= sig_align ) // doesn't work somewhy...
		{
			if( 0 == std::memcmp( &buf[offset], mft_signature, sizeof(mft_signature) ) )
			{
				const uint64_t sector = ( filepos + offset ) / 512;
				if( sector != prev_match_sector )
				{
					std::cout << sector << std::endl;
					prev_match_sector = sector;
				}
			}
		}
		std::cerr << '.';
		filepos += readbytes;
	}

	std::cerr << "done" << std::endl;

	return 0;
}
