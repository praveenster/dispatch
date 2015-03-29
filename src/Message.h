/*
  Copyright (c) 2011 Neevarp Yhtroom

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DISPATCH_MESSAGE_H_
#define	DISPATCH_MESSAGE_H_

#include "String.h"

namespace dispatch {
class Message
{
public:
  virtual void Deserialize(char* buffer, int& length);
  virtual void Serialize(char* buffer, int& length);
  virtual int GetSerializedSize();

  void DeserializeHeader(char* buffer, int length);
  int GetHeaderSize();

	short int type();
	short int version();
	int length();
	char* body();

  Message(int type);
  virtual ~Message();

protected:
  void SerializeByte(char* buffer, int& length, unsigned char value);
  void SerializeShortInt(char* buffer, int& length, short int value);
  void SerializeInt(char* buffer, int& length, int value);
  void SerializeUint(char* buffer, int& length, unsigned int value);
  void SerializeBuffer(char* buffer, int& length, const char* value, int count);
  void SerializeString(char* buffer, int& length, const lep::String& value);

  short int DeserializeByte(char* buffer, int& length);
  short int DeserializeShortInt(char* buffer, int& length);
  int DeserializeInt(char* buffer, int& length);
  unsigned int DeserializeUint(char* buffer, int& length);
  lep::String DeserializeString(char* buffer, int& length);
  void DeserializeBuffer(char* buffer, int& length, char* value, int count);

  short int type_;
  short int version_;
  int       length_;
  char*     body_;

private:
  Message(const Message&);
  Message& operator=(const Message&);
};
}
#endif // DISPATCH_MESSAGE_H_

