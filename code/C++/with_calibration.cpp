#include <opencv2/opencv.hpp>
#include <armadillo>
#include <H5Cpp.h>
#include <iostream>
#include <vector>
#include <string>

// Define device (CPU for simplicity)
#define DEVICE_CPU

// Read HDF5 file and return dataset as arma::mat
arma::mat read_hdf5(const std::string& file_path, const std::string& dataset_name) {
    H5::H5File file(file_path, H5F_ACC_RDONLY);
    H5::DataSet dataset = file.openDataSet(dataset_name);
    H5::DataSpace dataspace = dataset.getSpace();
    hsize_t dims[2];
    dataspace.getSimpleExtentDims(dims, NULL);

    arma::mat data(dims[0], dims[1]);
    dataset.read(data.memptr(), H5::PredType::NATIVE_FLOAT);

    return data;
}

// Process data into tensor-like structure (arma::cube)
arma::cube process_data_tensor(const arma::mat& df) {
    int num_images = df.n_rows;
    int img_size = sqrt(df.n_cols); // Assuming square images

    arma::cube images(img_size, img_size, num_images);
    for (int i = 0; i < num_images; ++i) {
        images.slice(i) = arma::mat(df.row(i).begin(), img_size, img_size);
    }
    return images;
}

// Estimate parameters using linear regression
std::pair<arma::mat, arma::cube> estimate_parameters(const arma::mat& calibration_images, const arma::mat& calibration_temperatures) {
    int num_images = calibration_images.n_rows;
    int img_size = sqrt(calibration_images.n_cols); // Assuming square images
    int num_pixels = img_size * img_size;

    arma::cube calibration_images_tensor = process_data_tensor(calibration_images);

    // Add bias term for intercept in linear regression
    arma::mat X = arma::join_horiz(calibration_temperatures, arma::ones(num_images, 1));

    // Perform linear regression
    arma::mat images_reshaped = arma::reshape(calibration_images_tensor, num_pixels, num_images).t();
    arma::mat responsivities_and_offsets = arma::solve(X, images_reshaped);

    // Separate responsivities and offsets
    arma::mat responsivities = responsivities_and_offsets.head_cols(num_pixels).t();
    arma::vec offsets = responsivities_and_offsets.tail_cols(1);

    // Reshape back to original image shape
    offsets.reshape(img_size, img_size);
    responsivities.reshape(img_size, img_size, 2);

    return { offsets, responsivities };
}

// Apply correction to scene image
arma::mat apply_correction(const arma::mat& scene_image, const arma::mat& offsets, const arma::cube& responsivities, float scene_temp, float avg_black_body_temp) {
    int img_size = scene_image.n_rows;
    arma::mat corrected_image(img_size, img_size);

    arma::mat intercepts = offsets;
    arma::mat coef_fpa = responsivities.slice(0);
    arma::mat coef_cn = responsivities.slice(1);
    arma::mat expected_value = intercepts + coef_fpa * scene_temp + coef_cn * avg_black_body_temp;

    corrected_image = scene_image - expected_value;

    return corrected_image;
}

// Main driver function
std::vector<arma::mat> applier(const arma::mat& calibration_images, const arma::mat& calibration_temperatures, const arma::mat& scene_images, const arma::vec& scene_temperatures) {
    auto [offsets, responsivities] = estimate_parameters(calibration_images, calibration_temperatures);
    float avg_black_body_temp = arma::mean(calibration_temperatures.col(1));

    std::vector<arma::mat> corrected_scene_images;
    for (size_t i = 0; i < scene_images.n_rows; ++i) {
        arma::mat scene_image = arma::mat(scene_images.row(i).begin(), sqrt(scene_images.n_cols), sqrt(scene_images.n_cols));
        float scene_temp = scene_temperatures(i);
        arma::mat corrected_image = apply_correction(scene_image, offsets, responsivities, scene_temp, avg_black_body_temp);
        corrected_scene_images.push_back(corrected_image);
    }

    return corrected_scene_images;
}

int main() {
    std::string dataset_location = "C:/Users/thang/OneDrive/Desktop/school/DataChallenge/dataset_for_students/";

    arma::mat scene_df_1 = read_hdf5(dataset_location + "scene.hdf5", "dataset_name");
    arma::mat calibration_df_2 = read_hdf5(dataset_location + "calibration_set_2.hdf5", "dataset_name");

    arma::mat calibration_images = calibration_df_2.cols(0, calibration_df_2.n_cols - 3); // assuming last two columns are temperatures
    arma::mat calibration_temperatures = calibration_df_2.cols(calibration_df_2.n_cols - 2, calibration_df_2.n_cols - 1);
    arma::mat scene_images = scene_df_1.cols(0, scene_df_1.n_cols - 2); // assuming last column is temperature
    arma::vec scene_temperatures = scene_df_1.col(scene_df_1.n_cols - 2);

    auto corrected_scene_images = applier(calibration_images, calibration_temperatures, scene_images, scene_temperatures);

    // Example output display (first corrected image)
    cv::Mat img_display(corrected_scene_images[0].n_rows, corrected_scene_images[0].n_cols, CV_32F, corrected_scene_images[0].memptr());
    cv::imshow("Corrected Image", img_display);
    cv::waitKey(0);

    return 0;
}
