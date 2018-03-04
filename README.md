
# SenseIMU
Control a mouse by human gesture via Bluetooth - A demonstration of Kalman Filter and other different optimization and estimation approaches.

## Input
* Accelerometer raw data
* Gyroscope raw data
* Magnetometer raw data

## Output
* Estimated Euler angles
* Estimated Quaternion set
* Represent object orientation on 3D Winform application
* Control mouse using estimated Euler angles via Bluetooth

## Functionality
* Bluetooth Connection
* Data plotting, 3D rendering
* Switching between different algorithms:
  - Tilt-compensation Method  
  - Gauss Newton Method 
  - AHRS
  - Quaternion based – gyro bias – EKF 
  - Quaternion based – gyro rate – EKF
  - DCM based KF 

## Screenshot

### HW Assembly  

<p>
  <img src="https://github.com/jimmyvo2410/SenseIMU/blob/master/_pic/HW0.jpg"
    width="40%" height="40%">
  <img src="https://github.com/jimmyvo2410/SenseIMU/blob/master/_pic/HW1.jpg" 
    width="40%" height="40%">
  <img src="https://github.com/jimmyvo2410/SenseIMU/blob/master/_pic/HW2.jpg" 
    width="40%" height="40%">
</p>

### Demonstration  
<p>
  <img src="https://github.com/jimmyvo2410/SenseIMU/blob/master/_pic/Demo1.jpg"
    >
  <img src="https://github.com/jimmyvo2410/SenseIMU/blob/master/_pic/Demo2.jpg" 
    >
</p>
