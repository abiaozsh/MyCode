#ifndef __SD_H__
#define __SD_H__

#include <Arduino.h>

#include <utility/SdFat.h>

#define FILE_READ O_READ
#define FILE_WRITE (O_READ | O_WRITE | O_CREAT)

class File : public Stream {
	private:
	char _name[13]; // our name
	SdFile *_file;  // underlying file pointer

public:
	File(SdFile f, const char *n) {
		// oh man you are kidding me, new() doesnt exist? Ok we do it by hand!
		_file = (SdFile *)malloc(sizeof(SdFile)); 
		if (_file) {
			memcpy(_file, &f, sizeof(SdFile));
    
			strncpy(_name, n, 12);
			_name[12] = 0;
		}
	}
	
	File(void) {
  _file = 0;
  _name[0] = 0;
  //Serial.print("Created empty file object");
}

size_t write(uint8_t val) {
  return write(&val, 1);
}
size_t write(const uint8_t *buf, size_t size) {
  size_t t;
  if (!_file) {
    setWriteError();
    return 0;
  }
  _file->clearWriteError();
  t = _file->write(buf, size);
  if (_file->getWriteError()) {
    setWriteError();
    return 0;
  }
  return t;
}
int read() {
  if (_file) 
    return _file->read();
  return -1;
}
int peek() {
  if (! _file) 
    return 0;

  int c = _file->read();
  if (c != -1) _file->seekCur(-1);
  return c;
}

int available() {
  if (! _file) return 0;

  uint32_t n = size() - position();

  return n > 0X7FFF ? 0X7FFF : n;
}
void flush() {
  if (_file)
    _file->sync();
}
// buffered read for more efficient, high speed reading
int read(void *buf, uint16_t nbyte) {
  if (_file) 
    return _file->read(buf, nbyte);
  return 0;
}
boolean seek(uint32_t pos) {
  if (! _file) return false;

  return _file->seekSet(pos);
}



uint32_t position() {
  if (! _file) return -1;
  return _file->curPosition();
}
  
uint32_t size() {
  if (! _file) return 0;
  return _file->fileSize();
}
void close() {
  if (_file) {
    _file->close();
    free(_file); 
    _file = 0;
  }
}
operator bool() {
  if (_file) 
    return  _file->isOpen();
  return false;
}

// returns a pointer to the file name
char* name(void) {
  return _name;
}


// a directory is a special type of file
boolean isDirectory(void) {
  return (_file && _file->isDir());
}
	void rewindDirectory(void) {  
	  if (isDirectory())
		_file->rewind();
	}

  using Print::write;
};

class SDClass {

private:
  // These are required for initialisation and use of sdfatlib
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  
public:
  // This needs to be called to set up the connection to the SD card
  // before other methods are used.
boolean begin() {
  return card.init() && volume.init(card) && root.openRoot(volume);
}

File openSimple(const char *filepath, uint8_t mode, uint8_t toend) {
  // Open the file itself
  SdFile file;

	if ( ! file.open(root, filepath, mode)) {
	  // failed to open the file :(
	  return File();
	}

  if (toend) 
    file.seekSet(file.fileSize());
  return File(file, filepath);
}

private:
  
  friend class File;
};

extern SDClass SD;

#endif
