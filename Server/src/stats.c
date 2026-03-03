
#include "../headers/server.h"

#define MAX_PARTIES 15


void makeStats(char *statsf,char *logf){
    char line[102];
    char *temp;
    char *token;
    char entry[MESS_SIZE];
    Party *parties=malloc(sizeof(Party)*MAX_PARTIES);     // Array to hold parties
    int i,found=0,count=0;
    int total=0;

    FILE * stats=fopen(statsf, "w+") ,*log=fopen(logf, "r");          // Open log file
    if (log<0){
                perror("Failed to open log file\n");

            }

    if (stats<0){
                perror("Failed to open stats file\n");

            }


    // Read and process each line from the file
    while (fgets(line, 102, log) != NULL) {

        if(strlen(line)>1){
       temp=line;
         if (temp == NULL) {
        fprintf(stderr, "Failed to duplicate string.\n");

            }



        token=strtok_r(temp," ",&temp);         // First name

        token=strtok_r(temp," ",&temp);       // last name

        token=strtok_r(temp," ",&temp);       // Party
         token[strlen(token)-1]=0;
                                            // Remove newline char



        if(count==0){                // First party to be put in array
            parties[0].name=strdup(token);


            parties[0].votes=1;

            count++;
        }
        else{
        for (i = 0; i < count; i++) {


            if (strcmp(parties[i].name, token) == 0) {       //found the party

                parties[i].votes++;

                found=1;

                break;
            }
           found=0;
             }

        if(!found){     // If the party does not exist in the array


             parties[count].name=strdup(token);

            parties[count].votes=1;

            count++;
                }
            }

        }
         }
    for(i=0; i<count; i++){

        total+=parties[i].votes;
        sprintf(entry,"%s %d\n",parties[i].name,parties[i].votes);
         if (fprintf(stats,"%s",entry) < 0){                    // Write to stats file
                perror("Writing error\n");
            }
    }
    sprintf(entry,"TOTAL VOTES: %d",total);
    if (fprintf(stats,"%s",entry) < 0){                    // Write last line
                perror("Writing error\n");
            }
    fclose(log);
    fclose(stats);
    printf("Election completed, results are available in specified file.\n");

    for(i=0; i<count; i++){
        free(parties[i].name);
    }
    free(parties);
}
