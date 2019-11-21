////////________````````        ]]]]]]]]        --------########
#include <stdio.h>
#include <stdlib.h>
////////        ++++++++        --------        ````````########
// There is a general macros Berkeley library for 
// handling lists, deques and stuff in C (sys/queue.h), but
// decided to write something less simple. It doesn't need very
// much of an explanation, but it's a linked data structure with
// a head (ahead), or tail depending on the view point. There is
// no UNDERFLOW ck, so before popping:) the is_empty stuff _____
// should be cked. _____________________________________________
#define stack(NOM, TYPE)				\
	struct NOM {					\
		TYPE        info;			\
		struct NOM *next;			\
	};						\
	struct NOM *cons##NOM(TYPE info)		\
	{						\
		struct NOM *p = malloc(sizeof *p);	\
							\
		p->info = info;				\
		p->next = NULL;				\
							\
		return p;				\
	}						\
	void push##NOM(struct NOM *head, TYPE info)	\
	{						\
		struct NOM *p = cons##NOM(info);	\
							\
		p->next = head->next;			\
		head->next = p;				\
	}						\
	TYPE pop##NOM(struct NOM *head)			\
	{						\
		struct NOM *p = head->next;		\
							\
		TYPE info = p->info;			\
		head->next = p->next;			\
		free(p);				\
							\
		return info;				\
	}
////////@@@@@@@@        >>>>>>>>________////////########........
#define is_empty(HEAD) ((HEAD)->next == NULL)
#define stkloop(HEAD, F) {			\
		__auto_type p = HEAD;		\
		__auto_type q = p->next;	\
						\
		while (q) {			\
			p = q;			\
			q = p->next;		\
			F(p);			\
		}				\
	}
#define clear(HEAD) stkloop(HEAD, free)
////////>>>>>>>>________,,,,,,,,********========        ########
stack(mov, int);
void dumpmov(struct mov *p) { printf("%d ", p->info); }
////////********--------________\\\\\\\\>>>>>>>>++++++++::::::::
#include <stdint.h>
int gN = 8;
int gK = 5;
#define mask(LEN, OFFSET) (((1LL << (LEN)) - 1) << (OFFSET))
int64_t cross(int64_t pos, int len, int offset)
{
	int64_t mask = mask(len, offset);
	
	return (pos & (~mask));
}
////////--------        oooooooo________<<<<<<<<        ////////
struct mov *getmoves(int64_t pos)
{
	int64_t mask = mask(gK, 0);
	struct mov *ahead = consmov(0);

	for (int j = 0; j < gN - gK + 1; j++) {
		if ((pos & mask) == mask) {
			pushmov(ahead, j);
		}
		mask <<= 1;
	}
	return ahead;
}
////////********        --------^^^^^^^^,,,,,,,,>>>>>>>>########
struct nod3 {
	int64_t      pos;
	struct mov  *movstk;
	int          playr;
	int          winq;
	struct nod3 *parent;
	int          termflag;
};
struct nod3 *consnod3(int64_t pos, int playr, struct nod3 *parent)
{
	struct nod3 *n3 = malloc(sizeof *n3);

	n3->pos      = pos;
	n3->movstk   = getmoves(pos);
	n3->playr    = playr;
	n3->winq     = !playr;
	n3->parent   = parent;
	n3->termflag = is_empty(n3->movstk);

	return n3;
}
struct nod3 *nextmove(struct nod3 *n3)
{
	if (is_empty(n3->movstk)) return NULL;

	int offset = popmov(n3->movstk);
	int64_t pos = cross(n3->pos, gK, offset);

	return consnod3(pos, !n3->playr, n3);
}
char *ptos(int64_t pos) // position to string
{
	static char s[64];

	int64_t mask = 1LL << (gN - 1);

	for (int j = 0; j < gN; j++) {
		s[j] = (pos & mask)? '1': '0';
		mask >>= 1;
	}
	return s;
}
void dumpnod3(struct nod3 *n3)
{
	static char P[] = {'W', 'B'};

	printf("pos: %s\n", ptos(n3->pos));
	printf("movstk: ");
	stkloop(n3->movstk, dumpmov);
	printf("\nplayr: %c\n", P[n3->playr]);
	printf("winq: %c\n", P[n3->winq]);
	printf("parent: %p\n", n3->parent);
	printf("termflag: %d\n", n3->termflag);
}
////////********________--------++++++++>>>>>>>>........,,,,,,,,
enum {W, B};
stack(rec, struct nod3 *);
void dumprec(struct rec *rec) { dumpnod3(rec->info); }
struct rec *journey(void)
{
	int64_t pos = mask(gN, 0);
	struct nod3 *q, *p = consnod3(pos, W, NULL);
	struct rec *rec = consrec(NULL);
	struct rec *term = consrec(NULL);	
	                                 		      	// [0. init]
	while (1) {
		q = nextmove(p);         		      	// [1. move]
		if (q) {	         		      	// [2. ck]
			pushrec(rec, p);  		      	//
			p = q;           		      	// [3. push]
		} else {
			if (p->termflag && (p->winq == W)) { 	// [4. term]
				pushrec(term, p);
			}
			while (1) {
				if (is_empty(rec)) goto exit;	// [5. void]
				q = poprec(rec);		// [6. pop]
				q->winq = p->winq;
				p = q;
				if (q->winq != q->playr) {	// [7. the q]
					break;
				}
			}
		}
	}
exit:	return term;
}
////////________````````--------<<<<<<<<        ========////////
#include <unistd.h>
void geropt(int argc, char *argv[])
{
	int opt;

	while ((opt = getopt(argc, argv, "k:n:")) != -1) {
		switch (opt) {
		case 'k':
			gK = atoi(optarg);
			break;
		case 'n':
			gN = atoi(optarg);
			break;
		default:
			printf("Usage: %s [-k K] [-n N]\n", argv[0]);
			exit(1);
		}
	}
}
void dumpvar(struct nod3 *n3)
{
	while (n3) {
		dumpnod3(n3);
		puts("");
		n3 = n3->parent;
	}
}
int main(int argc, char *argv[])
{
	geropt(argc, argv);

	printf("N = %d, K = %d\n", gN, gK);

	struct rec *term = journey();

	if (!is_empty(term)) {
		dumpvar(term->next->info);
	} else {
		printf("Black wins!");
	}
	return 0;
}
////////--------\\\\\\\\||||||||////////--------\\\\\\\\||||log:
// - Ha ge6e7us oT u3ocTaBa u nogo6Hu (moTo cTe egHaKBu),
// Tu cu 6oK7yK Be npusTe7! 
