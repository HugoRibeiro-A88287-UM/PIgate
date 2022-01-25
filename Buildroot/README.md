
# Buildroot Configuration

For this project the following packages are needed.

To add them in buildroot, type make menuconfig.

Note: By adding all the packages, the ROOT file system storage couldn't be enought, so to prevent that, please increase that memory in BR2_TARGET_ROOTFS_EXT2_SIZE






## Packages

-> System Configuration

	    dev management = devtmpfs + eudev

-> Target packages

    Audio and Video
		gstreamer 1.x 
			gst1-plugins-base
			gst1-plugins-good (jpeg e png support)
			gst1-plugins-bad (camerabin2)
		v4l2grab
		v4l2loopback + utils
		ffmpeg
  
	Interpreter languages and scripting
    
		python3 - Select
        module format (.py and .pyc)
		External python modules  
        
            -python-aenum
            -python-cchardet
            -python-googleapis-common
            -python-oauth2client
            -python-pip
            -python-jwt
            -python-protobuf
            -python-pycryptodome
            -python-requests-toolbelt
            -python-asn1crypto
            -python-certifi
            -python-cffi
            -python-chardet
            -python-charset-normalizer
            -python-cryptography
            -python-deprecated
            -python-httplib2
            -python-idna
            -python-jwcrypto
            -python-jwt
            -python-pyasn1
            -python-pyasn1-modules
            -python-pyparsing
            -python-requests
            -python-rsa
            -python-setuptools
            -python-six
            -python-urllib3
            -python-wrapt
            -python3-yaml
            -python3-ply
            -python3-jinja2

   -> Networking applications
   
        crda
        dhcpcd
        dropbear
            -client programs
            -optimize for size
        ifupdown scripts
        iprout2
        iw
        ntp
            -ntpd
        wireless-regdb
        wireless-tools
        wpa_supplicant
            -Enable nl80211 support
            -Enable autoscan
            -Install wpa_cli binary                                                  
            -Install wpa_client shared library                                        
            -Install wpa_passphrase binary 
  
  -> Hardware Handling

  	Firmware
        rpi-firmware (variant rpi4, firmware to boot = extended) + DTB overlays + vcdbg
  		rpi-wifi-firmware
    
    rpi-userland

  -> Libraries

			 Graphics
				jpeg support (variant jpeg)	
			 Hardware Handling
				bcm2835
				libv4l + utils tools
			 Multimedia
				libcamera (all)
			 Other
				libevent  
	 
  -> Text editors and viewers

        nano
        optimize for size
## Config.txt

Config.txt is present in output/images/rpi-firmware folder.

To make the camera and the PWM work, the config.txt needs to be modify. Please put/uncomment the following lines

    gpu_mem_256=512

    gpu_mem_512=512

    gpu_mem_1024=512

To enable pwm

    dtoverlay=pwm-2chan

To enable camera

    dtparam=i2c_arm=on
    dtparam=i2c_vc=on
    dtoverlay=vc4-fkms-v3d
    start_x=1

Note: Remove or comment the following lines if they apper.

    dtoverlay=ov5647
    dtoverlay=imx219
    dtoverlay=imx477
    dtoverlay=imx290,clock-frequency=74250000
    dtoverlay=imx290,clock-frequency=37125000 
    dtoverlay=imx378
    dtoverlay=ov9281
    camera_auto_detect=1/0
 

## Wifi

In order to make wifi work with adding it in raspberry, go to board/raspberrypi4 folder.

Then add the following line in post-build.sh

    cp board/raspberrypi4/interfaces ${TARGET_DIR}/etc/network/interfaces
    cp board/raspberrypi4/inittab ${TARGET_DIR}/etc/inittab
    cp board/raspberrypi4/wpa_supplicant.conf ${TARGET_DIR}/etc/wpa_supplicant.conf

After it, add interfaces, inittab and wpa_supplicant files in board/raspberrypi4 directory, with the proper configurations.

## Authors

- [@HugoRibeiro](https://github.com/HugoRibeiro-A88287-UM/)

