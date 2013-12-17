//start of ZipPipe.c file

// to bulid the sample type cl ZipPipe.c /link zlib.lib

 

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

#include <windows.h>

#include <string.h>

#include <stdio.h>

#include "zlib.h"                  // zlib library

 

enum {nBytesToRead=8192};           //Read-Write in 8 Kb chunks

const InBufferSize=131072;                //128 Kbfor Input Buffer

const OutBufferSize=131072;             //128 Kbfor Input Buffer

 

int main(int argc, char *argv[])

{

    HANDLE                                           hPipe;

    char                                    inBuffer [nBytesToRead];

    char                                    spipearg[50],spipe[300];

    char                                    sfile[300];

    char                                    scompress[5];

    unsigned long                  nBytesRead,bytesTransferred;

    int                                                       bResult,ncompression,badParm=TRUE,completionCode;

    gzFile                                  fgzh;

 

    if (argc == 4)

    {

                                sscanf (argv[1], "%s", spipearg);

                                sscanf (argv[2], "%s", sfile);

                                sscanf (argv[3], "%d", &ncompression);

                                badParm=FALSE;

    }

    sprintf(scompress,"wb%d",ncompression);

 

    if (badParm)

    {

        printf ("usage: ZipPipe in_pipe_name compressed_out_file compressionlevel  \n"

            "Creates a pipe,listens on it and compress what other process writes on it.\n");

        return (1);

    }

    strcpy(spipe,"\\\\.\\pipe\\");

    strcat(spipe,spipearg);

    hPipe = CreateNamedPipe(spipe,

          PIPE_ACCESS_DUPLEX, PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,

          OutBufferSize, InBufferSize, 1000, NULL);

 

    printf("\nListening on pipe %s and writing to file %s with compression level %d… \n",spipe,sfile,ncompression);

    ConnectNamedPipe(hPipe, NULL);

    fgzh = gzopen (sfile, scompress);

 

    if (fgzh == NULL )

    {

        printf ("Failed to open: %s\n", sfile);

        return (1);

    }

 

    completionCode=0;

    bResult=1;

    nBytesRead=1;

    while  (bResult &&  nBytesRead != 0  && completionCode==0)

    {

                bResult = ReadFile(hPipe, &inBuffer, nBytesToRead, &nBytesRead, NULL) ;

                bytesTransferred = gzwrite (fgzh,&inBuffer,nBytesRead);

                if (bytesTransferred != nBytesRead )

                {

                                completionCode = 1;

                }

    }

    gzflush (fgzh,Z_SYNC_FLUSH);

    gzclose (fgzh);

    DisconnectNamedPipe(hPipe);

    CloseHandle(hPipe);

    return(0);

}

//end  of ZipPipe.c file


//start of UnZipPipe.c file

// to bulid the sample type cl UnZipPipe.c /link zlib.lib

 

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

 

#include <windows.h>

#include <string.h>

#include <stdio.h>

#include "zlib.h"                  // zlib library

 

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

    gzFile                                  fgzh;

 

    if (argc == 3)

    {

                                sscanf (argv[1], "%s", spipearg);

                                sscanf (argv[2], "%s", sfile);

                                badParm=FALSE;

    }

 

    if (badParm)

    {

        printf ("usage: UnZipPipe out_pipe_name compressed_in_file  \n"

            "Reads and uncompres the content of a compressed file and writes it to a pipe.\n");

        return (1);

    }

    strcpy(spipe,"\\\\.\\pipe\\");

    strcat(spipe,spipearg);

    hPipe = CreateNamedPipe(spipe,

          PIPE_ACCESS_DUPLEX, PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,

          OutBufferSize, InBufferSize,1000, NULL);

 

    printf("\nReading from file %s and writing to pipe %s …\n",sfile,spipe);

    fgzh = gzopen (sfile, "rb");

 

    if (fgzh == NULL )

    {

        printf ("Failed to open: %s\n", sfile);

        return (1);

    }

 

    ConnectNamedPipe(hPipe, NULL);

 

    completionCode=0;

    bResult=1;

    nBytesRead=1;

    while  (bResult &&  nBytesRead != 0  && completionCode==0)

    {

                nBytesRead = gzread(fgzh, inBuffer, nBytesToRead) ;

                WriteFile (hPipe,inBuffer,nBytesRead,&bytesTransferred,NULL);

                if (nBytesRead ==0)

                {

                                bResult = 0;

                }

    }

    gzclose (fgzh);

    FlushFileBuffers(hPipe);

    CloseHandle(hPipe);

    DisconnectNamedPipe(hPipe);

    return(0);

}

//end of UnZipPipe.c file
