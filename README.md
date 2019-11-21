![gif](13.gif)
### cross
**Bue** cTe c npegaBaHeTo 3a Ky7Typa no KaHa7 -1 *nuK7uBo kt7To*
TeMaTa B gHemHus Hu 6pou' e *upoHusTa B pa3nsTueTo Ha
XpucToc* xa-xa ..
**Yeah** basically the society sucks it is nothing more than
a facebook, people speaking nicely, smiling etc. hiding
their real intentions behind a false mask. I will always
be an outsider you worth a shit.

Back to skool, this is the *4th* problem, it's about a game:)
this time two players are crossing squares on a piece of paper.
There are *N* squares and on each move a player crosses a length
of *K* consecutive squares. The winner is the player with the last
move. You can view details 
[here](https://ioinformatics.org/files/ioi1990round1.pdf).
It's a nice problem, by now we are walking many game trees so
the different thing here is how to find the winning strategy.

If we imagine a game tree all ***terminal*** nodes will represent
variations in which the terminal node player is losing.
The trick here is to use an *auxiliary* stack for walking the tree
using the usual non-recursive algorithm. Veneva in the current
position **P** all moves are exhausted we are popping a predecessor
*Q* and setting the winner of that node as the winner of **P** (for
all terminal nodes the winner is the opposite of the player).
If the player of *Q* and the winner of **P** are the same than we
don't need to go any further coz *Q* can choose which variant to
play. And basically that's it. The program implements the position
as a *64 bit* integer zeroes being crossed squares. All terminal nodes
for which *W playr* is winq are pushed on a term stack, and after
finishing the journey the most resent entry will represent the
winning strategy. Here is a backtrace for such a variant:
```
$ ./cross -n 40 -k 7
N = 40, K = 7
pos: 0000000000000000000001111100000000000000
movstk:
playr: B
winq: W
parent: 0x55845275e1f0
termflag: 1

pos: 0000000111111100000001111100000000000000
movstk:
playr: W
winq: W
parent: 0x55845275dc20
termflag: 0

pos: 0000000111111100000001111111111110000000
movstk:
playr: B
winq: W
parent: 0x55845275da90
termflag: 0

pos: 1111111111111100000001111111111110000000
movstk: 32 31 30 29 28 27 26 12 11 10 9 8 7
playr: W
winq: W
parent: 0x558452755260
termflag: 0

pos: 1111111111111100000001111111111111111111
movstk:
playr: B
winq: W
parent: 0x558452702670
termflag: 0

pos: 1111111111111111111111111111111111111111
movstk: 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
playr: W
winq: W
parent: (nil)
termflag: 0
```
https://youtu.be/cjVQ36NhbMk
