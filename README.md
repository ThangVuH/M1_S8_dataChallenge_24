# Lynred data challenge
**What is hidden inside this thermal image ?**

The challenges to be solved: 

* NUC the first sequence of images recorded in the same environnement as the calibration images
  * We called this the "controlled environment" calibration set.
* NUC the second sequence of images recorded in the another environnement of the calibration images.
  * We called this the "black body" calibration set.
* NUC the third sequence of images sequence without any calibration.

## An overview of provided files and how to run
* The "video" directory contains all of the videos from methods we attempted to use, or experiments we did. Some are not relevant, in the following report, we will reference the pertinent video for the methods we are describing.
* The "code" directory contains:
  * final notebooks -- these will walk you through the code of our major experiments and the final approach for each level of the challenge
  * test and replication -- more usable interface and utilities for running the third challenge denoising algorithm on videos and gathering metrics
  * test notebooks -- our experiments testing different methodologies for denoising images, mostly a papertrail left for documentation purposes, it is more useful to use the final notebooks which were made with replicability in mind
* The "final" and "test and replication" notebooks are documented internally, i.e. they include some explanations of approach and how to run. For deeper explanations of our approaches, see the following section.

## Our solutions to the three levels of challenge
### Controlled environment calibration
Initially, in our attempts to understand the challenge, we began with some basic filtering and the idea of constructing a filter from an average of the calibration dataset. We later learned this approach has a name -- temporal averaging. Temporal averaging did an ok job, but some of the scenes were left very dark.

Iterating on this idea of constructing a filter and with some words from Ronald Phllypo, we discovered we could model the response (or gain) and offset of the sensor using linear regression. This approach worked well, but we discovered it was, in fact, overkill for the first challenge. It would prove more useful for the second challenge - black body calibration.

Returning to the basics, we realized we could find the calibration image which had the closest temperature to each frame of the scene. This provided the best results we had seen -- see `video/with_controlled_env_calibration/calibration_matching/output-matching.avi` for results -- and would act as a baseline for all future results.

### Black body calibration -- AR
TODO: Explain black body methods

### No calibration -- CT
TODO: 
* WP to explain the variance stacking and blocks
* CT to explain and how/why we arrived at the stride method
* WP to explain auto-adjustment of contrast

#### Metrics -- TV
TODO: Some metrics?

#### Remaining challenges -- WP
TODO: Discuss about why the shutter approach isnt the best -- and how always updating to find some "maximal" pseudo-calibration live or right after capturing video might be feasible. 