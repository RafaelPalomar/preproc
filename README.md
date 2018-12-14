# preproc
Utility docker image and application to split medical volumes in 2D (resized) slices

A running container from this image would expect an input volume (directory) with medical images mounted at `/input_data` and an output volume mounted at `/output_volume`. In order to control the behaviour of the container, the user can define the following environment variables:

* `SIZE`: to output images `SIZE X SIZE`
* `FLIPX=1`: to flip the output images along the X axis
* `FLIPY=1`: to flip the output images along the Y axis
* `FORMAT`: to change the format of the output images (nrrd, nii, gipl, etc.). In general, any format recognized by ITK would work (not PNG though).

## Example
```
docker run rafaelpalomar/preproc -v /home/rafael/input_data:/input_data -v /home/rafael/output_data -e SIZE=256 -e FLIPX=1 -e FORMAT="nrrd"
```

In this example the images are stored in the `input_data` folder. A running container will find all the files (expected 3D medical volumes in single files....not DICOM) in the folder (including sub-folders....be careful!). For every file will create a directory in `output_data` which will contain individual output files (slices of 3D medical volumes along z axis...normally axial direction). The output images will be resized to 256x256 and mirrored respect to the X axis.
