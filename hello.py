#!/usr/bin/python
import time
import ekk
#print(ekk.getRunFlag())
print('hello to buffer')
while True:
	#print(ekk.getRunFlag())
	if ekk.getRunFlag()==0:
		break
	time.sleep(3)
	print(3.14)
	#print(ekk.getRunFlag())
	if ekk.getRunFlag()==0:
		break
	time.sleep(3)
	print('still talking to buffer')
	#print(ekk.getRunFlag())
	if ekk.getRunFlag()==0:
		break
	time.sleep(3)
	print(3.14)
	break
print("all done")