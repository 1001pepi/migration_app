brctl addbr xenbr0
brctl addif usb0
ifconfig usb0 0.0.0.0
ifconfig xenbr0 [ADRESSE_IP_OF_EHTERNET_DEVICE]