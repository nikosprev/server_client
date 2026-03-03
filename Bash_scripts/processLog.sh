

#!/bin/bash

# Check if the input file is provided as an argument
if [ $# -eq 0 ]; then
    echo "Usage: ./processLog.sh poll_log"
    exit 1
fi

filename="$1"
path="../"                # Log file in the previous folder
output="pollerResultsFile"
                            # Concatenate
input="${path}${filename}"

if [ ! -f "$input" ]; then
  echo "Input file '$input' not found."
  exit 1
fi

# Associative array to store party votes
declare -A party_votes

# Read the input file and count the votes for each party
while read -r first_name last_name party
do
    ((party_votes[$party]++))
done < "$input"

# Write the results to the output file
echo "" > "$output"        #Overwrite if exists
for party in "${!party_votes[@]}"
do
    echo "$party ${party_votes[$party]}" >> "$output"
done

# Calculate the total votes
total_votes=0
for votes in "${party_votes[@]}"
do
    ((total_votes += votes))
done

# Append the total votes to the output file
sort "$output" -o "$output" #sort
echo "Total votes: $total_votes" >> "$output"

echo "Processing completed. Results are available in $output."