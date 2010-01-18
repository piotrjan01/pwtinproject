/*
 * FileOperations.h
 *
 *  Created on: Jan 18, 2010
 *      Author: piotrrr
 */

#ifndef FILEOPERATIONS_H_
#define FILEOPERATIONS_H_

#include <string>
#include "../Main/Main.h"

class FileOperations {
public:
	static void writeToFile(string fname, char* data, int dataSize) {
		FILE *fp = 0;
		fp = fopen(fname.c_str(), "a+");
		if ( ! fp ) Main::getInstance()->handleError("Couldn't open file to write: "+fname);
		fwrite(data, sizeof(char), dataSize, fp);
	}

};

#endif /* FILEOPERATIONS_H_ */
