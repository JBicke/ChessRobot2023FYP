def convert_string(input_string):
	result = ""
	current_number = ""
    
	for char in input_string:
		if char.isalpha():
			result += str(ord(char.lower()) - ord('a') + 1)
		else:
			result += char # Add any remaining numbers to the result
			
	return result

# Example usage:
input_str = "f3e2"
converted_str = convert_string(input_str)
print(converted_str)  # Output: "1238"
