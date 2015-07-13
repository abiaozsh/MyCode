#include "SD.h"

#define MAX_COMPONENT_LEN 12 // What is max length?
#define PATH_COMPONENT_BUFFER_LEN MAX_COMPONENT_LEN+1

boolean SDClass::begin() {
  return card.init() && volume.init(card) && root.openRoot(volume);
}

File SDClass::openSimple(const char *filepath, uint8_t mode, uint8_t toend) {
  // Open the file itself
  SdFile file;

	if ( ! file.open(SD.root, filepath, mode)) {
	  // failed to open the file :(
	  return File();
	}

  if (toend) 
    file.seekSet(file.fileSize());
  return File(file, filepath);
}

void File::rewindDirectory(void) {  
  if (isDirectory())
    _file->rewind();
}

SDClass SD;
