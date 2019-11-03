# XDelta Wii

This is an attempt to create a homebrew app for the Nintendo Wii that can patch dumped games, without the need for a computer.

## Requirements

* Wii cable of homebrew
* An original dump of your source game (e.g. from CleanRip)
	* Place on the root of your SD/USB device named `game.iso`
* Your xDelta-format romhack / patch file
	* On the root of your SD/USB device, named `patch.xdelta`
* SD Card or USB drive with enough space for the original game, patch, and output files
	* Must be formatted to FAT32 (NTFS support coming soon...)
	* GameCube games, 4 GB or larger is recommended
		* 1.36 GB for game.iso (original game dump)
		* 1.36 GB for patched.iso (patched output file)
		* ??? MB for patch.xdelta (the romhack / patch file)
	* __Coming Soon:__ Wii games (single-layer disc), 16 GB or larger is recommended
		* 4.7 GB for game.iso (original game dump)
		* 4.7 GB for patched.iso (patched output file)
		* ??? GB for patch.xdelta (the romhack / patch file)
	* __Coming Soon:__ Wii games (dual-layer disc, e.g., _Super Smash Bros. Brawl_), 32 GB or larger is recommended
		* 8.5 GB for game.iso (original game dump)
		* 8.5 GB for patched.iso (patched output file)
		* ??? GB for patch.xdelta (the romhack / patch file)

## To Do

* Actually implement the xDelta library functions
* Add NTFS Support (required for patching Wii ISO files as they exceed the 4GB FAT file size limit)
* Port to GameCube
	* Change WPAD to PAD inputs
	* Replace Front SD / USB with SD Gecko

# Xdelta

Xdelta version 3 is a C library and command-line tool for delta
compression using VCDIFF/RFC 3284 streams.

# License

This repository contains branches of Xdelta 3.x that were
re-licensed by the original author under the [Apache Public
License version 2.0](http://www.apache.org/licenses/LICENSE-2.0),
namely:

- __release3_0_apl__ Change to APL based on 3.0.11 sources
- __release3_1_apl__ Merges release3_0_apl with 3.1.0 sources

The original GPL licensed Xdelta lives at http://github.com/jmacd/xdelta-gpl.
