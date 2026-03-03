#!/bin/bash

                                      # Check arguments
if [ "$#" -ne 1 ]; then
  echo "Usage: ./tallyVotes.sh tallyResultsFile"
  exit 1
fi

input_file="../inputfile.txt"   # Path to the input file
tally_results_file="$1"

                                # Check if the input file exists
if [ ! -f "$input_file" ]; then
  echo "Input file '$input_file' not found. Run the 'create.sh' script first."
  exit 1
fi

                                    # Associative array to store the vote counts
declare -A vote_counts
total_votes=0
                                        # Read and count the votes
while IFS=" " read -r first_name last_name party || [ -n "$first_name" ]; do
  if [[ ! "${vote_counts[$first_name $last_name]}" ]]; then
    vote_counts[$first_name $last_name]=$party  # Keep only First occurrence of a name
  fi
done < "$input_file"

                                    # Associative array to store the party counts
declare -A party_counts

# Count the votes for each party
for party in "${vote_counts[@]}"; do
 total_votes=$((total_votes + 1))
  if [[ ! "${party_counts[$party]}" ]]; then
    party_counts[$party]=1          # First occurrence of a party
  else
    ((party_counts[$party]++))

  fi
done

                                          # Create the tally results file
if [ -f "$tally_results_file" ]; then
  echo "Tally results file '$tally_results_file' already exists. Overwriting..."
fi

for party in "${!party_counts[@]}"; do              ## Overwrite if it exists
  echo "$party: ${party_counts[$party]}"
done > "$tally_results_file"

sort "$tally_results_file" -o "$tally_results_file"
echo "Total Votes: $total_votes" >> "$tally_results_file"
echo "Tally results file '$tally_results_file' created successfully."
