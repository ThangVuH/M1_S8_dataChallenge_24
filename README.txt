# This folder contains 3 pandas datasets:
* `scene.hdf5` : contains a sequence of a natural scene images (14bits images)
* `scene2.hdf5` : contains a sequence of a natural scene images (14bits images)
* `scene3.hdf5` : contains a sequence of a natural scene images (14bits images)
* `calibration_set_1.hdf5` : contains some calibration images (14bits images)
* `calibration_set_2.hdf5` : contains another set of calibration images (14bits images)

Those files contains pandas (v1.3.3) dataframe and can be opened in python with :
```
import pandas as pd
data = pd.read_hdf("xxxx.hdf5")
```
(replacing `xxxx.hdf5` by the filename)

Each of those Dataframe contains a serie of images with their meta-datas.

Correcting the scenes using `calibration_set_1.hdf5` is **easier** than using `calibration_set_2.hdf5`.

# The usefull columns of the dataframes are :
`image` : the RAW image (numpy array)
`cint`, `gsk`, `gsk_volt`, `tint`, `gfid` : some electronics parameters
`vtemp_plot` : tension (V) of a camera temperature sensor
`vtemp_ghostline`: numeric version of `vtemp_plot`
`t_cn` : black body temperature (if applicable)
`t_etuve` : measured hoven temperature
`t_etuve_set` : set hoven temperature
`t_fpa` : converted `vtemp_plot` into degree °C
`timestamp` : time
`date` : formated version of timestamp
`framerate` : camera framerate (Hz)
`brms` : the temporal standard deviation image of the pixels (temporal noise) (if applicable).