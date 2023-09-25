import re

def extract_eP(text):
	en_passant_pattern = r'\s([^0-9]+)\s(d+)\s(\d+)$'
	match = re.search(en_passant_pattern, text)
	
	if match:
		information = match.group(1)
		return information
	else:
		return None
		
def extract_eP(text):
	words = text.split()
	
	return words[-3]

text = 'e7e5 cp 161 Fen: rnbqkbnr/pp1ppppp/8/8/2PpP3/8/PP3PPP/RNBQKBNR b KQkq c3 0 3'

working = extract_eP(text)
print(working)		

