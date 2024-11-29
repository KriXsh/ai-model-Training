# promt - Code Ecosystem	Automated Testing	
#Verify the correct display of time zones in a scheduling app.


#!/bin/bash

# Define the function to show the current time in a specific timezone
show_time_in_timezone() {
    local timezone=$1

    # Validate the timezone by checking if it exists in the system's timezone database
    if [ ! -f "/usr/share/zoneinfo/$timezone" ]; then
        echo "Error: Unknown timezone '$timezone'"
        return 1
    fi

    # Fetch the current time for the valid timezone
    local current_time=$(TZ=$timezone date '+%Y-%m-%d %H:%M:%S')
    echo "Current time in $timezone: $current_time"
}

# Define the test function to validate outputs
test_show_time_in_timezone() {
    local timezone=$1
    local expected_output=$2

    # Call the function and capture its output
    local output=$(show_time_in_timezone "$timezone")

    # Check if the output matches the expected output
    if [[ $output == "$expected_output"* ]]; then
        echo "PASS: $expected_output"
    else
        echo "FAIL: Expected '$expected_output' but got '$output'"
    fi
}

# Define test cases with expected results
test_cases=(
    "America/New_York Current time in America/New_York:"
    "Europe/London Current time in Europe/London:"
    "Asia/Kolkata Current time in Asia/Kolkata:"
    "Australia/Sydney Current time in Australia/Sydney:"
    "Invalid/Timezone Error: Unknown timezone 'Invalid/Timezone'"
    " Error: Unknown timezone ''"
)

# Run the test cases
echo "Running test cases..."
for test_case in "${test_cases[@]}"; do
    # Extract timezone and expected output from each test case
    timezone=$(echo "$test_case" | awk '{print $1}')
    expected_output=$(echo "$test_case" | cut -d' ' -f2-)

    # Execute the test function
    test_show_time_in_timezone "$timezone" "$expected_output"
done
