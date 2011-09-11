/*
This file is part of OpenTexMod.


OpenTexMod is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenTexMod is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FooOpenTexMod.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "OTM_Main.h"

OTM_Client::OTM_Client( PipeStruct &pipe, OTM_Frame *frame) : wxThread(wxTHREAD_JOINABLE)
{
  Pipe.In = pipe.In;
  Pipe.Out = pipe.Out;
  MainFrame = frame;
}

OTM_Client::~OTM_Client(void)
{
  if (Pipe.Out != INVALID_HANDLE_VALUE)
  {
    DisconnectNamedPipe(Pipe.Out);
    CloseHandle(Pipe.Out);
    Pipe.Out = INVALID_HANDLE_VALUE;
  }
  if (Pipe.In != INVALID_HANDLE_VALUE)
  {
    DisconnectNamedPipe(Pipe.In);
    CloseHandle(Pipe.In);
    Pipe.In = INVALID_HANDLE_VALUE;
  }
}

void* OTM_Client::Entry(void)
{
  char buffer[BUFSIZE];
  while (1)
  {
    unsigned long size;
    bool ret = ReadFile(
             Pipe.In,        // handle to pipe
             buffer,    // buffer to receive data
             BUFSIZE, // size of buffer
             &size, // number of bytes read
             NULL);        // not overlapped I/O

    if (ret || GetLastError()==ERROR_MORE_DATA)
    {
      unsigned int pos=0;
      MsgStruct *commands;
      bool update_textures = false;
      while (pos<size-sizeof(MsgStruct))
      {
        commands = (MsgStruct*) &buffer[pos];
        //unsigned int add_length = 0;
        switch (commands->Control)
        {

        }
        pos+=sizeof(MsgStruct);// + add_length;
      }
    }
    else
    {
      break;
    }
  }
  CloseHandle(Pipe.In);
  Pipe.In = INVALID_HANDLE_VALUE;
  CloseHandle(Pipe.Out);
  Pipe.Out = INVALID_HANDLE_VALUE;

  OTM_Event event( OTM_EVENT_TYPE, ID_Delete_Game);
  event.SetClient(this);
  wxPostEvent( MainFrame, event);

  return NULL;
}

