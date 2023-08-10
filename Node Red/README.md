# Node Red

Node Red is a powerfool tool and is freely available for the Raspberry Pi it makes programming it easier and I will cover this in this repository.

Once the Raspberry Pi for is setup for the first time we can use the following lines of code to get the latest version of Node Red.

You need to open the comand line and copy and paste the following.

    bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)

You will get asked 2 questions and you answer the first as yes and the 2nd one as yes.

You then need to copy and paste the following to start Node Red on Boot Up of the Raspberyy Pi 4.

    sudo systemctl enable nodered.service

Moder details can be seen in this video I made https://youtu.be/31TqqJM__X4

-----

