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
#include "Socket.h"
#include "Logger.h"
#include "Server.h"
#include "Mailer.h"

using namespace lep;

namespace dispatch {
enum
{
  kServerSocketBacklog = 10
};

Server::Server(int port)
  : server_socket_(Socket::kInternet, Socket::kStream, Socket::kTcp),
    port_(port),
    shutdown_(false),
    listener_(0)
{
}

Server::~Server()
{
}

void Server::Start()
{
  LOG_D("Creating Dispatch server socket on port %d\n", port_);
  server_socket_.SetOptionReuse();

  LOG_D("Binding TCP server socket\n");
  SocketAddress server_address(*SocketAddress::ForLoopback(port_));
  server_socket_.Bind(server_address);
  server_socket_.Listen(kServerSocketBacklog);

  while((!shutdown_))
  {
    SmartPointer<Socket> client_socket = server_socket_.Accept(kServerSocketBacklog);

    LOG_D("Inbound Connect from  %s, port %d\n",
      client_socket->remote_address().address().toCharArray(), client_socket->remote_address().port());

    if (client_socket != NULL) {
      if (listener_ != NULL) {
        SmartPointer<Mailer> mailer = new Mailer(client_socket);
        listener_->OnClientConnected(mailer);
      }
      else {
        LOG_W("No registered listener. Closing connection.");
        client_socket->Close();
      }
    }
    else {
      LOG_E("Accept failed with error %d\n", server_socket_.GetLastError());
      continue;
    }
  }

  // server should be shutdown only after all the client requests have been completed.
  //server_socket_.Shutdown();
  server_socket_.Close();
}

void Server::Stop()
{
  shutdown_ = true;
  // next call shutdown on the server socket.
  // this will call it to abort the accept. this means, however,
  // that it is important to handle failure return from accept.
  server_socket_.Shutdown(Socket::kBoth);
}

void Server::AddListener(SmartPointer<ServerListener> listener)
{
  listener_ = listener;
}
} // namespace dispatch

