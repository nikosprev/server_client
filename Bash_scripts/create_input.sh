#!/bin/bash

                              # Check number of arguments
if [ "$#" -ne 2 ]; then
  echo "Usage: ./create_input.sh parties.txt numlines"
  exit 1
fi

parties_file="$1"
numlines="$2"
output_file="../inputfile.txt"  # Inputfile should be in the previous directory

                                    # Check if the parties file exists
if [ ! -f "$parties_file" ]; then
  echo "Parties file '$parties_file' not found."
  exit 1
fi

                                  # Read parties from the file into an array
IFS=$'\r\n' GLOBIGNORE='*' command eval 'parties=($(cat "$parties_file"))'

                                  # Random name
random_name() {
  local length=$((RANDOM % 10 + 3))
  tr -dc 'a-zA-Z' < /dev/urandom | head -c "$length"
}
random_last() {                                   # Random last name
  local length=$((RANDOM % 10 + 3))
  tr -dc 'a-zA-Z' < /dev/urandom | head -c "$length"
}

random_party() {                                   #Random party
  local num_parties=${#parties[@]}
  local index=$((RANDOM % num_parties))
  echo "${parties[$index]}"
}

                                            # Create the output file
if [ -f "$output_file" ]; then
  echo "Output file '$output_file' already exists. Overwriting..."
fi

for ((i=0; i<numlines; i++)); do                        # (Over)write
  name=$(random_name)
  last=$(random_last)
  party=$(random_party)

  echo "$name $last $party"

done > "$output_file"





echo "Output file '$output_file' created successfully."
