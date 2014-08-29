/* Copyright (C) 2000-2012 by George Williams */
/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.

 * The name of the author may not be used to endorse or promote products
 * derived from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "gimage.h"
#include "string.h"

static char *pixname(int i, int ncol) {
    static char one[2], two[3];
    char *usable = "!#$%&'()*+,-./0123456789;:<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~ ";
    static int len=0;

    if ( len==0 ) len = strlen(usable);
    if ( ncol<len ) {
	one[0]=usable[i];
return( one );
    } else {
	two[0] = usable[i/len];
	two[1] = usable[i%len];
return( two );
    }
}

int GImageWriteXpm(GImage *gi, char *filename) {
    struct _GImage *base = gi->list_len==0?gi->u.image:gi->u.images[0];
    FILE *file;
    char stem[256];
    char *pt; uint8 *scanline;
    int i,j;

    if ( base->image_type!=it_index )
return(false );

    if (( pt = strrchr(filename,'/'))==NULL )
	strcpy(stem,filename);
    else
	strcpy(stem,pt+1);
    if ( (pt = strchr(stem,'.'))!=NULL )
	*pt = '\0';

    if ((file=fopen(filename,"w"))==NULL )
return(false);

    fprintf(file,"/* XPM */\n" );
    fprintf(file,"static char *%s[] =\n", stem );
    fprintf(file,"{\n");
    fprintf(file,"/* width height ncolors chars_per_pixel */\n");
    fprintf(file,"\"%d %d %d %d\"\n", (int) base->width, (int) base->height, base->clut->clut_len,
	    base->clut->clut_len>95?2:1 );
    fprintf(file,"/* colors */\n");
    for ( i=0; i<base->clut->clut_len; ++i )
	fprintf(file,"\"%s c #%06x\"\n", pixname(i,base->clut->clut_len),
		(int) base->clut->clut[i]);
    fprintf(file,"/* image */\n");
    for ( i=0; i<base->height; ++i ) {
	fprintf(file,"\"" );
	scanline = base->data + i*base->bytes_per_line;
	for ( j=0; j<base->width; ++j )
	    fprintf(file,"%s", pixname(*scanline++,base->clut->clut_len));
	fprintf(file,"\"%s\n", i==base->height-1?"":"," );
    }
    fprintf(file,"};\n" );
    fflush(file);
    i = ferror(file);
    fclose(file);
return( i );
}
