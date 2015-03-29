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
#include <cstdio>
#include <cstring>
#include "Message.h"

using lep::String;

namespace dispatch {
Message::Message(int type)
: type_(type), version_(0), length_(0), body_(0)
{
}

Message::~Message()
{
  delete [] body_;
}

void Message::SerializeByte(char* buffer, int& length, unsigned char value)
{
  buffer[0] = (char)value;
  length += 1;
}

void Message::SerializeShortInt(char* buffer, int& length, short int value)
{
  buffer[0] = (char) (value & 0xff);
  buffer[1] = (char) ((value & 0xff00) >> 8);
  length += 2;
}

void Message::SerializeInt(char* buffer, int& length, int value)
{
  buffer[0] = (char) (value & 0xff);
  buffer[1] = (char) ((value & 0xff00) >> 8);
  buffer[2] = (char) ((value & 0xff0000) >> 16);
  buffer[3] = (char) ((value & 0xff000000) >> 24);
  length += 4;
}

void Message::SerializeUint(char* buffer, int& length, unsigned int value)
{
  buffer[0] = (char) (value & 0xff);
  buffer[1] = (char) ((value & 0xff00) >> 8);
  buffer[2] = (char) ((value & 0xff0000) >> 16);
  buffer[3] = (char) ((value & 0xff000000) >> 24);
  length += 4;
}

void Message::SerializeBuffer(char* buffer, int& length, const char* value, int count)
{
  memcpy(buffer, value, count);
  length += count;
}

void Message::SerializeString(char* buffer, int& length, const String& value)
{
  int temp = value.length();
  SerializeBuffer(buffer, length, value.toCharArray(), temp);
  buffer[temp] = '\0'; // include '\0'
  length++;
}

void Message::Serialize(char* buffer, int& length)
{
  // initialize the length. increment as variables are serialized.
  length = 0;
  SerializeShortInt(buffer, length, type_);
  SerializeShortInt(buffer + length, length, version_);
  SerializeInt(buffer + length, length, length_);

  if (length_) {
    SerializeBuffer(buffer + length, length, body_, length_);
  }
}

int Message::GetSerializedSize()
{
  return sizeof(type_) + sizeof(version_) + sizeof(length_) + length_;
}

short int Message::DeserializeByte(char* buffer, int& length)
{
  length += 1;
  unsigned char* ubuffer = (unsigned char*)buffer;
  return ubuffer[0];
}

short int Message::DeserializeShortInt(char* buffer, int& length)
{
  length += 2;
  unsigned char* ubuffer = (unsigned char*)buffer;
  return (ubuffer[1] << 8) | (ubuffer[0]);
}

int Message::DeserializeInt(char* buffer, int& length)
{
  length += 4;
  unsigned char* ubuffer = (unsigned char*)buffer;
  return (ubuffer[3] << 24) | (ubuffer[2] << 16) | (ubuffer[1] << 8) | (ubuffer[0]);
}

unsigned int Message::DeserializeUint(char* buffer, int& length)
{
  length += 4;
  unsigned char* ubuffer = (unsigned char*)buffer;
  return (ubuffer[3] << 24) | (ubuffer[2] << 16) | (ubuffer[1] << 8) | (ubuffer[0]);
}

String Message::DeserializeString(char* buffer, int& length)
{
  String value(buffer);
  length = value.length() + 1; // for null terminator;
  return value;
}

void Message::DeserializeBuffer(char* buffer, int& length, char* value, int count)
{
  memcpy(buffer, value, count);
  length += count;
}

void Message::Deserialize(char* buffer, int& length)
{
  length = 0;
  type_ = DeserializeShortInt(buffer, length);
  version_ = DeserializeShortInt(buffer + length, length);
  length_ = DeserializeInt(buffer + length, length);

  if (length_) {
    body_ = new char[length_];
    DeserializeBuffer(buffer + length, length, body_, length_);
  }
}

void Message::DeserializeHeader(char* buffer, int length)
{
  int temp = 0;
  type_ = DeserializeShortInt(buffer, temp);
  version_ = DeserializeShortInt(buffer + temp, temp);
  length_ = DeserializeInt(buffer + temp, temp);
  delete[] body_;
  if (length_ > 0) {
  	body_ = new char[length_];
  }
  else {
  	body_ = 0;
  }
}

short int Message::type()
{
	return type_;
}

short int Message::version()
{
	return version_;
}

int Message::length()
{
	return length_;
}

char* Message::body()
{
	return body_;
}

int Message::GetHeaderSize()
{
	return sizeof(type_) + sizeof(version_) + sizeof(length_);
}
} // namespace dispatch

