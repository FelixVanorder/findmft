# findmft
Utility to find offsets to Master File Table records in NTFS file dump or partition.
Created to achive absent ability of http://thewalter.net/stef/software/scrounge/ tool of finding out "MFT Offset: The position of the NTFS Master File Table", as mentioned here: http://thewalter.net/stef/software/scrounge/guessing.html.

findmft in my 1TB NTFS dump recovering gives me a hundred guesses of locations of MFT with 90% of false-positive, but scrounge has quickly check it and restore all deleted data in acceptable time, so it's at least useful right now as is.

Compile with C++11-enabled compiler.