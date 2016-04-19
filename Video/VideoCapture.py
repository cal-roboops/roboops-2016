#
# VideoCapture.py
# Python Project
#
# Created by Andrew Pau on 4/13/2016
# Edited by Mitchell Oleson on 4/14/2016
#
# Written for Debian Linux
#
#
# NOTE: We decided to use motion instead for simplicity
# and support. (sudo apt-get install motion -> then edit
# edit the config files)
#

import cv2

#Initialize the webcams
cam0 = cv2.VideoCapture(0)
cam1 = cv2.VideoCapture(1)
cam2 = cv2.VideoCapture(2)

#warmup time???
#Adjust resolution if not enough USB bandwidth
#Check if cameras are already open? 

# a loop to display cameras
while True:
	retval0, image0 = cam0.read
	retval1, image1 = cam1.read
	retval2, image2 = cam2.read
	#use namedWindow("name", WINDOW_NORMAL) if the window sizes aren't good
	cv2.imshow("Camera 0", image0)
	cv2.imshow("Camera 1", image1)
	cv2.imshow("Camera 2", image2)
	#Pressing 'q' quits the cameras
	if cv2.waitKey(1) & 0xFF == ord('q'):
		break

print("Exiting ...")
cam0.release()
cam1.release()
cam2.release()
cv2.destroyAllWindows()
