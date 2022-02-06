/**
 * @file licensePlate.hpp
 * @author PIgate
 * @brief License Plate recognition module include
 * @version 0.1
 * @date 2022-02-06
 *
 * @copyright Copyright (c) 2022
 * 
 */

#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/opencv2/opencv.hpp"
#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/opencv2/objdetect.hpp"
#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/opencv2/highgui.hpp"
#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/opencv2/imgproc.hpp"
#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/opencv2/videoio.hpp"

using namespace std;
using namespace cv;

//defines the camera device to use
#define CAMERA "/dev/video0"


/**
 * @brief loads the cascade model for recognizing license plates
 * 
 * @return EXIT_SUCCESS if all OK
 *          -EXIT_FAILURE if and error Occurs
 */
int init_cascade();


/**
 * @brief Takes a picture from the camera
 * 
 * @returns picture in CV::Mat structure
 */
Mat take_picture();


/**
 * @brief detects a plate in a given image
 * 
 * @param frame image to be analysed
 * @param len photo index
 * 
 * @return -EXIT_FAILURE if no plate found
 *          photo index in success
 */
int detectPlate( Mat , int );

/**
 * @brief runs tesseract character recognition on a found license plate
 * 
 * @param plate croped picture with a license plate
 * @return ERROR if something wrong occurs
 *          Detected String
 */
string read_license_plates( uint16_t );