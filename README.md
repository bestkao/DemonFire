# DemonFire

We’ve built a CT image diagnostic system that utilize these imaging techniques to provide metrics on Head-Neck Cetuximah CT scans for further analysis.

By using multiplayer reconstruction to generate the scans for sagittal, coronal, and axial views, we used intensity-base rigid registration with mutual information maximization is to transform the different sets of data into a united coordinate system to make fair comparisons between the images.

Image segmentation through the 3D region growing method to partition images into multiple labelled segments sets it up for easier and more meaningful analysis. With this we're able to quantify the amount and rate of change in the segmented organs to screen for abnormal tissue growth or decay.

## Prerequisites

- [CMake](http://cmake.org) to build the project
- [ITK](http://itk.org), an extensive suite of software tools for image analysis.
- [VTK](http://itk.org) for visualization.
