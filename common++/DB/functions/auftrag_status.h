
#ifndef AUFSTATUS
#define AUFSTATUS

typedef enum {UNCOMMITED=0, OPEN, CLOSED, STORNO} AufStatVal;
typedef enum {REQUEST=0, CONFIRM, PROBLEM} AufChangeStatVal;
typedef enum {MENGE='M', LIEFDATUM='D'} AufChangeTypeVal;

#endif
