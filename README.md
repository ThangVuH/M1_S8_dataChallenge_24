# Lynred data challenge
**What is hidden inside this thermal image ?**

The challenges to be solved: 

* NUC the first sequence of images recorded in the same environnement as the calibration images
  * We called this the "controlled environment" calibration set.
* NUC the second sequence of images recorded in the another environnement of the calibration images.
  * We called this the "black body" calibration set.
* NUC the third sequence of images sequence without any calibration.

## An overview of provided files and how to run
* The "video" directory contains all of the videos from methods we attempted to use, or experiments we did.
* The "code" directory contains:
  * final notebooks -- these will walk you through the code of our major experiments and the final approach for each level of the challenge
  * test and replication -- more usable interface and utilities for running the third challenge denoising algorithm on videos and gathering metrics
  * test notebooks -- our experiments testing different methodologies for denoising images, mostly a papertrail left for documentation purposes, it is more useful to use the final notebooks which were made with replicability in mind
* The "final" and "test and replication" notebooks are documented internally, i.e. they include some explanations of approach and how to run. 

For deeper explanations of our approaches, see [the linked report](https://docs.google.com/document/d/1AgLrvQ948yOEm715Q-wKb95xRIVeTkXzkjA1T3MV2JM/edit?usp=sharing).
