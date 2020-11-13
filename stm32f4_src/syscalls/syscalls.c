/**************************************************************************//*****
 * @file     stdio.c
 * @brief    Implementation of newlib syscall
 ********************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

#undef errno
extern int errno;
extern int _end;

extern int _eccram;

static char * ccsbrk(int incr) {
	static unsigned char *ccheap = NULL;
	unsigned char *prev_heap;

	if (ccheap == NULL) {
		ccheap = (unsigned char *)  &_eccram;
	}
	prev_heap = ccheap;

	ccheap += incr;

	return (caddr_t) prev_heap;
}

typedef long Align;

union header { /* Kopf eines Allokationsblocks */
	struct {
		union header *ptr; /* Zeiger auf zirkulaeren Nachfolger */
		unsigned size; /* Groesse des Blocks	*/
	} s;
	Align x; /* Erzwingt Block-Alignierung	*/
};

typedef union header Header;

static Header base; /* Anfangs-Header	*/
static Header *freep = NULL; /* Aktueller Einstiegspunkt in Free-Liste */
static Header *ccmorecore(unsigned); /* Aufruf ans Betriebssystem */

void* cc_malloc(unsigned nbytes) {
	Header *p, *prevp;
	unsigned nunits;

	/* Kleinstes Vielfaches von sizeof(Header), das die
	 geforderte Byte-Zahl enthalten kann, plus 1 fuer den Header selbst: */

	nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

	if ((prevp = freep) == NULL) { /* Erster Aufruf, Initialisierung */
		base.s.ptr = freep = prevp = &base;
		base.s.size = 0; /* base wird Block der Laenge 0 */
	}
	for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr) {

		/* p durchlaeuft die Free-Liste, gefolgt von prevp, keine
		 Abbruchbedingung!!	*/

		if (p->s.size >= nunits) { /* Ist p gross genug? 		*/
			if (p->s.size == nunits) /* Falls exakt, wird er... 	*/
				prevp->s.ptr = p->s.ptr;/* ... der Liste entnommen 	*/
			else { /* andernfalls...	   	*/
				p->s.size -= nunits; /* wird p verkleinert		*/
				p += p->s.size; /* und der letzte Teil ... 	*/
				p->s.size = nunits; /* ... des Blocks...		*/
			}
			freep = prevp;
			return (void*) (p + 1); /* ... zurueckgegeben, allerdings
			 unter der Adresse von p+1,
			 da p auf den Header zeigt.  	*/
		}
		if (p == freep) /* Falls die Liste keinen Block
		 ausreichender Groesse enthaelt,
		 wird morecore() aufgrufen	*/
			if ((p = ccmorecore(nunits)) == NULL)
				return NULL;
	}
}

#define NALLOC 	1024	/* Mindestgroesse fuer morecore()-Aufruf	*/

/* Eine static-Funktion ist ausserhalb ihres Files nicht sichtbar	*/
static Header *ccmorecore(unsigned nu) {
	char *cp;
	void free(void*);
	Header *up;
	if (nu < NALLOC)
		nu = NALLOC;
	cp = ccsbrk(nu * sizeof(Header));
	if (cp == (char *) -1) /* sbrk liefert -1 im Fehlerfall */
		return NULL;
	up = (Header*) cp;
	up->s.size = nu; /* Groesse wird eingetragen	*/
	cc_free((void*) (up + 1)); /* Einbau in Free-Liste		*/
	return freep;
}

void cc_free(void *ap) { /* Rueckgabe an Free-Liste	*/
	Header *bp, *p;

	if( ap == NULL ) return;

	bp = (Header*) ap - 1; /* Hier ist der Header des Blocks */

	/* Die Liste wird durchmustert, der Block soll der
	 Adressgroesse nach richtig eingefuegt werden,
	 um Defragmentierung zu ermoeglichen.				*/

	for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break; /* bp liegt vor Block mit kleinster oder hinter
			 Block mit groesster Adresse */

	if (bp + bp->s.size == p->s.ptr) {
		/* Vereinigung mit oberem Nachbarn 	*/
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else
		bp->s.ptr = p->s.ptr;

	if (p + p->s.size == bp) {
		/* Vereinigung mit unterem Nachbarn 	*/
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else
		p->s.ptr = bp;

	freep = p;
}

/*This function is used for handle heap option*/
__attribute__ ((used))
caddr_t _sbrk(int incr) {
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;

	if (heap == NULL) {
		heap = (unsigned char *) &_end;
	}
	prev_heap = heap;

	heap += incr;

	return (caddr_t) prev_heap;
}

__attribute__ ((used))
int link(char *old, char *new) {
	return -1;
}

__attribute__ ((used))
int _close(int file) {
	return -1;
}

__attribute__ ((used))
int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

__attribute__ ((used))
int _isatty(int file) {
	return 1;
}

__attribute__ ((used))
int _lseek(int file, int ptr, int dir) {
	return 0;
}

/*Low layer read(input) function*/
__attribute__ ((used))
int _read(int file, char *ptr, int len) {

#if 0
	//user code example
	int i;
	(void)file;

	for(i = 0; i < len; i++)
	{
		// UART_GetChar is user's basic input function
		*ptr++ = UART_GetChar();
	}

#endif

	return len;
}

/*Low layer write(output) function*/
__attribute__ ((used))
int _write(int file, char *ptr, int len) {

#if 0
	//user code example

	int i;
	(void)file;

	for(i = 0; i < len; i++)
	{
		// UART_PutChar is user's basic output function
		UART_PutChar(*ptr++);
	}
#endif

	return len;
}

__attribute__ ((used))
void abort(void) {
	/* Abort called */
	while (1)
		;
}

/* --------------------------------- End Of File ------------------------------ */
