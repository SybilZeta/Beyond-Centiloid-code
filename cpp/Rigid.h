#pragma once
#include <tuple>
#include <unordered_map>

#include "common.h"
#include "itkAffineTransform.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkCropImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkIntensityWindowingImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkStatisticsImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "onnxruntime_cxx_api.h"

using TransformType = itk::AffineTransform<double, 3>;

class Rigid {
 public:
  Rigid(const std::string& modelPath, const std::string& voxelMorphPath);
  ~Rigid();
  ImageType::Pointer preprocess(ImageType::Pointer image);
  ImageType::Pointer preprocessVoxelMorph(ImageType::Pointer image);
  std::unordered_map<std::string, std::vector<float>> predict(
      std::vector<float> inputTensor, const std::vector<int64_t> inputShape);
  std::unordered_map<std::string, std::vector<float>> predictVoxelMorph(
      std::vector<float> originalImg, std::vector<float> movingImg,
      std::vector<float> templateImg);
  std::tuple<ImageType::PointType, ImageType::DirectionType>
  getNewOriginAndDirection(
      ImageType::Pointer preprocessedImage, ImageType::Pointer originalImage,
      std::vector<float> ac, std::vector<float> pa,
      std::vector<float>
          is);  // anterior commisure position, posterior-anterior direction,
                // inferior-superior direction in voxel space
 private:
  Ort::Env env;
  Ort::Session* session;
  Ort::Session* voxelMorphSession;
};
