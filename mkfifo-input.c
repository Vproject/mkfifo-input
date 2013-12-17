/*

Copyright notice

================

 (C) 1995-2005

 

  This software is provided ‘as-is’, without any express or implied

  warranty.  In no event will the author be held liable for any damages

  arising from the use of this software.

 

  Permission is granted to anyone to use this software for any purpose,

  including commercial applications, and to alter it and redistribute it

  freely, subject to the following restrictions:

 

  1. The origin of this software must not be misrepresented; you must not

     claim that you wrote the original software. If you use this software

     in a product, an acknowledgment in the product documentation would be

     appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be

     misrepresented as being the original software.

  3. This notice may not be removed or altered from any source distribution.

 

*/

/*
 * Original: http://jcarlossaez1.wordpress.com/2005/10/24/ii-on-the-fly-compression-is-easy-on-unix-but-it-is-also-easy-on-windows/
 * modified by V (December 2013) to read from stdin instead of a compressed file
 */

 

#include <windows.h>

#include <string.h>

#include <stdio.h>

#include <fcntl.h> 
 

enum {nBytesToRead =    8192};      //Read-Write in 8 Kb chunks

const InBufferSize = 131072;              //128 Kb for Input Buffer

const OutBufferSize=131072;             //128 Kb for Input Buffer

int main(int argc, char *argv[])

{

    HANDLE                                           hPipe;

    char                                    inBuffer [nBytesToRead];

    char                                    spipearg[50],spipe[300];

    char                                    sfile[300];

    unsigned long                  nBytesRead,bytesTransferred;

    int                                                       bResult,badParm=TRUE,completionCode;

    FILE * fgzh;

 

    if (argc == 2)

    {

                                sscanf (argv[1], "%s", spipearg);


                                badParm=FALSE;

    }

 

    if (badParm)

    {

        printf ("usage: ... | mkfifo-input out_pipe_name\n"

            "Reads stdin and writes it to a pipe. Msys tools seem to have trouble open the pipe. Other mingw compiled tools don't seem to suffer from that. Note: backslashes may require escaping; \\\\.\\pipe\\ vs \\\\\\\\.\\\\pipe\\\\\n");

        return (1);

    }

    strcpy(spipe,"\\\\.\\pipe\\");

    strcat(spipe,spipearg);
    strcat(spipe,"");

    hPipe = CreateNamedPipe(spipe,

          PIPE_ACCESS_DUPLEX, PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,

          OutBufferSize, InBufferSize,1000, NULL);

 

    printf("\nReading from stdin and writing to pipe %s\n",spipe);

    if( _setmode(_fileno( stdin ),  _O_BINARY ) != -1 )
		fgzh = stdin;
	else
		fgzh = NULL;

 

    if (fgzh == NULL )

    {

        printf ("Failed to open stdin\n");

        return (1);

    }

 

    ConnectNamedPipe(hPipe, NULL);

 

    completionCode=0;

    bResult=1;

    nBytesRead=1;

    while  (bResult &&  nBytesRead != 0  && completionCode==0)

    {

                nBytesRead = fread(inBuffer, 1, nBytesToRead, fgzh) ;

                WriteFile (hPipe,inBuffer,nBytesRead,&bytesTransferred,NULL);

                if (nBytesRead ==0)

                {

                                bResult = 0;

                }

    }

    fclose(fgzh);

    FlushFileBuffers(hPipe);

    CloseHandle(hPipe);

    DisconnectNamedPipe(hPipe);

    return(0);

}
