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
#include "Mailer.h"
#include "Logger.h"

using namespace lep;

namespace dispatch {
Mailer::Mailer(SmartPointer<Socket> socket)
: socket_(socket)
{
}

Mailer::~Mailer()
{
  socket_ = 0;
}

void Mailer::Send(Message& message)
{
  int length = message.GetSerializedSize();
  if ((socket_!= 0) && (length != 0)) {
    char* buffer = new char[length];
    message.Serialize(buffer, length);
    socket_->Send(buffer, length, 0);
    delete[] buffer;
  }
}

void Mailer::Receive(Message& message)
{
  if (socket_ != 0) {
    int header_size = message.GetHeaderSize();

    char buffer[header_size];
    bool ok = true;
    int result;

    for (int i = 0; i < header_size; i++) {
      result = socket_->Receive(buffer + i, sizeof(char), 0);
      if ((result == 0) || (result == -1)) {
        ok = false;
        break;
      }
    }

    if (ok) {
      message.DeserializeHeader(buffer, header_size);
      int length = message.length();

      if (length > 0) {
        int offset = 0;
        char* body = message.body();

        do {
          result = socket_->Receive((char*)(body + offset), length - offset, 0);
          if ((result == 0) || (result == -1)) {
            ok = false;
            break;
          }

          offset += result;

          if (offset == length) {
            ok = true;
            break;
          }
        }
        while(true);
      }
    }
    else {
      LOG_E("Error receving data. Not ok! Socket error = %d\n", socket_->GetLastError());
    }
  }
}

} // namespace dispatch

