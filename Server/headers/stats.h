

#ifndef STATS_H
#define STATS_H

typedef struct party {      // A political party - storing its name and the votes
    char *name;
    int votes;
} Party;


void makeStats(char *,char *);

#endif